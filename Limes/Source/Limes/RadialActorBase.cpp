// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialActorBase.h"
#include "BuildingFactory.h"

//Public------------

FVector ARadialActorBase::GetRadialOrigin() const
{
	return m_pSelectedStructureFactory->GetActorLocation();

}

void ARadialActorBase::SetParentFactory(ABuildingFactory *pNewFactory)
{
	m_pSelectedStructureFactory = pNewFactory;

}