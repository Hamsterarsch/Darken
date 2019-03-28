// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveManager.h"
#include "../PolarMath/hdr/PolarPoint.h"	
#include "../PolarMath/hdr/Vector2D.h"
#include "Misc/Limes.h"
#include "Combat/Waves/WaveMob.h"
#include "Engine/World.h"


AWaveManager::AWaveManager() :
	m_CurrentEventIndex{ 0 },
	m_SpawnHeight{ 50 },
	m_MobSpacing{ 200 },
	m_StampLastEventCompletion{ 0 }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = FDaytime::s_HourInSeconds;


}

void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_aEventDescs.Num() <= m_CurrentEventIndex)
	{
		return;
	}

	auto CurrentWorldTime{ GetWorld()->GetTimeSeconds() };
	auto WorldAsDaytime{ FDaytime{ CurrentWorldTime } };

	//auto TimeSinceLastEvent{ CurrentWorldTime - m_StampLastEventCompletion };

	if (m_aEventDescs[m_CurrentEventIndex].Timecode <= WorldAsDaytime)
	{
		auto &WaveDesc{ m_aEventDescs[m_CurrentEventIndex] };
		m_eOnNewWave.Broadcast(WaveDesc.SpawnAngle, m_SpawnRadius);
		SpawnWave(WaveDesc);

		++m_CurrentEventIndex;
	}


}

void AWaveManager::BeginPlay()
{
	Super::BeginPlay();


}

#if WITH_EDITOR
void AWaveManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(!PropertyChangedEvent.MemberProperty || !PropertyChangedEvent.Property)
	{
		return;
	}
		
	if(PropertyChangedEvent.MemberProperty->GetNameCPP() == GET_MEMBER_NAME_STRING_CHECKED(AWaveManager, m_aEventDescs))
	{
		if
		(
			PropertyChangedEvent.Property->GetNameCPP() == GET_MEMBER_NAME_STRING_CHECKED(FDaytime, Day)
			|| PropertyChangedEvent.Property->GetNameCPP() == GET_MEMBER_NAME_STRING_CHECKED(FDaytime, Hour)
		)
		{
			using ValueType = decltype(m_aEventDescs)::ElementType;
			m_aEventDescs.Sort([](const ValueType &Lhs, const ValueType &Rhs)
			{
				return Lhs.Timecode < Rhs.Timecode;


			});
		}		
	}
	

}
#endif

void AWaveManager::SpawnWave(const FWaveEventDesc& WaveDesc)
{
	if(WaveDesc.CreepCount <= 0)
	{
		return;
	}

	UE_LOG
	(RTS_Events, Log, 
		TEXT("Spawning wave at timecode %i/%i (d/h) at angle %f.\nCreeps: %i"),
	WaveDesc.Timecode.Day, WaveDesc.Timecode.Hour, WaveDesc.SpawnAngle, WaveDesc.CreepCount
	);

	auto FrontCartesian{ PolarMath::SPolarPoint{ m_SpawnRadius, WaveDesc.SpawnAngle }.ToCartesian() };
	FVector FrontPos{static_cast<float>(FrontCartesian.X), static_cast<float>(FrontCartesian.Y), m_SpawnHeight};
	
	if(GetCreepClass())
	{
		GetWorld()->SpawnActor(m_pCreepClassLoaded, &FrontPos);

		if(WaveDesc.CreepCount > 1)
		{
			auto IncrementCartesian{ PolarMath::SPolarPoint{ m_SpawnRadius + m_MobSpacing, WaveDesc.SpawnAngle}.ToCartesian() };
			FVector IncrementPos{ static_cast<float>(IncrementCartesian.X), static_cast<float>(IncrementCartesian.Y), m_SpawnHeight };
			FVector IncrementDispCartesian{ IncrementPos - FrontPos };

			for(int32 MobIndex{ 1 }; MobIndex < WaveDesc.CreepCount; ++MobIndex)
			{
				FrontPos += IncrementDispCartesian;
				GetWorld()->SpawnActor(m_pCreepClassLoaded, &FrontPos);

			}
		}		
	}


}

bool AWaveManager::GetCreepClass()
{
	if(!m_pCreepClassLoaded)
	{
		m_pCreepClassLoaded = SafeLoadClassPtr(m_CreepClass);
	}

	return m_pCreepClassLoaded != nullptr;


}



