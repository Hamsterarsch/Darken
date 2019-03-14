// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WaveMob.generated.h"

UCLASS(Abstract, CustomConstructor)
class LIMES_API AWaveMob : public APawn
{
	GENERATED_BODY()

public:
	AWaveMob();

	virtual float GetCurrentHealth() const noexcept { return m_CurrentHealthpoints; }

	float GetAttackRange() const noexcept { return  m_AttackRange; }

	UFUNCTION(BlueprintCallable)
		float GetRemainingHealthPercent() const noexcept { return m_CurrentHealthpoints / m_MaxHealthpoints; }


protected:
	virtual void PostInitializeComponents() override;


	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "1"))
		float m_MaxHealthpoints;

	UPROPERTY(EditDefaultsOnly)
		float m_AttackRange;

	float m_CurrentHealthpoints;
	

	
};
