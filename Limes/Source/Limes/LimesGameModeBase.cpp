// Fill out your copyright notice in the Description page of Project Settings.

#include "LimesGameModeBase.h"
#include "Engine/World.h"
#include "BuildingFactory.h"
#include "RTSGameInstance.h"
#include "Limes.h"

void ALimesGameModeBase::PreInitializeComponents() 
{
	Super::PreInitializeComponents();
	
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("GM, world does not live"));
	}

	if (GetWorld() && !m_StructureFactoryClass.IsNull())
	{
		if (auto *pClass = SafeLoadClassPtr(m_StructureFactoryClass))
		{
			auto *pMainFactory = GetWorld()->SpawnActor<class ABuildingFactory>(pClass);

			UE_LOG(RTS_StructureFactory, Warning, TEXT("Play structure factory"));

			if (auto *pGameInstance = Cast<URTSGameInstance>(GetGameInstance()))
			{
				pGameInstance->SetMainStructureFactory(pMainFactory);
				return;

			}


		}		
		
	}
	UE_LOG(RTS_StructureFactory, Fatal, TEXT("Could not setup main structure factory"));


}


