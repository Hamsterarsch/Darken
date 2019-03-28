// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModdableGenerator.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModdableGenerator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIMES_API IModdableGenerator
{
	GENERATED_BODY()

public:
	virtual void AttachMod(TSubclassOf<class UGeneratorModBase> pModClass, bool bIsGlobal = false) = 0;
	
	virtual void DetachMod(TSubclassOf<class UGeneratorModBase> pModClass, bool bIsGlobal = false) = 0;


};
