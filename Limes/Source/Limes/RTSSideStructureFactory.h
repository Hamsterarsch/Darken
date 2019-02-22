// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSStructureFactory.h"
#include "RTSSideStructureFactory.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ARTSSideStructureFactory : public ARTSStructureFactory
{
	GENERATED_BODY()
	
public:


protected:
	virtual void PostInitializeComponents() override;


	UPROPERTY(EditDefaultsOnly)
		float m_MinRadiusMultiplier;
	
};
