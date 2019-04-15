// Fill out your copyright notice in the Description page of Project Settings.

#include "Creep.h"
#include "CreepController.h"


ACreep::ACreep() :
	m_DamagePerHit{ 10 },
	m_HitsPerSecond{ 1 },
	m_DamageToBeacons{ 10 },
	m_pController{ nullptr }
{ 	
	PrimaryActorTick.bCanEverTick = true;


}

void ACreep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACreep::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController)
	{
		m_pController = NewController->IsA<ACreepController>() ? Cast<ACreepController>(NewController) : nullptr;
	}
	else
	{
		m_pController = nullptr;
	}


}

void ACreep::UnPossessed()
{
	Super::UnPossessed();

	m_pController = nullptr;

	
}

void ACreep::OnTakeDamage
(
	AActor *pDamagedActor, 
	const float Damage, 
	const UDamageType *pDamageType,
	AController *pInstigatedBy, 
	AActor *pDamageCauser
)
{
	Super::OnTakeDamage(pDamagedActor, Damage, pDamageType, pInstigatedBy, pDamageCauser);
	   
	if(!m_pController)
	{
		return;
	}

	if(auto *pCtrl{ Cast<ACreepController>(m_pController) })
	{
		pCtrl->SetNewAttackTarget(pDamageCauser);
	}


}
