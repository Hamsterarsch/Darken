// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSGameMode.h"
#include "Resources/ResourceSystem.h"


ARTSGameMode::ARTSGameMode()
{
	m_pResourceSystem = CreateDefaultSubobject<UResourceSystem>(TEXT("ResourceSystem"));


}

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_pResourceSystem->AddCitizens(6, ECitizenType::Commoner);
	m_pResourceSystem->AddCitizens(4, ECitizenType::Child);


}
