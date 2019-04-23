// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialActorBase.h"
#include "BuildingBase.generated.h"

UCLASS(Abstract, CustomConstructor)
class LIMES_API ABuildingBase : public ARadialActorBase
{
	GENERATED_BODY()
	
public:
	ABuildingBase();

	const UStaticMeshComponent *GetBuildingMeshComp() const ;

	void RefreshIlluminationState();


protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent *m_pBuildingMeshComp;

	bool m_bIsIlluminated;

	
};
