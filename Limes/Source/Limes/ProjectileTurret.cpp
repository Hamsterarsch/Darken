// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileTurret.h"
#include "ProjectileBase.h"
#include "WaveMob.h"
#include "Limes.h"
#include "Engine/World.h"


AProjectileTurret::AProjectileTurret() :
	m_ProjectileDamage{ 25 },
	m_ProjectileCooldown{ 1 },
	m_ProjectileSpeed{ 100 },
	m_bFiresHomingShots{ false },
	m_pLoadedProjectileClass{ nullptr },
	m_LastAttackStamp{ 0 }
{
	PrimaryActorTick.bCanEverTick = true;

	m_pProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));
	m_pProjectileSpawn->SetupAttachment(GetCenteredRoot());


}

void AProjectileTurret::OnEnemyEntersRange(AWaveMob* pMob)
{
	if(pMob)
	{
		m_apMobsInRange.Add(pMob);		
	}


}

void AProjectileTurret::OnEnemyLeavesRange(AWaveMob* pMob)
{
	if(pMob)
	{
		m_apMobsInRange.RemoveSingle(pMob);
	}


}

void AProjectileTurret::AttackMob(AWaveMob* pMob)
{
	if(m_pLoadedProjectileClass)
	{
		auto SpawnLocation{ m_pProjectileSpawn->GetComponentLocation() };
		auto pProjectile{ Cast<AProjectileBase>(GetWorld()->SpawnActor(m_pLoadedProjectileClass, &SpawnLocation)) };

		if(pProjectile)
		{
			pProjectile->LaunchAt(this, pMob, m_ProjectileSpeed, m_ProjectileDamage, m_bFiresHomingShots);
		}


	}


}

void AProjectileTurret::UpdateBehavior()
{
	if(m_apMobsInRange.Num() == 0)
	{
		return;
	}
	

	auto TimeSinceAttack{ GetWorld()->GetTimeSeconds() - m_LastAttackStamp };
	if(TimeSinceAttack < m_ProjectileCooldown)
	{
		return;
	}
	m_LastAttackStamp = GetWorld()->GetTimeSeconds();
		
	decltype(m_apMobsInRange)::ElementType pLowestLifeMob{ nullptr };
	for(auto pMob : m_apMobsInRange)
	{
		if(pLowestLifeMob)
		{
			pLowestLifeMob = pLowestLifeMob->GetCurrentHealth() > pMob->GetCurrentHealth() ? pMob : pLowestLifeMob;
		}
		else
		{
			pLowestLifeMob = pMob;
		}
			   		 
	}

	if(pLowestLifeMob)
	{
		AttackMob(pLowestLifeMob);
	}


}

void AProjectileTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBehavior();


}

void AProjectileTurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_pLoadedProjectileClass = SafeLoadClassPtr(m_ProjectileAsset);


}
