// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSideStructureFactory.h"
#include "Limes.h"
#include "RTSGameInstance.h"
#include "RTSMainStructureFactory.h"




ARTSSideStructureFactory::ARTSSideStructureFactory() 
{
}

void ARTSSideStructureFactory::BeginPlay()
{
	Super::BeginPlay();


}

void ARTSSideStructureFactory::OnCollisionInitialized()
{
	Super::OnCollisionInitialized();

	auto *pGInst{ Cast<URTSGameInstance>(GetGameInstance()) };
	ARTSMainStructureFactory *pMainFactory{ nullptr };
	if (pGInst)
	{
		pMainFactory = pGInst->GetMainStructureFactory();
	}

	if (!pMainFactory)
	{
		UE_LOG(RTS_StructureFactory, Error, TEXT("Side factory could not acquire a main factory reference"));
		return;
	}

	m_SpaceDiscretizer = SpaceDiscretizer{ m_pCenteredRoot->GetComponentLocation(), pMainFactory->GetCellWidthArc(), pMainFactory->GetCellDepth(), m_MaxRingCount, m_InnermostCellcount };
	SetupGridVisualization();


}
