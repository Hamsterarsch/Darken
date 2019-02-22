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

	void InitializePolarCollision(class ARTSStructureFactory *pNewOwningFactory);

	FVector GetRadialOrigin() const;

	bool HasIntersectionsWith(ARadialActorBase *pRadialActor) const;

	UFUNCTION(BlueprintImplementableEvent)
		void OnCollisionInitialized();

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
	
	uint32 GetWidthInCells() const { return m_ActorWidthInCells; }

	uint32 GetDepthInCells() const { return m_ActorDepthInCells; }


protected:
	void SetupHullVisualization();


	UPROPERTY()
		class ARTSStructureFactory *m_pOwningFactory;
	
	UPROPERTY(EditDefaultsOnly)
		uint32 m_ActorWidthInCells;

	UPROPERTY(EditDefaultsOnly)
		uint32 m_ActorDepthInCells;

	UPROPERTY()
		UStaticMeshComponent *m_pHullVisualizerPlane;

	PolarMath::CPolarCollision m_PolarCollision;

	bool m_bIsCollisionInitialized;

	
};
