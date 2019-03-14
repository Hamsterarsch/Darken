// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretBase.h"
#include "Components/SphereComponent.h"
#include "Combat/Waves/WaveMob.h"
#include "Combat/HealthComponent.h"


ATurretBase::ATurretBase() :
	m_AttackRange{ 300 },
	m_MaxHealthpoints{ 100 }
{
	m_pAttackRangeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeTrigger"));
	m_pAttackRangeTrigger->SetupAttachment(GetCenteredRoot());
	m_pAttackRangeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATurretBase::OnAttackRangeBeginOverlap);
	m_pAttackRangeTrigger->OnComponentEndOverlap.AddDynamic(this, &ATurretBase::OnAttackRangeEndOverlap);
	m_pAttackRangeTrigger->SetCollisionProfileName(TEXT("Trigger"));

	m_pHealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealtComp"));

	m_pHealthComp->m_eOnHealthZero.AddDynamic(this, &ATurretBase::ReceiveOnDeath);


}

void ATurretBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_pAttackRangeTrigger->SetSphereRadius(m_AttackRange);
	m_CurrentHealthpoints = m_MaxHealthpoints;

	OnTakeAnyDamage.AddDynamic(this, &ATurretBase::OnTakeDamage);


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

void ATurretBase::OnTakeDamage
(
	AActor *pDamagedActor, 
	const float Damage, 
	const UDamageType *pDamageType,
	AController *pInstigatedBy,
	AActor *pDamageCauser
)
{
	if(Damage > 0)
	{
		m_pHealthComp->TakeDamage(Damage);		
	}


}

void ATurretBase::ReceiveOnDeath()
{
	OnDeath();
	Destroy();

}
