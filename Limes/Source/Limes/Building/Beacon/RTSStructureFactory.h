// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceDiscretizer.h"
#include "Building/BuildingBase.h"
#include "PolarTransform.h"
#include "RTSStructureFactory.generated.h"



UENUM(BlueprintType)
enum class EBuildingTypes : uint8
{
	None,
	SimpleHome

};


UCLASS(Abstract)
class LIMES_API ARTSStructureFactory : public ABuildingBase
{
	GENERATED_BODY()
	
public:	
	ARTSStructureFactory();

	UFUNCTION(BlueprintCallable)
		void InstantiatePreviewBuilding(const TSoftClassPtr<class ABuildingBase> &TypeToPreview);
	
	bool TryCommitPreviewBuilding(class ABuildingPreview *pPreviewBuilding);

	FVector Discretize(const FVector &ToConvert) const;

	bool IsPlacableAtPosition(ARadialActorBase *pActor) const;

	void AddChildBuilding(class ARadialActorBase *pNewChild);

	void RemoveChildBuilding(class ARadialActorBase *pChildToRemove);

	bool HasIntersectionsWithChildBuildings(class ARadialActorBase *pBuildingToTest) const;

	bool HasIntersectionsWithChildBuildings(const class PolarMath::CPolarCollider &HullToTest, PolarMath::CPolarCollider *out_pFirstHit = nullptr) const;

	void ShowBuildingPlacementGrid();

	void HideBuildingPlacementGrid();

	double GetCellWidthAngle(double LowRangeRadius) const;

	PolarMath::CPolarTransform GetPolarTransform() const { return PolarMath::CPolarTransform { m_pCenteredRoot->GetComponentLocation().X, m_pCenteredRoot->GetComponentLocation().Y }; }

	FVector GetCenteredRootLocation() const { return m_pCenteredRoot->GetComponentLocation(); }

	double GetCellDepth() const  { return m_SpaceDiscretizer.GetCellDepth(); }

	double GetCellWidthArc() const  { return m_SpaceDiscretizer.GetCellArcWidth(); }

	double GetInnermostRingRadius() const  { return m_SpaceDiscretizer.GetInnermostRadius(); }


protected:
	virtual void PostInitializeComponents() override;

	void AddCollisionComponents(ARadialActorBase *pActor) const;

	void SetupGridVisualization();


	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent *m_pVisualizerPlane;
	
	UPROPERTY(EditDefaultsOnly, Category="Factory|Grid")
		int32 m_InnermostCellcount;

	UPROPERTY(EditDefaultsOnly, Category = "Factory|Grid")
		int32 m_MaxRingCount;

	UPROPERTY()
		TArray<ARadialActorBase *> m_apChildBuildings;
		
	SpaceDiscretizer m_SpaceDiscretizer;
	   

private:
	UFUNCTION()
		void OnActorClickedEvent(AActor *pTouchedActor, FKey ButtonPressed);

	
};
