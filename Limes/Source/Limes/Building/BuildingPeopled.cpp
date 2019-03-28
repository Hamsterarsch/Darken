// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingPeopled.h"
#include "Engine/World.h"
#include "Misc/RTSGameMode.h"
#include "Resources/ResourceSystem.h"

void ABuildingPeopled::BeginPlay()
{
	Super::BeginPlay();

	if(auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		m_OnCitizenRemovedHandle.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(ABuildingPeopled, OnCitizenRemoved));
		pGm->GetResourceSystem()->m_eOnCitizenRemoved.Add(m_OnCitizenRemovedHandle);
	}


}

void ABuildingPeopled::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		pGm->GetResourceSystem()->m_eOnCitizenRemoved.Remove(m_OnCitizenRemovedHandle);			
	}


}

void ABuildingPeopled::OnCitizenRemoved(int32 CitizenId)
{
	m_sContainedCitizens.Remove(CitizenId);


}
