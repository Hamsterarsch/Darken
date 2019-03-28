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

	void Possess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	void SetNewAttackTarget(AActor *pNewTarget);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;



private:
	void UpdateBehavior();

	UPROPERTY()
		TWeakObjectPtr<AActor> m_pRetributionTarget;

	UPROPERTY()
		float m_LastAttackStamp;

	FAIRequestID m_CurrentMoveID, m_LatestMoveResultID;
	FPathFollowingResult m_LatestMoveResult;
	bool m_bLastFrameWasRetribution;


};
