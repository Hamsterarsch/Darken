// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ExtendedWidgetBase.h"
#include "BuildingWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EBuildingWidgetExtension : uint8
{
	None,
	Assignable
};

/**
 * 
 */
UCLASS()
class LIMES_API UBuildingWidgetBase : public UExtendedWidgetBase
{
	GENERATED_BODY()
	
public:
	void UpdateBuildingExtension(EBuildingWidgetExtension ExtensionType);

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateBuildingExtension(EBuildingWidgetExtension ExtensionType);

	void SetTargetBuilding(class ABuildingBase *pNewTarget)  { m_pTargetBuilding = pNewTarget; }


protected:
	UPROPERTY(BlueprintReadOnly, DisplayName="Target Building")
		class ABuildingBase *m_pTargetBuilding;

	UPROPERTY(BlueprintReadonly, DisplayName="Current Widget Extension")
		EBuildingWidgetExtension m_CurrentWidgetExtension;

	
};
