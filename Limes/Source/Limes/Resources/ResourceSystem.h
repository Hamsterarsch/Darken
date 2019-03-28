// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModdableGenerator.h"
#include "ResourceSystem.generated.h"


USTRUCT(BlueprintType)
struct FPopulationMeta
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		int32 CountAdults;

	UPROPERTY(BlueprintReadWrite)
		int32 CountAdultsIdle;

	UPROPERTY(BlueprintReadWrite)
		int32 CountChildren;

	UPROPERTY(BlueprintReadWrite)
		int32 CountChildrenIdle;
	

};

UENUM(BlueprintType)
enum class ECitizenType : uint8
{
	Commoner UMETA(DisplayName="Commoner"),
	Child UMETA(DisplayName="Child")

};

USTRUCT()
struct FCitizen
{
	GENERATED_BODY()

public:
	FCitizen();

	int32 GetInstanceId() const  { return InstanceId; }


	UPROPERTY()
		FString Name;

	UPROPERTY()
		APawn *pPawn;
	
	ECitizenType Type;

	float LastEatTime;

private:
	int32 InstanceId;

	static int32 s_TimesInstantiated;


};

struct FCitizenKeyFuncs : BaseKeyFuncs<FCitizen, int32>
{
	static FORCEINLINE KeyInitType GetSetKey(ElementInitType Element)
	{
		return Element.GetInstanceId();
	}

	static FORCEINLINE bool Matches(KeyInitType A, KeyInitType B)
	{
		return A == B;
	}

	static FORCEINLINE uint32 GetKeyHash(KeyInitType Key)
	{
		return GetTypeHash(Key);
	}
	
	 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCitizenEvent, int32, CitizenId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopulationMetaEvent, FPopulationMeta, Info);

UCLASS( ClassGroup=(Resources), Meta=(BlueprintSpawnableComponent) )
class LIMES_API UResourceSystem : public UActorComponent, public IModdableGenerator
{
	GENERATED_BODY()

public:	
	UResourceSystem();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttachGenerator(const TWeakObjectPtr<const class ABuildingProduction> &pToAttach);

	void DetachGenerator(const class ABuildingProduction *pToDetach);

	void AttachMod(TSubclassOf<UGeneratorModBase> pModClass, bool bIsGlobal) override;

	void DetachMod(TSubclassOf<UGeneratorModBase> pModClass, bool bIsGlobal) override;

	void AddCitizens(int32 Count, ECitizenType Type);

	void RemoveCitizen(int32 CitizenId);

	int32 EmployWorker(ECitizenType Type);

	void NotifyWorkerFreed(int32 CitizenId);

	void NotifyWorkerFreed(const TSet<int32> &CitizenIds);

	const FCitizen *GetCitizenMetaTemp(int32 CitizenId) const;

	UFUNCTION(BlueprintCallable)
		FPopulationMeta GetPopulationMeta() const;
	   
	UFUNCTION(BlueprintCallable)
		int32 GetWoodUnits() const  { return static_cast<int32>(m_Wood); }

	UFUNCTION(BlueprintCallable)
		int32 GetFoodUnits() const  { return static_cast<int32>(m_Food); }


	UPROPERTY(BlueprintAssignable)
		FCitizenEvent m_eOnCitizenWorkstateUpdated;

	UPROPERTY(BlueprintAssignable)
		FPopulationMetaEvent m_eOnPopulationChanged;

	UPROPERTY(BlueprintAssignable)
		FCitizenEvent m_eOnCitizenMetaUpdated;

	UPROPERTY(BlueprintAssignable)
		FCitizenEvent m_eOnCitizenRemoved;
		

protected:
	virtual void BeginPlay() override;

	TSet<int32> &GetTargetIdleSet(ECitizenType Type) ;

	TSet<int32> &GetTargetUnemployableSet(ECitizenType Type) ;

	int32 &GetTargetCountMember(ECitizenType Type) ;

	void PropagatePopulationChange();


	UPROPERTY(EditDefaultsOnly)
		float m_Food;

	UPROPERTY(EditDefaultsOnly)
		float m_Wood;

	UPROPERTY(EDitDefaultsOnly)
		TSubclassOf<class UResourceWidgetBase> m_ResourceWidgetClass;

	UPROPERTY()
		class UResourceWidgetBase *m_pResourceWidget;

	UPROPERTY()
		TArray<TWeakObjectPtr<const ABuildingProduction>> m_apAttachedGenerators;
	
	UPROPERTY()
		TArray<class UGeneratorModBase *> m_aLocalModStack;

	UPROPERTY()
		TArray<class UGeneratorModBase *> m_aGlobalModStack;
	
	TSet<FCitizen, FCitizenKeyFuncs> m_sCitizens;

	int32 m_AdultsCount, m_ChildrenCount;

	TSet<int32> m_sAdultsIdle;

	TSet<int32> m_sAdultsUnemployable;

	TSet<int32> m_sChildrenIdle;

	TSet<int32> m_sChildrenUnemployable;

	bool m_bChildrenAreWorkers;
	

};
