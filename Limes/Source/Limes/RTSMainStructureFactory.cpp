// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSMainStructureFactory.h"
#include "RTSGameInstance.h"



ARTSMainStructureFactory::ARTSMainStructureFactory() :
	m_MinRadius{ 600 },
	m_CellDepthMultiplier{ 3 }
{
}

void ARTSMainStructureFactory::BeginPlay()
{
	Super::BeginPlay();

	auto *pGInst{ Cast<URTSGameInstance>(GetGameInstance()) };
	if (pGInst)
	{
		pGInst->SetMainStructureFactory(this);


	}


}

//Protected--------------------

void ARTSMainStructureFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_SpaceDiscretizer = SpaceDiscretizer{ GetActorLocation(), m_MinRadius, m_InnermostCellcount, m_CellDepthMultiplier };
	SetupGridVisualization();


}
