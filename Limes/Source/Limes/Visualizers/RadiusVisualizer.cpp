// Fill out your copyright notice in the Description page of Project Settings.


#include "RadiusVisualizer.h"
#include "Misc/RTSGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"


URadiusVisualizer::URadiusVisualizer() :
	m_Radius{ 100 }
{
	PrimaryComponentTick.bCanEverTick = false;

}

void URadiusVisualizer::BeginPlay()
{
	Super::BeginPlay();

	if (auto *pMaterial{ GetMaterial(0) })
	{
		auto *pDynMat{ CreateDynamicMaterialInstance(0, pMaterial) };

		pDynMat->SetScalarParameterValue(TEXT("Radius"), m_Radius);
		pDynMat->SetVectorParameterValue(TEXT("OriginWS"), FLinearColor{ GetComponentLocation() });

		SetRelativeScale3D({ m_Radius / 35, m_Radius /35, 1 });
	}

}
