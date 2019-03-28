// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepController.h"
#include "Creep.h"
#include "Engine/World.h"
#include "Building/Beacon/RTSMainStructureFactory.h"
#include "Misc/RTSGameInstance.h"


ACreepController::ACreepController() :
	m_LastAttackStamp{ 0 },
	m_bLastFrameWasRetribution{ false }
{
	PrimaryActorTick.bCanEverTick = true;
	bSetControlRotationFromPawnOrientation = true;

}

void ACreepController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	auto *pPawn{ Cast<ACreep>(GetPawn()) };
	if (!pPawn)
	{
		return;
	}

	auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) };
	if (!pGI)
	{
		return;
	}

	auto *pMainBeacon{ pGI->GetMainStructureFactory() };
	if(pMainBeacon)
	{
		MoveToActor(pMainBeacon, pPawn->GetAttackRange());
		m_CurrentMoveID = GetPathFollowingComponent()->GetCurrentRequestId();	
	}
	else
	{
		UE_DEBUG_BREAK();
	}


}

void ACreepController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBehavior();


}

void ACreepController::SetNewAttackTarget(AActor* pNewTarget)
{
	if(m_pRetributionTarget.IsValid())
	{
		//return if the current retribution target is not dead
		return;
	}

	m_pRetributionTarget = pNewTarget;

	auto *pPawn{ Cast<ACreep>(GetPawn()) };
	if (!pPawn)
	{
		return;
	}
	StopMovement();
	MoveToActor(pNewTarget, pPawn->GetAttackRange());
	m_CurrentMoveID = GetPathFollowingComponent()->GetCurrentRequestId();


}

void ACreepController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	m_LatestMoveResultID = RequestID;
	m_LatestMoveResult = Result;

}

void ACreepController::UpdateBehavior()
{
	auto *pPawn{ Cast<ACreep>(GetPawn()) };
	if(!pPawn)
	{
		return;
	}
	   
	//if we arrived at the actor that should be attacked
	if
	(
		(
			m_CurrentMoveID.GetID() == m_LatestMoveResultID.GetID() 
			|| m_CurrentMoveID.GetID() == TNumericLimits<uint32>::Max()
		) 
		&& m_LatestMoveResult.IsSuccess()
	)
	{
		//is it a retribution target 
		if (auto *pRet{ m_pRetributionTarget.Get() })
		{
			auto WorldTime{ GetWorld()->GetTimeSeconds() };
			if ((WorldTime - m_LastAttackStamp) > pPawn->GetHitsPerSecond())
			{
				m_LastAttackStamp = WorldTime;

				FDamageEvent Event{};
				Event.DamageTypeClass = nullptr;
				pRet->TakeDamage(pPawn->GetDamagePerHit(), Event, this, pPawn);
			}
		}
		//otherwise its the main beacon
		else
		{

			auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) };
			if (!pGI)
			{
				return;
			}
			auto *pMainBeacon{ pGI->GetMainStructureFactory() };
			
			if(m_bLastFrameWasRetribution)
			{
				MoveToActor(pMainBeacon, pPawn->GetAttackRange());
				m_CurrentMoveID = GetPathFollowingComponent()->GetCurrentRequestId();
			}

			FDamageEvent Event{};
			Event.DamageTypeClass = nullptr;
			pMainBeacon->TakeDamage(pPawn->GetBeaconDamage(), Event, this, pPawn);

			pPawn->Destroy();
		}
	}

	m_bLastFrameWasRetribution = m_pRetributionTarget.IsValid();

	

/*


	//has attack target 
	if(m_pRetributionTarget)
	{
		auto DispToTarget{ m_pRetributionTarget->GetActorLocation() - pPawn->GetActorLocation() };
		auto DistToTarget{ DispToTarget.Size2D() };

		
		//is in range -> attack
		if(DistToTarget <= pPawn->GetAttackRange())
		{	
			auto WorldTime{ GetWorld()->GetTimeSeconds() };
			if((WorldTime - m_LastAttackStamp) > pPawn->GetHitsPerSecond())
			{
				m_LastAttackStamp = WorldTime;

				FDamageEvent Event{};
				Event.DamageTypeClass = nullptr;
				m_pRetributionTarget->TakeDamage(pPawn->GetDamagePerHit(), Event, this, pPawn);
			}			
		}
		//not in range -> move to target
		else
		{
			if(GetPathFollowingComponent()->GetMoveGoal() != m_pRetributionTarget)
			{
				MoveToActor(m_pRetributionTarget, pPawn->GetAttackRange() - 1);				
			}
		}
				
	}
	//no attack target
	else
	{
		auto *pGI{ Cast<URTSGameInstance>(GetGameInstance()) };
		if(!pGI)
		{
			return;
		}
		auto *pMainBeacon{ pGI->GetMainStructureFactory() };
		
		auto DispToTarget{ pMainBeacon->GetActorLocation() - pPawn->GetActorLocation() };
		auto DistToTarget{ DispToTarget.Size2D() };//- pMainBeacon->GetInnermostRingRadius()
		
		//is in range -> attack core
		if(DistToTarget <= pPawn->GetAttackRange())
		{
			FDamageEvent Event{};
			Event.DamageTypeClass = nullptr;
			pMainBeacon->TakeDamage(pPawn->GetBeaconDamage(), Event, this, pPawn);

			pPawn->Destroy();			
		}
		//is not in range -> move to core
		else
		{	
			if (GetPathFollowingComponent()->GetMoveGoal() != pMainBeacon)
			{
				MoveToActor(pMainBeacon, pPawn->GetAttackRange() - 1);				
			}
		}				
	}

*/

}
