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

ARTSStructureFactory::ARTSStructureFactory() :
	m_InnermostCellcount{ 30 }
{
	PrimaryActorTick.bCanEverTick = true;

	m_pVisualizerPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualizerPlane"));
	m_pVisualizerPlane->SetupAttachment(GetRootComponent());
	m_pVisualizerPlane->SetRelativeLocation({ 0, 0, 5 });
	m_pVisualizerPlane->SetVisibility(false);
	m_pVisualizerPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

}


void ARTSStructureFactory::InstantiatePreviewBuilding(const TSoftClassPtr<class ABuildingBase> &TypeToPreview)
{
	UE_LOG(RTS_StructureFactory, Log, TEXT("Instantiating preview building: %s"), *TypeToPreview.GetAssetName());
	
	auto *pNewPreview{ ABuildingPreview::SpawnNewBuildingPreview(GetWorld(), TypeToPreview) };
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

bool ARTSStructureFactory::TryCommitPreviewBuilding(ABuildingPreview *pPreviewBuilding)
{
	//todo: game instance updates (gameplay)

	if (auto *pClass = pPreviewBuilding->GetPreviewedClass())
	{
		auto Transform = pPreviewBuilding->GetActorTransform();
		auto *pActor = GetWorld()->SpawnActor(pClass, &Transform);

		auto *pBuildingActor = Cast<ARadialActorBase>(pActor);
		
		pPreviewBuilding->InitMinimumCollision(this);
		if (HasIntersectionsWithChildBuildings(pPreviewBuilding))
		{
			pActor->Destroy();
			return false;
		}
		else
		{
			UE_LOG(RTS_StructureFactory, Log, TEXT("Commiting preview building %s"), *pPreviewBuilding->GetName());
			//AddCollisionComponents(pRadialActor);
			//pActor->DrawDebugComponents();
			pBuildingActor->InitExpandedCollision(this);
			AddChildBuilding(pBuildingActor);
			pPreviewBuilding->Destroy();
			return true;


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

void ARTSStructureFactory::AddChildBuilding(ARadialActorBase *pNewChild)
{
	m_apChildBuildings.AddUnique(pNewChild);


}

void ARTSStructureFactory::RemoveChildBuilding(ARadialActorBase *pChildToRemove)
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

bool ARTSStructureFactory::HasIntersectionsWithChildBuildings(const PolarMath::CPolarCollider &HullToTest, PolarMath::CPolarCollider *out_pFirstHit) const
{
	for(auto &&ChildActor : m_apChildBuildings)
	{
		if(ChildActor->HasIntersectionsWith(HullToTest, out_pFirstHit))
		{
			return true;
		}

	}
	return false;


}

void ARTSStructureFactory::ShowBuildingPlacementGrid()
{
	m_pVisualizerPlane->SetVisibility(true);


}

void ARTSStructureFactory::HideBuildingPlacementGrid()
{
	m_pVisualizerPlane->SetVisibility(false);


}

double ARTSStructureFactory::GetCellWidthAngle(double LowRangeRadius) const
{
	return m_SpaceDiscretizer.GetCellWidthAngle(LowRangeRadius);


}


//Protected--------------------

void ARTSStructureFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnClicked.AddDynamic(this, &ARTSStructureFactory::OnActorClickedEvent);


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

void ARTSStructureFactory::SetupGridVisualization()
{
	auto *pMaterial = m_pVisualizerPlane->GetMaterial(0);
	if (!pMaterial)
	{
		return;
	}

	auto *pDynMat = m_pVisualizerPlane->CreateDynamicMaterialInstance (0, pMaterial);
	
	pDynMat->SetScalarParameterValue(TEXT("CellDepth"), static_cast<float>(m_SpaceDiscretizer.GetCellDepth()));
	pDynMat->SetScalarParameterValue(TEXT("InnerCellCount"), m_SpaceDiscretizer.GetInnerCellCount());
	pDynMat->SetScalarParameterValue(TEXT("MinRadius"), m_SpaceDiscretizer.GetInnermostRadius());
	
	pDynMat->SetScalarParameterValue(TEXT("MaxRingCount"), m_SpaceDiscretizer.GetMaxRingCount() < 30 ? 30 : m_SpaceDiscretizer.GetMaxRingCount());


	FLinearColor CenterPos{ m_SpaceDiscretizer.GetCartesianOrigin().X, m_SpaceDiscretizer.GetCartesianOrigin().Y, 0, 0 };
	pDynMat->SetVectorParameterValue(TEXT("CenterPosWs"), CenterPos);
	
	float PlaneScale;
	if (m_SpaceDiscretizer.GetMaxRingCount() < 0)
	{
		PlaneScale = 1000;
	}
	else
	{
		PlaneScale = FMath::CeilToInt(m_SpaceDiscretizer.GetOutmostRadius() / 50);
	}
	m_pVisualizerPlane->SetWorldScale3D({ PlaneScale, PlaneScale, 1 });
	m_pVisualizerPlane->SetRelativeLocation({ 0, 0, 5 });


}

//Private-----------------------

void ARTSStructureFactory::OnActorClickedEvent(AActor *pTouchedActor, FKey ButtonPressed)
{
	if (auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) })
	{
		pGI->SetSelectedStructureFactory(this);
		

	}


}
