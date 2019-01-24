// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingFactory.h"
#include "Limes.h"
#include "BuildingPreview.h"
#include "Engine/World.h"
#include "RTSGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"
#include "RTSPlayerEye.h"

constexpr auto ECCPlacable = ECC_GameTraceChannel1;
constexpr auto ECCNonPlacable = ECC_GameTraceChannel2;

ABuildingFactory::ABuildingFactory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABuildingFactory::CommitPreviewBuilding(const ABuildingPreview *pPreviewBuilding) const
{
	bool bCommitSuccessful{ true };
	//todo: game instance updates (gameplay)

	if (auto *pClass = pPreviewBuilding->GetPreviewedClass())
	{
		if (pPreviewBuilding->IsA<ARadialActorBase>())
		{
			auto Transform = pPreviewBuilding->GetActorTransform();
			auto *pActor = GetWorld()->SpawnActor(pClass, &Transform);

			auto *pRadialActor = Cast<ARadialActorBase>(pActor);
			
			if (IsPlacableAtPosition(pRadialActor))
			{				
				UE_LOG(RTS_StructureFactory, Log, TEXT("Commiting preview building %s"), *pPreviewBuilding->GetName());
				AddCollisionComponents(pRadialActor);
				pActor->DrawDebugComponents();
				return;
			}		
			pActor->Destroy();

		}

	}	
	UE_LOG(RTS_StructureFactory, Error, TEXT("Could not commit preview building"));
		   

}

FVector ABuildingFactory::Discretize(const FVector &ToConvert) const
{
	return m_SpaceDiscretizer.Discretize(ToConvert);

}

bool ABuildingFactory::IsPlacableAtPosition(ARadialActorBase *pActor) const
{
	auto SymmetryOffset = FMath::FloorToInt(pActor->GetCellWidth() * .5);
	bool bBlockingHit{ false };

	for (uint32 DepthOffset{ 0 }; DepthOffset < pActor->GetCellDepth(); ++DepthOffset)
	{
		auto LowBorder = m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), -SymmetryOffset, DepthOffset);
		auto HighBorder = m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), SymmetryOffset, DepthOffset);

		bBlockingHit |= GetWorld()->LineTraceTestByChannel(LowBorder, HighBorder, ECCNonPlacable);


	}

	return !bBlockingHit;


}

void ABuildingFactory::InstantiatePreviewBuilding(EBuildingTypes Type) const
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
		pPlayer->NotifyNewBuildingPreview(pNewPreview);

	}
	else
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("Unable to notify rts player"));
		pNewPreview->Destroy();

	}

	
}


//Protected--------------------

void ABuildingFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_SpaceDiscretizer = SpaceDiscretizer{ GetActorLocation(), 600, 30, 3 };

}

void ABuildingFactory::AddCollisionComponents(ARadialActorBase *pActor) const
{
	auto SymmetryOffset = FMath::FloorToInt(pActor->GetCellWidth() * .5);

	for (uint32 CellOffset{ 0 }; CellOffset < pActor->GetCellWidth(); ++CellOffset)
	{
		for (uint32 DepthOffset{ 0 }; DepthOffset < pActor->GetCellDepth(); ++DepthOffset)
		{
			auto CellCenterPos{ m_SpaceDiscretizer.Discretize(pActor->GetActorLocation(), CellOffset - SymmetryOffset, DepthOffset) };
			
			auto *pComp = NewObject<UBoxComponent>(pActor);
			pComp->RegisterComponent();

			pComp->SetBoxExtent({ m_SpaceDiscretizer.GetCellDepth() * .4f, 32, 32 });
			
			auto QuatToFactory = (GetActorLocation() - CellCenterPos).GetSafeNormal2D().ToOrientationQuat();
			pComp->SetWorldLocationAndRotation(CellCenterPos, QuatToFactory);
			pComp->AttachToComponent(pActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

			pComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

			//CellCenterPos = m_SpaceDiscretizer.Discretize(Pos, CellOffset - SymmetryOffset, DepthOffset, true);


		}

	}


}
