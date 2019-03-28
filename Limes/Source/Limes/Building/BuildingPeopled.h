// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BuildingBase.h"
#include "BuildingPeopled.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ABuildingPeopled : public ABuildingBase
{
	GENERATED_BODY()
	
public:


protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
		void OnCitizenRemoved(int32 CitizenId);


	UPROPERTY(EditDefaultsOnly)
		int32 m_CitizenCapacity;

	UPROPERTY()
		TSet<int32> m_sContainedCitizens;

private:
	TScriptDelegate<> m_OnCitizenRemovedHandle;
	
	
};
