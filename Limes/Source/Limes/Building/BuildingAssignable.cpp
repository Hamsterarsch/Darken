// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingAssignable.h"
#include "Engine/World.h"
#include "Misc/RTSGameMode.h"
#include "Misc/Limes.h"
#include "Resources/ResourceSystem.h"

ABuildingAssignable::ABuildingAssignable() :
	m_AssignedCommonerCount{ 0 },
	m_AssignedChildrenCount{ 0 }
{
}

void ABuildingAssignable::AssignWorker(ECitizenType Type)
{
	if(m_sContainedCitizens.Num() >= m_CitizenCapacity)
	{
		return;
	}

	if(auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		auto Id{ pGm->GetResourceSystem()->EmployWorker(Type) };
		if(Id >= 0)
		{
			m_sContainedCitizens.Add(Id);

			switch(Type)
			{
			case ECitizenType::Child: 
				++m_AssignedChildrenCount;
				break;
			default:
				++m_AssignedCommonerCount;
			
			}
		}
	}
	UpdateProductivity();


}

void ABuildingAssignable::ReleaseWorker(ECitizenType Type)
{
	if (auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		int32 IdToRemove{ -1 };
		auto *pResSys{ pGm->GetResourceSystem() };
		for(auto &&Id : m_sContainedCitizens)
		{
			if (auto *pMeta{ pResSys->GetCitizenMetaTemp(Id) })
			{
				if(pMeta->Type == Type)
				{
					IdToRemove = Id;
					break;
				}
			}
			else
			{
				UE_LOG(RTS_ResourceSys, Error, TEXT("ABuildingAssignable::ReleaseWorker could not get meta for citizen"));
			}

		}
				
		if (IdToRemove >= 0)
		{
			pResSys->NotifyWorkerFreed(IdToRemove);
			m_sContainedCitizens.Remove(IdToRemove);

			switch (Type)
			{
			case ECitizenType::Child:
				--m_AssignedChildrenCount;
				break;
			default:
				--m_AssignedCommonerCount;

			}

		}
	}
	UpdateProductivity();


}

void ABuildingAssignable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (auto *pGm{ Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()) })
	{
		auto *pResSys{ pGm->GetResourceSystem() };
		pResSys->NotifyWorkerFreed(m_sContainedCitizens);
	}


}

void ABuildingAssignable::UpdateProductivity()
{
	m_ProductivityCoeff = m_sContainedCitizens.Num() / static_cast<float>(m_CitizenCapacity);


}
