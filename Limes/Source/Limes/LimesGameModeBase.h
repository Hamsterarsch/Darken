// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LimesGameModeBase.generated.h"

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()

public:


};



/**
 * 
 */
UCLASS()
class LIMES_API ALimesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddBuilding(UINT PlayerIndex, enum class EBuildingTypes : uint8 BuildingType);
	
	virtual void PreInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ARTSStructureFactory> m_StructureFactoryClass;
	
};
