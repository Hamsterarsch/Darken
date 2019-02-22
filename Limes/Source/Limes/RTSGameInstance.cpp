// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSGameInstance.h"
#include "Limes.h"
#include "RTSStructureFactory.h"

void URTSGameInstance::SetSelectedStructureFactory(class ARTSStructureFactory *pNewFactory)
{
	if (!pNewFactory)
	{
		m_pSelectedStructureFactory = m_pMainStructureFactory;
		UE_LOG(RTS_GameInstance, Log, TEXT("Selected structure factory is the main factory"));

	}
	UE_LOG(RTS_GameInstance, Log, TEXT("Selected structure factory is now %s"), *pNewFactory->GetName());
	m_pSelectedStructureFactory = pNewFactory;


}

void URTSGameInstance::SetMainStructureFactory(class ARTSStructureFactory *pMainStructureFactory)
{
	if (!m_pMainStructureFactory)
	{
		m_pMainStructureFactory = pMainStructureFactory;
		SetSelectedStructureFactory(m_pMainStructureFactory);
	}


}

