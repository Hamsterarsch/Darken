// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RTSGameMode.generated.h"


/**
 * 
 */
UCLASS()
class LIMES_API ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARTSGameMode();

	UFUNCTION(BlueprintCallable)
		class UResourceSystem *GetResourceSystem() const  { return m_pResourceSystem; }


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
		class UResourceSystem *m_pResourceSystem;


};
