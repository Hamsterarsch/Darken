// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BuildingAssignable.h"
#include "Resources/ModdableGenerator.h"
#include "BuildingProduction.generated.h"

UENUM(BlueprintType)
enum class EBuildingResourceTypes : uint8
{
	None,
	Wood UMETA(DisplayName="Wood"),
	Food UMETA(DisplayName="Food")


};

/**
 * 
 */
UCLASS()
class LIMES_API ABuildingProduction : public ABuildingAssignable
{
	GENERATED_BODY()
	
public:
	virtual float GetGeneratedAmount(float ForSeconds) const { return 0; }

	EBuildingResourceTypes GetGeneratedType() const  { return m_GeneratedResource; }


protected:
	UPROPERTY(EditDefaultsOnly)
		EBuildingResourceTypes m_GeneratedResource;
		
	
};
