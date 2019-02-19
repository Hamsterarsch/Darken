// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSStructureFactory.h"
#include "Limes.h"
#include "BuildingPreview.h"
#include "Engine/World.h"
#include "RTSGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"
#include "BuildingBase.h"
#include "RTSPlayerEye.h"

constexpr auto ECCPlacable = ECC_GameTraceChannel1;
constexpr auto ECCNonPlacable = ECC_GameTraceChannel2;

ARTSStructureFactory::ARTSStructureFactory()
{
	PrimaryActorTick.bCanEverTick = true;

}

bool ARTSStructureFactory::TryCommitPreviewBuilding(ABuildingPreview *pPreviewBuilding)
{
	//todo: game instance updates (gameplay)

	if (auto *pClass = pPreviewBuilding->GetPreviewedClass())
	{
		if (pClass->IsChildOf<ABuildingBase>())
		{
			auto Transform = pPreviewBuilding->GetActorTransform();
			auto *pActor = GetWorld()->SpawnActor(pClass, &Transform);

			auto *pBuildingActor = Cast<ABuildingBase>(pActor);
			pBuildingActor->InitializePolarCollision(this);
			
			if (HasIntersectionsWithChildBuildings(pBuildingActor))
			{
				pActor->Destroy();
				return false;
			}
			else
			{
				UE_LOG(RTS_StructureFactory, Log, TEXT("Commiting preview building %s"), *pPreviewBuilding->GetName());
				//AddCollisionComponents(pRadialActor);
				//pActor->DrawDebugComponents();
				AddChildBuilding(pBuildingActor);
				pPreviewBuilding->Destroy();
				return true;
					

			}

		}

	}	
	UE_LOG(RTS_StructureFactory, Error, TEXT("Could not commit preview building"));
	return false;


}

FVector ARTSStructureFactory::Discretize(const FVector &ToConvert) const
{
	return m_SpaceDiscretizer.Discretize(ToConvert);

}

bool ARTSStructureFactory::IsPlacableAtPosition(ARadialActorBase *pActor) const
{
	auto SymmetryOffset = FMath::FloorToInt(pActor->GetWidthInCells() * .5);
	bool bBlockingHit{ false };

	for (uint32 DepthOffset{ 0 }; DepthOffset < pActor->GetDepthInCells(); ++DepthOffset)
	{
		auto LowBorder = m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), -SymmetryOffset, DepthOffset);
		auto HighBorder = m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), SymmetryOffset, DepthOffset);

		bBlockingHit |= GetWorld()->LineTraceTestByChannel(LowBorder, HighBorder, ECCNonPlacable);


	}

	return !bBlockingHit;


}

void ARTSStructureFactory::InstantiatePreviewBuilding(EBuildingTypes Type)
{	
	if (Type == EBuildingTypes::None)
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("InstantiatePreviewBuilding:: missing building type"));

	}

	UE_LOG(RTS_StructureFactory, Log, TEXT("Instantiating preview building: %i"), (uint8)Type);
	ABuildingPreview *pNewPreview{ nullptr };
	switch (Type)
	{
	case EBuildingTypes::SimpleHome:
		if (auto pClass = SafeLoadClassPtr(m_SimpleHomePreviewClass))
		{
			pNewPreview = GetWorld()->SpawnActor<ABuildingPreview>(pClass);

		}
		else
		{
			UE_LOG(RTS_StructureFactory, Error, TEXT("Unable to load simple home instantiate preview building class"));

		}
		break;

	}

	if (!pNewPreview)
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("Unable to instantiate preview building"));
		return;

	}

	if (auto *pPlayer = Cast<ARTSPlayerEye>(GetGameInstance()->GetFirstLocalPlayerController(GetWorld())->GetPawn()))
	{
		pPlayer->NotifyNewBuildingPreview(pNewPreview, this);

	}
	else
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("Unable to notify rts player"));
		pNewPreview->Destroy();

	}

	
}


PolarMath::CPolarTransform ARTSStructureFactory::GetPolarTransform() const
{
	return m_PolarTransform;

}

double ARTSStructureFactory::GetCellWidthAngle(double LowRangeRadius) const
{
	return m_SpaceDiscretizer.GetCellWidthAngle(LowRangeRadius);


}

double ARTSStructureFactory::GetCellDepth() const
{
	return m_SpaceDiscretizer.GetCellDepth();


}

void ARTSStructureFactory::AddChildBuilding(ABuildingBase *pNewChild)
{
	m_apChildBuildings.AddUnique(pNewChild);


}

void ARTSStructureFactory::RemoveChildBuilding(ABuildingBase *pChildToRemove)
{
	m_apChildBuildings.Remove(pChildToRemove);


}

bool ARTSStructureFactory::HasIntersectionsWithChildBuildings(class ARadialActorBase *pBuildingToTest) const
{
	for (auto &&ChildBuilding : m_apChildBuildings)
	{
		if (ChildBuilding->HasIntersectionsWith(pBuildingToTest))
		{
			return true;
		}
	
	
	}
	return false;


}

//Protected--------------------

void ARTSStructureFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARTSStructureFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto ActorPosWs{ GetActorLocation() };
	m_SpaceDiscretizer = SpaceDiscretizer{ ActorPosWs, 600, 30, 3 };
	m_PolarTransform = PolarMath::CPolarTransform{ ActorPosWs.X, ActorPosWs.Y };


}

void ARTSStructureFactory::AddCollisionComponents(ARadialActorBase *pActor) const
{
	auto SymmetryOffset = FMath::FloorToInt(pActor->GetWidthInCells() * .5);

	for (uint32 CellOffset{ 0 }; CellOffset < pActor->GetWidthInCells(); ++CellOffset)
	{
		for (uint32 DepthOffset{ 0 }; DepthOffset < pActor->GetDepthInCells(); ++DepthOffset)
		{
			auto CellCenterPos{ m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), CellOffset - SymmetryOffset, DepthOffset) };
			
			auto *pComp = NewObject<UBoxComponent>(pActor);
			pComp->RegisterComponent();

			pComp->SetBoxExtent({ static_cast<float>(m_SpaceDiscretizer.GetCellDepth()) * .4f, 32, 32 });
			
			auto QuatToFactory = (GetActorLocation() - CellCenterPos).GetSafeNormal2D().ToOrientationQuat();
			pComp->SetWorldLocationAndRotation(CellCenterPos, QuatToFactory);
			pComp->AttachToComponent(pActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

			pComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

			//CellCenterPos = m_SpaceDiscretizer.Discretize(Pos, CellOffset - SymmetryOffset, DepthOffset, true);


		}

	}


}
