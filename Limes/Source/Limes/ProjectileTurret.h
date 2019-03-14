// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretBase.h"
#include "ProjectileTurret.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API AProjectileTurret : public ATurretBase
{
	GENERATED_BODY()

public:
	AProjectileTurret();

	virtual void OnEnemyEntersRange(AWaveMob* pMob) override;

	virtual void OnEnemyLeavesRange(AWaveMob* pMob) override;

protected:
	void AttackMob(AWaveMob *pMob);

	virtual void UpdateBehavior();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;


	UPROPERTY(EditDefaultsOnly)
		float m_ProjectileDamage;

	UPROPERTY(EditDefaultsOnly)
		float m_ProjectileCooldown;

	UPROPERTY(EditDefaultsOnly)
		float m_ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly)
		bool m_bFiresHomingShots;

	UPROPERTY(EditDefaultsOnly)
		class USceneComponent *m_pProjectileSpawn;

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class AProjectileBase> m_ProjectileAsset;

	UPROPERTY()
		TArray<AWaveMob *> m_apMobsInRange;

	UPROPERTY()
		UClass *m_pLoadedProjectileClass;
	
	float m_LastAttackStamp;

	
};
