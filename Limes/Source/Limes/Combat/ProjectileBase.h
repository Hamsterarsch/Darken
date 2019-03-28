// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS(Abstract)
class LIMES_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	virtual void Tick(float DeltaTime) override;

	void LaunchAt(AActor *pSource, AActor *pTarget, float Speed,  float Damage = -1, float MaxFlyDistance = TNumericLimits<float>::Max(), bool bIsHoming = false);

	void LaunchAt(AActor *pSource, FVector &Target, float Speed, float Damage = -1, float MaxFlyDistance = TNumericLimits<float>::Max());


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnProjectileStopped(const FHitResult &Hit);

	UFUNCTION()
		void OnActorHitEvent(AActor *pSelfActor, AActor *pOtherActor, FVector NormalImpulse, const FHitResult &Hit);

	
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent *m_pMovementComp;

	UPROPERTY()
		class AActor *m_pSource;

	float m_Damage;

	float m_MaxFlyDistance;

	FVector m_SpawnPoint;


};
