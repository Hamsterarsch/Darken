// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingWidgetBase.h"


void UBuildingWidgetBase::UpdateBuildingExtension(EBuildingWidgetExtension ExtensionType)
{
	m_CurrentWidgetExtension = ExtensionType;
	OnUpdateBuildingExtension(ExtensionType);


}
