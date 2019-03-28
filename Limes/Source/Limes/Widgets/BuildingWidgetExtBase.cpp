// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingWidgetExtBase.h"


void UBuildingWidgetExtBase::SetTargetBuilding(ABuildingBase* pNewTarget)
{
	m_pTargetBuilding = pNewTarget;
	OnTargetBuildingUpdated();


}
