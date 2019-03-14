// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepController.h"
#include "Creep.h"
#include "Engine/World.h"
#include "RTSMainStructureFactory.h"
#include "RTSGameInstance.h"


ACreepController::ACreepController() :
	m_pRetributionTarget{ nullptr },
	m_LastAttackStamp{ 0 }
{
	PrimaryActorTick.bCanEverTick = true;
	bSetControlRotationFromPawnOrientation = true;

}

void ACreepController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBehavior();


}

void ACreepController::UpdateBehavior()
{
	auto *pPawn{ Cast<ACreep>(GetPawn()) };
	if(!pPawn)
	{
		return;
	}

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
			MoveToActor(m_pRetributionTarget);
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
			MoveToActor(pMainBeacon);
		}				
	}
		

}
