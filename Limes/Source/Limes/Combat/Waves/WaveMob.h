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

	float GetAttackRange() const noexcept { return  m_AttackRange; }

	float GetCurrentHealth() const;

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		virtual void OnTakeDamage
		(
			AActor *pDamagedActor,
			float Damage, 
			const class UDamageType *pDamageType, 
			class AController *pInstigatedBy, 
			AActor *pDamageCauser
		);

	UFUNCTION()
		void ReceiveOnDeath();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDeath();


	UPROPERTY(EditDefaultsOnly)
		float m_AttackRange;

	UPROPERTY(VisibleDefaultsOnly)
		class UHealthComponent *m_pHealthComp;

	
};
