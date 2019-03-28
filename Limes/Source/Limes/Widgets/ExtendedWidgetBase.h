// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExtendedWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API UExtendedWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void Show();

	UFUNCTION(BlueprintCallable)
		void Hide();


protected:
	//Widget opacity is 0 and visibility visible when this event is called. 
	//Additional widget animations for widget fade in should be here.
	UFUNCTION(BlueprintImplementableEvent, Category = "Extended")
		void OnShow();

	//Hook for widget fade outs.
	//The widgets visibility is set to collapsed afterwards.
	UFUNCTION(BlueprintImplementableEvent)
		void OnHide();

	
	
};
