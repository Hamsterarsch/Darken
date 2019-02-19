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

	void InitializePolarCollision(class ARTSStructureFactory *pNewOwningFactory);

	FVector GetRadialOrigin() const;

	bool HasIntersectionsWith(ARadialActorBase *pRadialActor) const;

	uint32 GetWidthInCells() const { return m_CellWidth; }
	uint32 GetDepthInCells() const { return m_CellDepth; }

protected:
	UPROPERTY()
		class ARTSStructureFactory *m_pOwningFactory;
	
	UPROPERTY(EditDefaultsOnly)
		uint32 m_CellWidth;

	UPROPERTY(EditDefaultsOnly)
		uint32 m_CellDepth;

	PolarMath::CPolarCollision m_PolarCollision;

	bool m_bIsCollisionInitialized;

	
};
