// Fill out your copyright notice in the Description page of Project Settings.

#include "Creep.h"
#include "CreepController.h"


ACreep::ACreep() :
	m_pController{ nullptr },
	m_DamagePerHit{ 10 },
	m_HitsPerSecond{ 1 },
	m_DamageToBeacons{ 10 }
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
	
	if(NewController)
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

void ACreep::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACreep::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnTakeAnyDamage.AddDynamic(this, &ACreep::OnTakeDamage);


}

void ACreep::OnTakeDamage(AActor* pDamagedActor, float Damage, const UDamageType* pDamageType, AController* pInstigatedBy, AActor* pDamageCauser)
{
	if (!m_pController)
	{
		return;
	}

	if (pDamageCauser->bCanBeDamaged)
	{
		m_pController->SetNewAttackTarget(pDamageCauser);
	}

	m_CurrentHealthpoints -= std::abs(Damage);
	OnHealthLostEvent(GetRemainingHealthPercent(), m_CurrentHealthpoints);

	if (m_CurrentHealthpoints <= 0)
	{
		Destroy();
	}


}


