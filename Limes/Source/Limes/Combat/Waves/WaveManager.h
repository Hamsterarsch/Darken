// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/Daytime.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

//event times must not be dependent on each other
//use fixed hour/day stamps and sort on play

//hour/days should be queriable somewhere

USTRUCT(BlueprintType)
struct FWaveEventDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FDaytime Timecode;

	UPROPERTY(EditAnywhere, Meta=(ClampMin="0"))
		int32 CreepCount;

	UPROPERTY(EditAnywhere, Meta=(ClampMin="0", ClampMax="359"))
		float SpawnAngle;


};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIncomingWaveDelegateSign, float, SpawnAngle, float, SpawnRadius);

UCLASS()
class LIMES_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaveManager();

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintAssignable)
		FIncomingWaveDelegateSign m_eOnNewWave;


protected:
	virtual void BeginPlay() override;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	void SpawnWave(const FWaveEventDesc &WaveDesc);
	
	bool GetCreepClass();

	UPROPERTY(EditDefaultsOnly)
		float m_SpawnHeight;

	UPROPERTY(EditDefaultsOnly)
		float m_MobSpacing;

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class AWaveMob> m_CreepClass;

	UPROPERTY(EditDefaultsOnly)
		TArray<FWaveEventDesc> m_aEventDescs;
		
	UPROPERTY(EditDefaultsOnly)
		float m_SpawnRadius;

	UPROPERTY()
		UClass *m_pCreepClassLoaded;
	
	int32 m_CurrentEventIndex;

	float m_StampLastEventCompletion;
	   
	
};
