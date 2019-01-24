// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceDiscretizer.h"
#include "RadialActorBase.h"
#include "BuildingFactory.generated.h"



UENUM(BlueprintType)
enum class EBuildingTypes : uint8
{
	None,
	SimpleHome

};


UCLASS()
class LIMES_API ABuildingFactory : public ARadialActorBase
{
	GENERATED_BODY()
	
public:	
	ABuildingFactory();

	void CommitPreviewBuilding(const class ABuildingPreview *pPreviewBuilding) const;

	FVector Discretize(const FVector &ToConvert) const;

	bool IsPlacableAtPosition(ARadialActorBase *pActor) const;

	UFUNCTION(BlueprintCallable)
		void InstantiatePreviewBuilding(EBuildingTypes Type) const;
	
	UFUNCTION(BlueprintCallable)
		void SetNewRadialOrigin(const FVector &NewOrigin) noexcept { m_CurrentRadialOrigin = NewOrigin; }
	


	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ABuildingPreview> m_SimpleHomePreviewClass;


protected:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void AddCollisionComponents(ARadialActorBase *pActor) const;

	FVector m_CurrentRadialOrigin{ 0,0,0 };
	SpaceDiscretizer m_SpaceDiscretizer;
	
};
