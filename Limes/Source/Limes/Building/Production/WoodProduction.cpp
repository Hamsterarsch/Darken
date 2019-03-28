// Fill out your copyright notice in the Description page of Project Settings.

#include "WoodProduction.h"
#include "Misc/Daytime.h"

#include "Engine/World.h"
#include "Misc/RTSGameMode.h"
#include "Resources/ResourceSystem.h"

AWoodProduction::AWoodProduction() :
	m_WoodPerHour{ 10 }
{
	m_GeneratedResource = EBuildingResourceTypes::Wood;


}

float AWoodProduction::GetGeneratedAmount(const float ForSeconds) const
{
	const auto HourFraction{ ForSeconds / FDaytime::s_HourInSeconds};
	
	return HourFraction * m_WoodPerHour * m_ProductivityCoeff;


}

void AWoodProduction::BeginPlay()
{
	Super::BeginPlay();

	if(auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())})
	{
		auto *pRescSys{ pGm->GetResourceSystem() };
		pRescSys->AttachGenerator(this);

	}


}

void AWoodProduction::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		pGm->GetResourceSystem()->DetachGenerator(this);

	}

}
