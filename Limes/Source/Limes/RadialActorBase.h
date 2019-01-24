// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialActorBase.generated.h"

UCLASS(Abstract)
class LIMES_API ARadialActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	FVector GetRadialOrigin() const;
	void SetParentFactory(class ABuildingFactory *pNewParent);

	uint32 GetCellWidth() const { return m_CellWidth; }
	uint32 GetCellDepth() const { return m_CellDepth; }

protected:
	UPROPERTY()
		class ABuildingFactory *m_pSelectedStructureFactory;
	
	UPROPERTY(EditDefaultsOnly)
		uint32 m_CellWidth;

	UPROPERTY(EditDefaultsOnly)
		uint32 m_CellDepth;

	
};
