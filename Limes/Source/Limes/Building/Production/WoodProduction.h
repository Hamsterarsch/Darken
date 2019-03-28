// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Production/BuildingProduction.h"
#include "WoodProduction.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API AWoodProduction : public ABuildingProduction
{
	GENERATED_BODY()

public:
	AWoodProduction();

	virtual float GetGeneratedAmount(float ForSeconds) const override;


protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY(EditDefaultsOnly)
		float m_WoodPerHour;
	
	
	
};
