// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceSystem.h"
#include "Building/Production/BuildingProduction.h"
#include "GeneratorModBase.h"
#include "Misc/Limes.h"
#include "Widgets/ResourceWidgetBase.h"


int32 FCitizen::s_TimesInstantiated{ 0 };

FCitizen::FCitizen() :
	LastEatTime{ -1 },
	Type{ ECitizenType::Commoner },
	InstanceId{ s_TimesInstantiated },
	pPawn{ nullptr }
{	
	++s_TimesInstantiated;


}

//Public-----------------------

UResourceSystem::UResourceSystem() :
	m_pResourceWidget{ nullptr }
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 5;


}

void UResourceSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float	WoodDeltaBasis{ 0 },
			FoodDeltaBasis{ 0 };
	{
		int32 InvalidEntries{ 0 };
		for (auto &&pWeakGenerator : m_apAttachedGenerators)
		{
			if (!pWeakGenerator.IsValid())
			{
				++InvalidEntries;
				continue;
			}
			const auto *pGenerator{ pWeakGenerator.Get() };

			switch (pGenerator->GetGeneratedType())
			{
			case EBuildingResourceTypes::Wood:
				WoodDeltaBasis += pGenerator->GetGeneratedAmount(DeltaTime);
				break;
			case EBuildingResourceTypes::Food:
				FoodDeltaBasis += pGenerator->GetGeneratedAmount(DeltaTime);
				break;
			default:
				UE_DEBUG_BREAK();
				UE_LOG(RTS_ResourceSys, Error, TEXT("Found generator with invalid generated resource"));
			}

		}

		if (InvalidEntries != 0)
		{
			UE_LOG(RTS_ResourceSys, Error, TEXT(" \tResource system update:\n \t\t%i invalid entries occured while querying generators"));
		}
	}


	auto WoodDeltaMod{ WoodDeltaBasis },
		 FoodDeltaMod{ FoodDeltaBasis };
	for(auto &&pLocalMod : m_aLocalModStack)
	{
		if(!pLocalMod)
		{
			continue;
		}

		WoodDeltaMod += pLocalMod->GetModDelta(EBuildingResourceTypes::Wood, WoodDeltaBasis);
		FoodDeltaMod += pLocalMod->GetModDelta(EBuildingResourceTypes::Food, FoodDeltaBasis);

	}


	auto WoodDeltaFinal{ WoodDeltaMod },
		 FoodDeltaFinal{ FoodDeltaMod };
	for(auto &&pGlobalMod : m_aGlobalModStack)
	{
		if(!pGlobalMod)
		{
			continue;
		}

		WoodDeltaFinal += pGlobalMod->GetModDelta(EBuildingResourceTypes::Wood, WoodDeltaMod);
		FoodDeltaFinal += pGlobalMod->GetModDelta(EBuildingResourceTypes::Food, FoodDeltaMod);

	}

	m_Food += FoodDeltaFinal;
	m_Wood += WoodDeltaFinal;	

	if(m_pResourceWidget)
	{
		m_pResourceWidget->OnFoodAmountChanged(static_cast<int32>(m_Food));
		m_pResourceWidget->OnWoodAmountChanged(static_cast<int32>(m_Wood));
	}


}

void UResourceSystem::AttachGenerator(const TWeakObjectPtr<const ABuildingProduction> &pToAttach)
{
	if (!pToAttach.IsValid())
	{
		return;
	}

	m_apAttachedGenerators.Add(pToAttach);


}

void UResourceSystem::DetachGenerator(const ABuildingProduction *pToDetach)
{
	TWeakObjectPtr<const ABuildingProduction> AsWeak{ pToDetach };
	m_apAttachedGenerators.RemoveSingle(AsWeak);


}

void UResourceSystem::AttachMod(TSubclassOf<UGeneratorModBase> pModClass, bool bIsGlobal)
{
	auto *pClass{ pModClass.Get() };
	if (!pClass)
	{
		return;
	}

	auto &ModStack{ bIsGlobal ? m_aGlobalModStack : m_aLocalModStack };

	ModStack.Add(NewObject<UGeneratorModBase>(this, pClass));


}

void UResourceSystem::DetachMod(TSubclassOf<UGeneratorModBase> pModClass, bool bIsGlobal)
{
	auto *pClass{ pModClass.Get() };
	if (!pClass)
	{
		return;
	}

	auto &ModStack{ bIsGlobal ? m_aGlobalModStack : m_aLocalModStack };

	int32 FoundAt{ -1 };
	using ElemType = decltype(m_aGlobalModStack)::ElementType;
	ModStack.FindByPredicate([SearchedClass = pClass](const ElemType &Entry)
	{
		return Entry->GetClass() == SearchedClass;

	});

	if (FoundAt >= 0)
	{
		ModStack.RemoveAt(FoundAt);
	}


}

void UResourceSystem::AddCitizens(int32 Count, ECitizenType Type)
{
	GetTargetCountMember(Type) += Count;

	for(int32 Index{ 0 }; Index < Count; ++Index)
	{
		FCitizen Citizen{};
		Citizen.Type = Type;
		Citizen.Name = TEXT("Debug Citizen ") + FString::FromInt(Citizen.GetInstanceId());

		m_sCitizens.Add(Citizen);
		GetTargetIdleSet(Type).Add(Citizen.GetInstanceId());

	}
	PropagatePopulationChange();


}

void UResourceSystem::RemoveCitizen(int32 CitizenId)
{			
	if(auto *pMeta{ GetCitizenMetaTemp(CitizenId) })
	{
		auto Type{ pMeta->Type };

		GetTargetIdleSet(Type).Remove(CitizenId);
		GetTargetUnemployableSet(Type).Remove(CitizenId);

		--GetTargetCountMember(Type);

		m_sCitizens.Remove(CitizenId);
		PropagatePopulationChange();
	}


}

int32 UResourceSystem::EmployWorker(ECitizenType Type)
{
	int32 OutId{ -1 };

	auto &Set{ GetTargetIdleSet(Type) };

	if(Set.Num() <= 0)
	{
		return OutId;
	}
	OutId = *(Set.CreateConstIterator());
	Set.Remove(OutId);

	PropagatePopulationChange();

	return OutId;


}

void UResourceSystem::NotifyWorkerFreed(int32 CitizenId)
{
	if (auto *pCitizen{ m_sCitizens.Find(CitizenId) })
	{
		GetTargetIdleSet(pCitizen->Type).Add(CitizenId);
		PropagatePopulationChange();
	}


}

void UResourceSystem::NotifyWorkerFreed(const TSet<int32> &CitizenIds)
{
	for (auto &&Id : CitizenIds)
	{
		NotifyWorkerFreed(Id);

	}


}

const FCitizen *UResourceSystem::GetCitizenMetaTemp(int32 CitizenId) const
{
	return m_sCitizens.Find(CitizenId);


}


//Protected--------------------

void UResourceSystem::BeginPlay()
{
	Super::BeginPlay();

	if(auto *pClass{ m_ResourceWidgetClass.Get() })
	{
		m_pResourceWidget = CreateWidget<UResourceWidgetBase>(GetWorld(), pClass);
		m_pResourceWidget->AddToViewport();
	}


}

TSet<int32> &UResourceSystem::GetTargetIdleSet(ECitizenType Type) 
{
	switch (Type)
	{
	case ECitizenType::Child:
		return m_sChildrenIdle;
		break;
	default:
		return m_sAdultsIdle;

	}


}

TSet<int32> &UResourceSystem::GetTargetUnemployableSet(ECitizenType Type) 
{
	switch (Type)
	{
	case ECitizenType::Child:
		return m_sChildrenUnemployable;
		break;
	default:
		return  m_sAdultsUnemployable;

	}


}

int32& UResourceSystem::GetTargetCountMember(ECitizenType Type) 
{
	switch (Type)
	{	
	case ECitizenType::Child:
		return m_ChildrenCount;
	default:
		return m_AdultsCount;

	}


}

void UResourceSystem::PropagatePopulationChange()
{
	auto Info{ GetPopulationMeta() };
	m_eOnPopulationChanged.Broadcast(Info);

	if(m_pResourceWidget)
	{
		m_pResourceWidget->OnPopulationMetaChanged(Info);
	}


}

FPopulationMeta UResourceSystem::GetPopulationMeta() const
{
	FPopulationMeta Out{};

	Out.CountAdults = m_AdultsCount;
	Out.CountAdultsIdle = m_sAdultsIdle.Num();

	Out.CountChildren = m_ChildrenCount;
	Out.CountChildrenIdle = m_sChildrenIdle.Num();

	return Out;


}


