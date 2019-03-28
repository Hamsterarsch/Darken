// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NameGenerator.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNameGenrator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIMES_API INameGenrator
{
	GENERATED_BODY()

public:
	virtual FString GetName() = 0;

	
};
