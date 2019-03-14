// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API UBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetBarPercent(float Percentage);

	
};
