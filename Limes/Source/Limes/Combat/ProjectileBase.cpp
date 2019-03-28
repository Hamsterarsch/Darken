// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Misc/Limes.h"
#include "Components/ShapeComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .25;

	m_pMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	m_pMovementComp->SetAutoActivate(false);
	m_pMovementComp->ProjectileGravityScale = 0;
	m_pMovementComp->bRotationFollowsVelocity = true;
	m_pMovementComp->HomingAccelerationMagnitude = 10000;

	m_pMovementComp->OnProjectileStop.AddDynamic(this, &AProjectileBase::OnProjectileStopped);

	OnActorHit.AddDynamic(this, &AProjectileBase::OnActorHitEvent);

}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if((m_SpawnPoint - GetActorLocation()).Size() >= m_MaxFlyDistance)
	{
		Destroy();
	}


}

void AProjectileBase::LaunchAt(AActor *pSource, AActor* pTarget, float Speed, float Damage, float MaxFlyDistance, bool bIsHoming)
{	
	if(!pTarget)
	{
		return;
	}

	if(bIsHoming)
	{
		m_pMovementComp->bIsHomingProjectile = bIsHoming;
		m_pMovementComp->HomingTargetComponent = pTarget->GetRootComponent();
	}

	auto TargetPos{ pTarget->GetActorLocation() };
	LaunchAt(pSource, TargetPos, Speed, Damage, MaxFlyDistance);


}

void AProjectileBase::LaunchAt(AActor *pSource, FVector& Target, float Speed, float Damage, float MaxFlyDistance)
{
	auto DispToTarget{ Target - GetActorLocation() };
	m_pMovementComp->MaxSpeed = Speed;
	m_pMovementComp->Velocity = DispToTarget.GetUnsafeNormal() * Speed;// m_pMovementComp->bIsHomingProjectile ? DispToTarget.GetUnsafeNormal() * Speed * .125 : DispToTarget.GetUnsafeNormal() * Speed;
	m_pSource = pSource;
	m_Damage = Damage;
	m_MaxFlyDistance = MaxFlyDistance;

	m_pMovementComp->Activate(true);

	
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	m_SpawnPoint = GetActorLocation();


}

void AProjectileBase::OnProjectileStopped(const FHitResult &Hit)
{
	if (auto *pHitActor{ Hit.Actor.Get() })
	{
		if(m_Damage > 0)
		{
			//pass the source and not the projectile itself here
			pHitActor->TakeDamage(m_Damage, {}, GetInstigatorController(), m_pSource);
		}
		Destroy();
	}
	else
	{
		UE_LOG(RTS_CombatSys, Error, TEXT("A projectile could not apply damage to its hit target"));
		Destroy();
	}
	

}

void AProjectileBase::OnActorHitEvent(AActor* pSelfActor, AActor* pOtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	m_pMovementComp->StopSimulating(Hit);


}
