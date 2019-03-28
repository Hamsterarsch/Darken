// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GeneratorModBase.generated.h"


enum class EBuildingResourceTypes : uint8;

UCLASS(Abstract, CustomConstructor, ClassGroup=(Resources))
class LIMES_API UGeneratorModBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGeneratorModBase();

	float GetModDelta(EBuildingResourceTypes Resource, float Basis) const;


protected:
	virtual float GetModDelta_Internal(float Basis) const { return 0; }


	UPROPERTY(EditAnywhere)
		EBuildingResourceTypes m_ModifiedResource;
				
	
};
