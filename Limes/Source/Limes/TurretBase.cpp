// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretBase.h"
#include "Components/SphereComponent.h"
#include "WaveMob.h"


ATurretBase::ATurretBase() :
	m_AttackRange{ 300 },
	m_MaxHealthpoints{ 100 }
{
	m_pAttackRangeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeTrigger"));
	m_pAttackRangeTrigger->SetupAttachment(GetCenteredRoot());
	m_pAttackRangeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATurretBase::OnAttackRangeBeginOverlap);
	m_pAttackRangeTrigger->OnComponentEndOverlap.AddDynamic(this, &ATurretBase::OnAttackRangeEndOverlap);
	m_pAttackRangeTrigger->SetCollisionProfileName(TEXT("Trigger"));


}

void ATurretBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_pAttackRangeTrigger->SetSphereRadius(m_AttackRange);
	m_CurrentHealthpoints = m_MaxHealthpoints;


}

void ATurretBase::OnAttackRangeBeginOverlap
(
	UPrimitiveComponent *pOverlappedComponent,
	AActor *pOtherActor,
	UPrimitiveComponent *pOtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if(auto *pAsMob{ Cast<AWaveMob>(pOtherActor)} )
	{
		OnEnemyEntersRange(pAsMob);
	}


}

void ATurretBase::OnAttackRangeEndOverlap
(
	UPrimitiveComponent *pOverlappedComponent,
	AActor *pOtherActor,
	UPrimitiveComponent *pOtherComp,
	int32 OtherBodyIndex
)
{
	if(auto *pAsMob{ Cast<AWaveMob>(pOtherActor)})
	{
		OnEnemyLeavesRange(pAsMob);
	}


}
