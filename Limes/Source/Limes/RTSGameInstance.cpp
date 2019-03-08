// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSGameInstance.h"
#include "Limes.h"
#include "RTSMainStructureFactory.h"
#include "Paths.h"

const FRTSGlobalData& URTSGameInstance::GetGlobalData() const noexcept
{
	return m_GlobalData;


}

void URTSGameInstance::SetSelectedStructureFactory(class ARTSStructureFactory *pNewFactory)
{	
	if (m_pSelectedStructureFactory)
	{
		m_pSelectedStructureFactory->HideBuildingPlacementGrid();
	}

	if (!pNewFactory)
	{
		m_pSelectedStructureFactory = m_pMainStructureFactory;
		UE_LOG(RTS_GameInstance, Log, TEXT("Selected structure factory is the main factory"));

	}
	UE_LOG(RTS_GameInstance, Log, TEXT("Selected structure factory is now %s"), *pNewFactory->GetName());
	m_pSelectedStructureFactory = pNewFactory;

	m_pSelectedStructureFactory->ShowBuildingPlacementGrid();


}

void URTSGameInstance::SetMainStructureFactory(ARTSMainStructureFactory *pMainStructureFactory)
{
	if (!m_pMainStructureFactory)
	{
		m_pMainStructureFactory = pMainStructureFactory;
		SetSelectedStructureFactory(m_pMainStructureFactory);
	}


}

void URTSGameInstance::Init()
{
	Super::Init();

	m_GlobalData.pPlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (!m_GlobalData.pPlaneMesh)
	{
		UE_LOG(RTS_GameInstance, Error, TEXT("Could not load plane mesh from path"));
	}
	

}

