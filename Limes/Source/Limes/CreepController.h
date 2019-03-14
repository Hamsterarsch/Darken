// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CreepController.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ACreepController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACreepController();

	virtual void Tick(float DeltaTime) override;

	void SetNewAttackTarget(AActor *pNewTarget) noexcept { m_pRetributionTarget = pNewTarget; }


private:
	void UpdateBehavior();


	UPROPERTY()
		AActor *m_pRetributionTarget;

	UPROPERTY()
		float m_LastAttackStamp;


};
