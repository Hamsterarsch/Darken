// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API UResourceWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnWoodAmountChanged(int32 NewAmount);

	UFUNCTION(BlueprintImplementableEvent)
		void OnFoodAmountChanged(int32 NewAmount);

	UFUNCTION(BlueprintImplementableEvent)
		void OnPopulationMetaChanged(struct FPopulationMeta Info);
	
	
};
