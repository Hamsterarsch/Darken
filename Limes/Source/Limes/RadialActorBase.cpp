// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialActorBase.h"
#include "PolarCollider.h"
#include "Misc/Limes.h"
#include "Building/Beacon/RTSStructureFactory.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Misc/RTSGameInstance.h"

//Public------------

ARadialActorBase::ARadialActorBase() :
	m_ActorWidthInCells{ 3 },
	m_ActorDepthInCells{ 2 },
	m_bIsCollisionInitialized{ false }
{	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("PolarRoot")));

	m_pCenteredRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CenteredRoot"));
	m_pCenteredRoot->SetupAttachment(GetRootComponent());


}


void ARadialActorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	
}

void ARadialActorBase::InitMinimumCollision(ARTSStructureFactory* pOwningFactory)
{
	m_pOwningFactory = pOwningFactory;
	auto DiscretizedPos{ GetActorLocation() };

	//DrawDebugCrosshairs(GetWorld(), DiscretizedPos, FRotator::ZeroRotator, 50, FColor::Red, true, 100);

	auto HullDepth{ m_pOwningFactory->GetCellDepth() * GetDepthInCells() };
	m_pCenteredRoot->SetRelativeLocation({ static_cast<float>(-HullDepth) * .5f, 0, 0 });

	auto PolarSystemCartesianOrigin{ m_pOwningFactory->GetCenteredRootLocation() };
	auto CartesianCenter{ PolarMath::SVector2D(DiscretizedPos.X - PolarSystemCartesianOrigin.X , DiscretizedPos.Y - PolarSystemCartesianOrigin.Y) };
	auto PolarLowrangeCenter{ CartesianCenter.ToPolar() };

	auto CellWidthAngle{ m_pOwningFactory->GetCellWidthAngle(PolarLowrangeCenter.Radius) };
	auto HalfBuildingWidth{ CellWidthAngle * GetWidthInCells() * .5 };
	PolarMath::CPolarCollider MinimumHull{ PolarLowrangeCenter,  HalfBuildingWidth, HullDepth };

	m_PolarCollision = PolarMath::CPolarCollision{ MinimumHull, pOwningFactory->GetPolarTransform(), m_pOwningFactory->GetCellWidthArc() *.5 };
	m_bIsCollisionInitialized = true;

	OnCollisionInitialized();
	SetupHullVisualization();


	
}

void ARadialActorBase::InitExpandedCollision(ARTSStructureFactory* pOwningFactory)
{
	m_pOwningFactory = pOwningFactory;
	auto DiscretizedPos{ GetActorLocation() };

	auto PolarSystemCartesianOrigin{ m_pOwningFactory->GetCenteredRootLocation() };
	//relative to owning factory
	auto CartesianCenter{ PolarMath::SVector2D(DiscretizedPos.X - PolarSystemCartesianOrigin.X , DiscretizedPos.Y - PolarSystemCartesianOrigin.Y) };
	auto PolarLowrangeCenter{ CartesianCenter.ToPolar() };
	auto LowrangeCellWidthAngle{ m_pOwningFactory->GetCellWidthAngle(PolarLowrangeCenter.Radius) };
	auto HalfMinBuildingWidth{ LowrangeCellWidthAngle * GetWidthInCells() * .5 };

	std::forward_list<PolarMath::CPolarCollider> lHulls{};
	for(unsigned int HullIndex{ 0 }; HullIndex < GetDepthInCells(); ++HullIndex)
	{
		//construct collision hulls for neighbor checking
		auto HullCellWidthAngle{ m_pOwningFactory->GetCellWidthAngle(PolarLowrangeCenter.Radius + m_pOwningFactory->GetCellDepth() * HullIndex) };

		auto HullLowrangeCenter{ PolarLowrangeCenter };
		HullLowrangeCenter.Radius += HullIndex * m_pOwningFactory->GetCellDepth();

		//Left neighbors
		auto LeftExpandedCenter{ HullLowrangeCenter };
		LeftExpandedCenter.Angle -= HullCellWidthAngle * .5;
		if (LeftExpandedCenter.Angle < 0)
		{
			LeftExpandedCenter.Angle = 360 + LeftExpandedCenter.Angle;
		}

		PolarMath::CPolarCollider LeftExpandedHull{ LeftExpandedCenter, HalfMinBuildingWidth + HullCellWidthAngle * .5, m_pOwningFactory->GetCellDepth() };

		//Right neighbors
		auto RightExpandedCenter{ HullLowrangeCenter };
		RightExpandedCenter.Angle = std::fmod(RightExpandedCenter.Angle + HullCellWidthAngle * .5, 360);

		PolarMath::CPolarCollider RightExpandedHull{ RightExpandedCenter, HalfMinBuildingWidth + HullCellWidthAngle * .5, m_pOwningFactory->GetCellDepth() };
		

		//check for any collisions in the owning factory with that hull
		auto LeftAngle{ PolarLowrangeCenter.Angle - HalfMinBuildingWidth };
		auto RightAngle{ std::fmod(PolarLowrangeCenter.Angle + HalfMinBuildingWidth, 360) };


		//get the right/left angle of the collider that was hit
		PolarMath::CPolarCollider Hit;
		if(m_pOwningFactory->HasIntersectionsWithChildBuildings(LeftExpandedHull, &Hit))
		{
			//because a valid minimum collision is a precondition for calling this function
			//a hit on the left side of the collision can only be caused by the right border of a different collider.
			LeftAngle = Hit.GetRightMin().Angle;
		}
		else if (LeftAngle < 0)
		{
			//do any correction of the LeftAngle's initialized value
			//if we do not overwrite it.
			LeftAngle = 360 + LeftAngle;
		}

		if(m_pOwningFactory->HasIntersectionsWithChildBuildings(RightExpandedHull, &Hit))
		{
			//see conditional left angle assignment
			RightAngle = Hit.GetLeftMin().Angle;
		}

			   
		//construct a hull encompassing the needed min/max angles
		auto HullWidthAngle{ PolarMath::GetShortestAngleBetween(LeftAngle, RightAngle) };
		auto CenterAngle{ std::fmod(LeftAngle + HullWidthAngle * .5, 360) };

		HullLowrangeCenter.Angle = CenterAngle;
		PolarMath::CPolarCollider FinalHull{ HullLowrangeCenter, HullWidthAngle * .5, m_pOwningFactory->GetCellDepth() };
		
		//add hull to the hull list
		lHulls.push_front(FinalHull);
		
	}
	//construct the collision
	m_PolarCollision = PolarMath::CPolarCollision{ std::move(lHulls), m_pOwningFactory->GetPolarTransform(), m_pOwningFactory->GetCellWidthArc() * .5 };
	m_bIsCollisionInitialized = true;

	OnCollisionInitialized();
	SetupHullVisualization();


}

FVector ARadialActorBase::GetRadialOrigin() const
{
	return m_pOwningFactory->GetActorLocation();
	
}

bool ARadialActorBase::HasIntersectionsWith(ARadialActorBase *pRadialActor) const
{
	if (!pRadialActor)
	{
		return true;
	}

	if (!m_bIsCollisionInitialized || !pRadialActor->m_bIsCollisionInitialized)
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ARadialActorBase:: intersecton check with uninitalized polar actors"));
		return true;
	}

	return m_PolarCollision.HasIntersectionsWith(pRadialActor->m_PolarCollision);


}

bool ARadialActorBase::HasIntersectionsWith(const PolarMath::CPolarCollider& HullToTest, PolarMath::CPolarCollider* out_pFirstHit) const
{
	auto PolarCenter{ m_pOwningFactory->GetPolarTransform() };
	PolarMath::CPolarCollision TempCollision{ HullToTest, PolarCenter, m_pOwningFactory->GetCellWidthArc() * .5 };

	return m_PolarCollision.HasIntersectionsWith(TempCollision, out_pFirstHit);


}

float ARadialActorBase::GetMainHullDepth() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullDepth() : 0;


}

float ARadialActorBase::GetMainHullRightAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullRightAngle() : 0;


}

float ARadialActorBase::GetMainHullLeftAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullLeftAngle() : 0;


}

float ARadialActorBase::GetMainHullMinRadius() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullMinRadius() : 0;


}

FVector2D ARadialActorBase::GetCollisionCartesianCenter() const
{
	auto CartesianCenter{ m_PolarCollision.GetCartesianCenter() };
	return m_bIsCollisionInitialized ? FVector2D{ static_cast<float>(CartesianCenter.X), static_cast<float>(CartesianCenter.Y) } : FVector2D::ZeroVector;


}

float ARadialActorBase::GetMainHullHalfWidthAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullHalfWidthAngle() : 0;


}

float ARadialActorBase::GetMainHullCenterAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullCenterAngle() : 0;


}


//Protected----------------------

void ARadialActorBase::SetupHullVisualization()
{	
	ResetHullVisualization();

	auto &HullList{ m_PolarCollision.GetTempHullReference() };

	auto ScaleBase{ (m_ActorDepthInCells > m_ActorWidthInCells ? m_ActorDepthInCells : m_ActorWidthInCells) * 2 };
	FLinearColor CenterAsColor{ static_cast<float>(m_PolarCollision.GetCartesianCenter().X), static_cast<float>(m_PolarCollision.GetCartesianCenter().Y), 0, 0 };

	
	UStaticMesh *pVisMesh{ nullptr };
	UMaterialInterface *pMaterial{ nullptr };
	if (auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) })
	{
		pVisMesh = pGI->GetGlobalData().pPlaneMesh;
		pMaterial = pGI->GetGlobalData().pMaterialHullVisDefault;

		if(!pVisMesh || !pMaterial)
		{
			UE_LOG(RTS_StructurePlacement, Error, TEXT("Could not get plane mesh or material for hull visualization"));
			return;			
		}
	}

	for(auto &&Hull : HullList)
	{
		auto *Comp{ NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass()) };
		if(!Comp)
		{
			UE_LOG(RTS_StructurePlacement, Error, TEXT("Could not create static mesh component for hull visualization in actor %s. Aborting"), *GetName());
			return;
		}

		Comp->SetupAttachment(GetRootComponent());
		Comp->RegisterComponent();
		Comp->SetStaticMesh(pVisMesh);
		Comp->SetWorldScale3D({ static_cast<float>(ScaleBase) * 3, static_cast<float>(ScaleBase * 3), 1 });
		Comp->SetRelativeLocation({ 0,0,10 });
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_apHullVisPlanes.Add(Comp);


		auto *pDynMat{ Comp->CreateDynamicMaterialInstance(0, pMaterial) };

		pDynMat->SetVectorParameterValue( TEXT("CartesianCenter"), CenterAsColor );
		pDynMat->SetScalarParameterValue( TEXT("MinRadius"), static_cast<float>(Hull.GetRadiusMin()) );
		pDynMat->SetScalarParameterValue( TEXT("Depth"), static_cast<float>(Hull.GetDepth()) );
		pDynMat->SetScalarParameterValue( TEXT("HalfWidthAngle"), static_cast<float>(Hull.GetHalfAngle()) );
		pDynMat->SetScalarParameterValue( TEXT("CenterAngle"), static_cast<float>(Hull.GetCenterAngle()) );
			   
	}


}

void ARadialActorBase::CenterCenteredRoot()
{
	const auto HullDepth{ m_pOwningFactory->GetCellDepth() * GetDepthInCells() };
	m_pCenteredRoot->SetRelativeLocation({ static_cast<float>(-HullDepth) * .5f, 0, 0 });


}

void ARadialActorBase::OnCollisionInitialized()
{
	CenterCenteredRoot();


}

void ARadialActorBase::ResetHullVisualization()
{
	if(m_apHullVisPlanes.Num() <= 0)
	{
		return;
	}

	for(auto &&pHullPlane : m_apHullVisPlanes)
	{
		pHullPlane->DestroyComponent();
	}
	m_apHullVisPlanes.Empty();
	

}

