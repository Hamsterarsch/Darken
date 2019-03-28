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

	float GetDamagePerHit() const  { return m_DamagePerHit; }

	float GetHitsPerSecond() const  { return m_HitsPerSecond; }

	float GetBeaconDamage() const  { return m_DamageToBeacons; }

	
protected:
	virtual void OnTakeDamage
	(
		AActor *pDamagedActor, 
		float Damage, 
		const UDamageType *pDamageType, 
		AController *pInstigatedBy, 
		AActor *pDamageCauser
	)
	override;
	

	UPROPERTY(EditDefaultsOnly)
		float m_DamagePerHit;

	UPROPERTY(EditDefaultsOnly)
		float m_HitsPerSecond;

	UPROPERTY(EditDefaultsOnly)
		float m_DamageToBeacons;

	UPROPERTY()
		class ACreepController *m_pController;
	

};
