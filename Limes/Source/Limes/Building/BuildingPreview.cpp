// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingPreview.h"
#include "Misc/Limes.h"
#include "BuildingBase.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Building/Beacon/RTSStructureFactory.h"
#include "Misc/RTSGameInstance.h"
#include "Kismet/GameplayStatics.h"


//Public--------------------

ABuildingPreview* ABuildingPreview::SpawnNewBuildingPreview(UWorld *pWorld, const TSoftClassPtr<ABuildingBase> &PreviewedBuilding)
{
	auto *pPreviewClass{ SafeLoadClassPtr(PreviewedBuilding) };
	if( !pPreviewClass )
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not spawn building preview because the class ptr could not be loaded"));
		return nullptr;
	}

	auto *pDefaultObject{ Cast<ABuildingBase>(pPreviewClass->ClassDefaultObject) };
	if(!pDefaultObject)
	{
		return nullptr;
	}

	auto *pMeshComp{ pDefaultObject->GetBuildingMeshComp() };
	if(!pMeshComp)
	{
		return nullptr;
	}
		
	auto *pOut{ pWorld->SpawnActorDeferred<ABuildingPreview>(StaticClass(), FTransform::Identity) };
	pOut->m_pBuildingMeshComp->SetStaticMesh(pMeshComp->GetStaticMesh());
	pOut->m_pBuildingMeshComp->SetRelativeTransform(pMeshComp->GetRelativeTransform());
	pOut->m_pPreviewedClass = pPreviewClass;
	pOut->m_ActorWidthInCells = pDefaultObject->GetWidthInCells();
	pOut->m_ActorDepthInCells = pDefaultObject->GetDepthInCells();
	UGameplayStatics::FinishSpawningActor(pOut, FTransform::Identity);
	   
	return pOut;


}

ABuildingPreview::ABuildingPreview() :
	m_pMaterialPlacable{ nullptr },
	m_pMaterialNonPlacable{ nullptr },
	m_pPreviewedClass{ nullptr }
{
	PrimaryActorTick.bCanEverTick = false;


}

void ABuildingPreview::NotifyNonPlacable()
{
	if(!m_pMaterialNonPlacable)
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not get 'non-placable' material"));
		return;
	}
	
	m_pBuildingMeshComp->SetMaterial(0, m_pMaterialNonPlacable);
	UE_LOG(RTS_StructurePlacement, Verbose, TEXT("ABuildingPreview:: set 'non-placable' material on preview mesh"));


}

void ABuildingPreview::NotifyPlacable()
{
	if (!m_pMaterialPlacable)
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not get 'placable' material"));
		return;
	}

	m_pBuildingMeshComp->SetMaterial(0, m_pMaterialPlacable);
	UE_LOG(RTS_StructurePlacement, Verbose, TEXT("ABuildingPreview:: set 'placable' material on preview mesh"));


}

void ABuildingPreview::ResetPolarCollision()
{
	m_bIsCollisionInitialized = false;

	for (auto &&pComp : m_apHullVisPlanes)
	{
		if (!pComp)
		{
			continue;
		}

		pComp->DestroyComponent();

	}
	m_apHullVisPlanes.Empty();


}


//Protected-------------

void ABuildingPreview::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) };
	if (!pGI)
	{	
		return;
	}

	m_pMaterialNonPlacable = pGI->GetGlobalData().pMaterialBuildingNonPlaceable;
	m_pMaterialPlacable = pGI->GetGlobalData().pMaterialBuildingPlaceable;


}

UClass *ABuildingPreview::GetPreviewedClass() const
{
	return m_pPreviewedClass;


}

