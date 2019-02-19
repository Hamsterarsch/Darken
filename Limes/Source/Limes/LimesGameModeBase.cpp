// Fill out your copyright notice in the Description page of Project Settings.

#include "LimesGameModeBase.h"
#include "Engine/World.h"
#include "RTSStructureFactory.h"
#include "RTSGameInstance.h"
#include "Limes.h"

void ALimesGameModeBase::PreInitializeComponents() 
{
	Super::PreInitializeComponents();
	
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("GM, world does not live"));
	}

	bool bWithEditor{ false };
#if WITH_EDITOR
	bWithEditor = true;
#endif

	if (GetWorld() && !m_StructureFactoryClass.IsNull())
	{
		if (auto *pClass = SafeLoadClassPtr(m_StructureFactoryClass))
		{
			auto *pMainFactory = GetWorld()->SpawnActor<class ARTSStructureFactory>(pClass);

			UE_LOG(RTS_StructureFactory, Warning, TEXT("Play structure factory"));

			if (auto *pGameInstance = Cast<URTSGameInstance>(GetGameInstance()))
			{
				pGameInstance->SetMainStructureFactory(pMainFactory);
				return;

			}


		}		
		
	}

	if (m_StructureFactoryClass.IsNull())
	{
		if (!bWithEditor)
		{
			UE_LOG(RTS_StructureFactory, Fatal, TEXT("Could not setup main structure factory"));
		}

	}


}


