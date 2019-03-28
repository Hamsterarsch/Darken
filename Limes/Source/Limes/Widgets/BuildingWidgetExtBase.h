// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingWidgetExtBase.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API UBuildingWidgetExtBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetTargetBuilding(class ABuildingBase *pNewTarget);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTargetBuildingUpdated();


protected:
	UPROPERTY(BlueprintReadOnly, DisplayName = "Target Building")
		class ABuildingBase *m_pTargetBuilding;

	
};
