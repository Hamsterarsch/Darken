// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolarCollision.h"
#include "RadialActorBase.generated.h"


UCLASS(Abstract)
class LIMES_API ARadialActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARadialActorBase();

	virtual void PostInitializeComponents() override;

	void InitMinimumCollision(class ARTSStructureFactory *pOwningFactory);

	void InitExpandedCollision(class ARTSStructureFactory *pOwningFactory);

	FVector GetRadialOrigin() const;

	bool HasIntersectionsWith(ARadialActorBase *pRadialActor) const;

	bool HasIntersectionsWith(const PolarMath::CPolarCollider &HullToTest, PolarMath::CPolarCollider *out_pFirstHit = nullptr) const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullDepth() const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullRightAngle() const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullLeftAngle() const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullMinRadius() const;

	UFUNCTION(BlueprintCallable)
		FVector2D GetCollisionCartesianCenter() const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullHalfWidthAngle() const;

	UFUNCTION(BlueprintCallable)
		float GetMainHullCenterAngle() const;
	
	uint32 GetWidthInCells() const  { return m_ActorWidthInCells; }

	uint32 GetDepthInCells() const  { return m_ActorDepthInCells; }

	class USceneComponent *GetCenteredRoot() const  { return m_pCenteredRoot; }


protected:
	void SetupHullVisualization();

	void CenterCenteredRoot();

	virtual void OnCollisionInitialized();

	void ResetHullVisualization();


	UPROPERTY(EditDefaultsOnly)
		uint32 m_ActorWidthInCells;

	UPROPERTY(EditDefaultsOnly)
		uint32 m_ActorDepthInCells;

	UPROPERTY()
		class USceneComponent *m_pCenteredRoot;

	UPROPERTY()
		class ARTSStructureFactory *m_pOwningFactory;

	UPROPERTY()
		TArray<class UStaticMeshComponent *> m_apHullVisPlanes;

	PolarMath::CPolarCollision m_PolarCollision;

	bool m_bIsCollisionInitialized;

	
};
