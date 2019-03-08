// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSStructureFactory.h"
#include "RTSMainStructureFactory.generated.h"

/**
 * 
 */
UCLASS()
class LIMES_API ARTSMainStructureFactory : public ARTSStructureFactory
{
	GENERATED_BODY()
	
public:
	ARTSMainStructureFactory();


protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


	UPROPERTY(EditDefaultsOnly, Category = "Factory|Grid")
		float m_MinRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Factory|Grid")
		int32 m_CellDepthMultiplier;

	UPROPERTY()
		TArray<class ARTSSideStructureFactory *> m_apSideFactories;

	
};
