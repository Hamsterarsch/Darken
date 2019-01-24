// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingPreview.h"
#include "Limes.h"
#include "BuildingBase.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"


//Public--------------------

ABuildingPreview::ABuildingPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	Cast<USceneComponent>(m_pStaticMesh)->SetupAttachment(GetRootComponent());


}

void ABuildingPreview::NotifyNonPlacable()
{
	if(auto *pAsset = m_MaterialNonPlacable.Get())
	{
		m_pStaticMesh->SetMaterial(0, pAsset);
		UE_LOG(RTS_StructurePlacement, Verbose, TEXT("ABuildingPreview:: set 'non-placable' material on preview mesh"));

	}
	else
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not get 'non-placable' material"));
	}


}

void ABuildingPreview::NotifyPlacable()
{
	if (auto *pAsset = m_MaterialPlacable.Get())
	{
		m_pStaticMesh->SetMaterial(0, pAsset);
		UE_LOG(RTS_StructurePlacement, Verbose, TEXT("ABuildingPreview:: set 'placable' material on preview mesh"));

	}
	else
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not get 'placable' material"));
	}


}


//Protected-------------

void ABuildingPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingPreview::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (auto *pMaterial = SafeLoadObjectPtr(m_MaterialPlacable))
	{
		//todo: check: for each instantiation the pMaterial will refer to the same object,
		//but does this prevent multiple addition to the root set ?
		pMaterial->AddToRoot();
	}
	else
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not add 'placable' material to root set."));
	}	

	if (auto *pMaterial = SafeLoadObjectPtr(m_MaterialNonPlacable))
	{
		pMaterial->AddToRoot();		
	}
	else
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ABuildingPreview:: could not add 'non-placable' material to root set."));
	}


}

UClass *ABuildingPreview::GetPreviewedClass() const
{
	return SafeLoadClassPtr(m_PreviewedClass);

}

