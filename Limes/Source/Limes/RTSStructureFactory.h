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


UCLASS(Abstract)
class LIMES_API ARTSStructureFactory : public ARadialActorBase
{
	GENERATED_BODY()
	
public:	
	ARTSStructureFactory();

	bool TryCommitPreviewBuilding(class ABuildingPreview *pPreviewBuilding);

	FVector Discretize(const FVector &ToConvert) const;

	bool IsPlacableAtPosition(ARadialActorBase *pActor) const;

	UFUNCTION(BlueprintCallable)
		void InstantiatePreviewBuilding(TSoftClassPtr<class ABuildingPreview> Type);
	
	PolarMath::CPolarTransform GetPolarTransform() const;

	double GetCellWidthAngle(double LowRangeRadius) const;

	double GetCellDepth() const;

	double GetCellArcWidth() const;

	void AddChildBuilding(class ABuildingBase *pNewChild);

	void RemoveChildBuilding(class ABuildingBase *pChildToRemove);

	bool HasIntersectionsWithChildBuildings(class ARadialActorBase *pBuildingToTest) const;


	UPROPERTY(EditDefaultsOnly)
		uint32 m_RadiusInCells;
	   	

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void AddCollisionComponents(ARadialActorBase *pActor) const;

	void SetUpGridVisualization();
	
	UPROPERTY(EditAnywhere)
		bool m_bIsMainFactory;

	UPROPERTY(EditDefaultsOnly)
		int32 m_InnermostCellcount;

	UPROPERTY(EditDefaultsOnly)
		int32 m_MaxRingCount;
	   	
	UPROPERTY(EditDefaultsOnly, Meta = ( EditCondition = "m_bIsMainFactoy"))
		float m_MinRadius;

	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "m_bIsMainFactory"))
		int32 m_CellDepthMultiplier;

	UPROPERTY()
		TArray<ABuildingBase *> m_apChildBuildings;

	UPROPERTY()
		UStaticMeshComponent *m_pVisualizerPlane;
	
	PolarMath::CPolarTransform m_PolarTransform;

	SpaceDiscretizer m_SpaceDiscretizer;
	   

private:
	UFUNCTION()
		void OnActorClickedEvent(AActor *pTouchedActor, FKey ButtonPressed);

	
};
