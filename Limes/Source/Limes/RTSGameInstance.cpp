// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSGameInstance.h"
#include "RTSStructureFactory.h"

void URTSGameInstance::SetSelectedStructureFactory(class ARTSStructureFactory *pNewFactory)
{
	m_pSelectedStructureFactory = pNewFactory;


}

void URTSGameInstance::SetMainStructureFactory(class ARTSStructureFactory *pMainStructureFactory)
{
	m_pMainStructureFactory = pMainStructureFactory;


}

