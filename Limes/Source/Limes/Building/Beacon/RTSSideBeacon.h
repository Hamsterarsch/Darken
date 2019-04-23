// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BuildingBase.h"
#include "RTSSideBeacon.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ARTSSideBeacon : public ABuildingBase
{
	GENERATED_BODY()

public:

	float GetIlluminationRadius() const;


protected:
	UPROPERTY(EditDefaultsOnly)
		float m_IlluminationRadius;

	
};
