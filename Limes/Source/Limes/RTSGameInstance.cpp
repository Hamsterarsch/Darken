// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSGameInstance.h"
#include "BuildingFactory.h"

void URTSGameInstance::SetSelectedStructureFactory(class ABuildingFactory *pNewFactory)
{
	m_pSelectedStructureFactory = pNewFactory;


}

void URTSGameInstance::SetMainStructureFactory(class ABuildingFactory *pMainStructureFactory)
{
	m_pMainStructureFactory = pMainStructureFactory;


}

