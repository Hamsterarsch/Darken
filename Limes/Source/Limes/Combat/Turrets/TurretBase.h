// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BuildingBase.h"
#include "TurretBase.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ATurretBase : public ABuildingBase
{
	GENERATED_BODY()
	
public:
	ATurretBase();


protected:
	virtual void PostInitializeComponents() override;

	virtual void OnEnemyEntersRange(class AWaveMob *pMob) {};

	virtual void OnEnemyLeavesRange(class AWaveMob *pMob) {};

	UFUNCTION()
		void OnAttackRangeBeginOverlap
		(
			UPrimitiveComponent *pOverlappedComponent, 
			AActor *pOtherActor, 
			UPrimitiveComponent *pOtherComp, 
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult
		);

	UFUNCTION()
		void OnAttackRangeEndOverlap
		(
			UPrimitiveComponent *pOverlappedComponent, 
			AActor *pOtherActor, 
			UPrimitiveComponent *pOtherComp, 
			int32 OtherBodyIndex
		);

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
	
	float GetRemainingHealthPercent() const noexcept { return m_CurrentHealthpoints / m_MaxHealthpoints; }


	UPROPERTY(EditDefaultsOnly)
		float m_AttackRange;

	UPROPERTY(EditDefaultsOnly, Meta=(ClampMin="1"))
		float m_MaxHealthpoints;
	   
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent *m_pAttackRangeTrigger;

	UPROPERTY(VisibleDefaultsOnly)
		class UHealthComponent *m_pHealthComp;

	float m_CurrentHealthpoints;


};
