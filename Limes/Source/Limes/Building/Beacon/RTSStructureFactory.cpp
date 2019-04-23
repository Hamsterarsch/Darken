// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSStructureFactory.h"
#include "Misc/Limes.h"
#include "Building/BuildingPreview.h"
#include "Engine/World.h"
#include "Misc/RTSGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"
#include "Building/BuildingBase.h"
#include "Player/RTSPlayerEye.h"
#include "RTSSideBeacon.h"

constexpr auto ECCPlacable = ECC_GameTraceChannel1;
constexpr auto ECCNonPlacable = ECC_GameTraceChannel2;

ARTSStructureFactory::ARTSStructureFactory() :
	m_InnermostCellcount{ 30 },
	m_IlluminatedRingsCount{ 9 }
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
	
	auto *pNewPreview{ ABuildingPreview::SpawnNewBuildingPreview(GetWorld(), TypeToPreview, this) };
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
		UE_LOG(RTS_StructureFactory, Error, TEXT("Unable to notify rts player about new preview building"));
		pNewPreview->Destroy();

	}


}

bool ARTSStructureFactory::TryCommitPreviewBuilding(ABuildingPreview *pPreviewBuilding)
{
	//todo: game instance updates (gameplay)

	if (auto *pClass = pPreviewBuilding->GetPreviewedClass())
	{		
		if (HasIntersectionsWithChildBuildings(pPreviewBuilding))
		{
			pPreviewBuilding->RefreshPolarCollision();
			return false;
		}
		else
		{
			if(!IsBuildingIlluminated(pPreviewBuilding))
			{
				pPreviewBuilding->RefreshPolarCollision();
				return false;
			}

			auto Transform{ pPreviewBuilding->GetActorTransform() };
			auto *pActor{ GetWorld()->SpawnActor(pClass, &Transform) };
			auto *pBuildingActor{ Cast<ABuildingBase>(pActor) };

			UE_LOG(RTS_StructureFactory, Log, TEXT("Commiting preview building %s"), *pPreviewBuilding->GetName());

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

void ARTSStructureFactory::AddChildBuilding(ABuildingBase *pNewChild)
{
	m_apChildBuildings.AddUnique(pNewChild);
		
	if (auto *pAsBeacon{ Cast<ARTSSideBeacon>(pNewChild) })
	{
		m_apSideBeacons.AddUnique(pAsBeacon);
		for(auto *pBuilding : m_apChildBuildings)
		{
			pBuilding->RefreshIlluminationState();

		}
	}


}

void ARTSStructureFactory::RemoveChildBuilding(ABuildingBase *pChildToRemove)
{
	m_apChildBuildings.Remove(pChildToRemove);

	if (auto *pAsBeacon{ Cast<ARTSSideBeacon>(pChildToRemove) })
	{
		m_apSideBeacons.Remove(pAsBeacon);
		for (auto *pBuilding : m_apChildBuildings)
		{
			pBuilding->RefreshIlluminationState();
		
		}
	}


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

float ARTSStructureFactory::GetIlluminationRadius() const
{
	return static_cast<float>(m_SpaceDiscretizer.GetCellDepth() * m_IlluminatedRingsCount + GetInnermostRingRadius());

}

bool ARTSStructureFactory::IsBuildingIlluminated(const ABuildingBase* pBuilding) const
{
	if(!pBuilding)
	{
		return false;
	}

	auto BuildingCenterPos{ pBuilding->GetCenteredRoot()->GetComponentLocation() };
	auto DistToThis{ FVector::Dist2D(BuildingCenterPos, GetCenteredRootLocation()) };

	bool bIsIlluminated{ DistToThis <= GetIlluminationRadius() };
	for(const auto *pBeacon : m_apSideBeacons)
	{
		if(!pBeacon)
		{
			continue;
		}
	 	bIsIlluminated |= FVector::Dist2D(BuildingCenterPos, pBeacon->GetCenteredRoot()->GetComponentLocation()) <= pBeacon->GetIlluminationRadius();
		
	}
	return bIsIlluminated;


}

void ARTSStructureFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnClicked.AddDynamic(this, &ARTSStructureFactory::OnActorClickedEvent);


}

/*
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


}*/

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
	m_pVisualizerPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

//Private-----------------------

void ARTSStructureFactory::OnActorClickedEvent(AActor *pTouchedActor, FKey ButtonPressed)
{
	if (auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) })
	{
		pGI->SetSelectedStructureFactory(this);
		

	}


}
