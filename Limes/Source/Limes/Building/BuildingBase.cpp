// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingBase.h"
#include "Components/StaticMeshComponent.h"
#include "Building/Beacon/RTSStructureFactory.h"

ABuildingBase::ABuildingBase()
{
	m_pBuildingMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	Cast<USceneComponent>(m_pBuildingMeshComp)->SetupAttachment(m_pCenteredRoot);
	

}

const UStaticMeshComponent *ABuildingBase::GetBuildingMeshComp() const 
{
	return m_pBuildingMeshComp;


}

void ABuildingBase::RefreshIlluminationState()
{
	m_bIsIlluminated = m_pOwningFactory->IsBuildingIlluminated(this);
	

}
