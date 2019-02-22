// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSideStructureFactory.h"
#include "Limes.h"
#include "RTSGameInstance.h"




void ARTSSideStructureFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto *pGInst{ Cast<URTSGameInstance>(GetGameInstance()) };
	ARTSStructureFactory *pMainFactory{ nullptr };
	if (pGInst)
	{
		pMainFactory = pGInst->GetMainStructureFactory();
	}

	if (!pMainFactory)
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("Side factory could not acquire a main factory reference"));
		return;
	}

	//m_SpaceDiscretizer = SpaceDiscretizer{ GetActorLocation(), pMainFactory->GetCellArcWidth(), pMainFactory->GetCellDepth(), }


}
