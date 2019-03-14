// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WaveMob.h"
#include "Creep.generated.h"

UCLASS()
class LIMES_API ACreep : public AWaveMob
{
	GENERATED_BODY()

public:
	ACreep();	

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	float GetDamagePerHit() const noexcept { return m_DamagePerHit; }

	float GetHitsPerSecond() const noexcept { return m_HitsPerSecond; }

	float GetBeaconDamage() const noexcept { return m_DamageToBeacons; }


protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnTakeDamage(AActor *pDamagedActor, float Damage, const class UDamageType *pDamageType, class AController *pInstigatedBy, AActor *pDamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthLostEvent(float RemainingPercent, float RemainingHealth);


protected:
	UPROPERTY()
		class ACreepController *m_pController;

	UPROPERTY(EditDefaultsOnly)
		float m_DamagePerHit;

	UPROPERTY(EditDefaultsOnly)
		float m_HitsPerSecond;

	UPROPERTY(EditDefaultsOnly)
		float m_DamageToBeacons;


	

};
