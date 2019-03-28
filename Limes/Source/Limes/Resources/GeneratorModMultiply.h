// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resources/GeneratorModBase.h"
#include "GeneratorModMultiply.generated.h"


UCLASS(ClassGroup = (Resources))
class LIMES_API UGeneratorModMultiply : public UGeneratorModBase
{
	GENERATED_BODY()

public:
	UGeneratorModMultiply();


protected:
	virtual float GetModDelta_Internal(float Basis) const override;


	UPROPERTY(EditDefaultsOnly)
		float m_Multiplier;


};
