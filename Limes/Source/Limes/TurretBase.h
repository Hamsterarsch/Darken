// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
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

	virtual void OnEnemyEntersRange(class AWaveMob *pMob) {};

	virtual void OnEnemyLeavesRange(class AWaveMob *pMob) {};

	float GetRemainingHealthPercent() const noexcept { return m_CurrentHealthpoints / m_MaxHealthpoints; }


	UPROPERTY(EditDefaultsOnly)
		float m_AttackRange;

	UPROPERTY(EditDefaultsOnly, Meta=(ClampMin="1"))
		float m_MaxHealthpoints;
	   
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent *m_pAttackRangeTrigger;

	float m_CurrentHealthpoints;


};
