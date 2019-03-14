// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSStructureFactory.h"
#include "RTSSideStructureFactory.generated.h"

/**
 * 
 */
UCLASS(CustomConstructor)
class LIMES_API ARTSSideStructureFactory : public ARTSStructureFactory
{
	GENERATED_BODY()
	
public:
	ARTSSideStructureFactory();


protected:	
	virtual void BeginPlay() override;

	virtual void OnCollisionInitialized() override;


};
