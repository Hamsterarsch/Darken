// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceDiscretizer.h"
#include "RadialActorBase.h"
#include "PolarTransform.h"
#include "RTSStructureFactory.generated.h"



UENUM(BlueprintType)
enum class EBuildingTypes : uint8
{
	None,
	SimpleHome

};


UCLASS()
class LIMES_API ARTSStructureFactory : public ARadialActorBase
{
	GENERATED_BODY()
	
public:	
	ARTSStructureFactory();

	bool TryCommitPreviewBuilding(class ABuildingPreview *pPreviewBuilding);

	FVector Discretize(const FVector &ToConvert) const;

	bool IsPlacableAtPosition(ARadialActorBase *pActor) const;

	UFUNCTION(BlueprintCallable)
		void InstantiatePreviewBuilding(EBuildingTypes Type);
	
	UFUNCTION(BlueprintCallable)
		void SetNewRadialOrigin(const FVector &NewOrigin) noexcept { m_CurrentRadialOrigin = NewOrigin; }
	

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ABuildingPreview> m_SimpleHomePreviewClass;

	UPROPERTY(EditDefaultsOnly)
		uint32 m_RadiusInCells;


	PolarMath::CPolarTransform GetPolarTransform() const;
	double GetCellWidthAngle(double LowRangeRadius) const;
	double GetCellDepth() const;

	void AddChildBuilding(class ABuildingBase *pNewChild);
	void RemoveChildBuilding(class ABuildingBase *pChildToRemove);

	bool HasIntersectionsWithChildBuildings(class ARadialActorBase *pBuildingToTest) const;



protected:
	UPROPERTY()
		TArray<ABuildingBase *> m_apChildBuildings;

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void AddCollisionComponents(ARadialActorBase *pActor) const;

	FVector m_CurrentRadialOrigin{ 0,0,0 };
	PolarMath::CPolarTransform m_PolarTransform;
	SpaceDiscretizer m_SpaceDiscretizer;
	
};
