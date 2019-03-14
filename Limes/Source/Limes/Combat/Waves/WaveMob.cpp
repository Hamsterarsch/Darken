// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveMob.h"
#include "Combat/HealthComponent.h"


AWaveMob::AWaveMob() :
	m_AttackRange{ 500 }
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	m_pHealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	m_pHealthComp->m_eOnHealthZero.AddDynamic(this, &AWaveMob::ReceiveOnDeath);


}

float AWaveMob::GetCurrentHealth() const
{
	return m_pHealthComp->GetRemainingHp();


}

void AWaveMob::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnTakeAnyDamage.AddDynamic(this, &AWaveMob::OnTakeDamage);


}

void AWaveMob::OnTakeDamage
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

void AWaveMob::ReceiveOnDeath()
{
	OnDeath();
	Destroy();


}
