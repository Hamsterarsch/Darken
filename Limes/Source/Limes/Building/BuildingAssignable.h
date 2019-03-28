// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BuildingPeopled.h"
#include "Resources/ResourceSystem.h"
#include "BuildingAssignable.generated.h"


/**
 * 
 */
UCLASS(CustomConstructor)
class LIMES_API ABuildingAssignable : public ABuildingPeopled
{
	GENERATED_BODY()

public:
	ABuildingAssignable();

	UFUNCTION(BlueprintCallable)
		void AssignWorker(ECitizenType Type);

	UFUNCTION(BlueprintCallable)
		void ReleaseWorker(ECitizenType Type);

	UFUNCTION(BlueprintCallable)
		float GetProductivity() const  { return m_ProductivityCoeff; }

	UFUNCTION(BlueprintCallable)
		int32 GetWorkerCapacity() const  { return m_CitizenCapacity; }

	UFUNCTION(BlueprintCallable)
		int32 GetNumberAssigned() const { return m_sContainedCitizens.Num(); }

	UFUNCTION(BlueprintCallable)
		int32 GetAssignedCommoners() const { return m_AssignedCommonerCount; }

	UFUNCTION(BlueprintCallable)
		int32 GetAssignedChildren() const { return m_AssignedChildrenCount; }

	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateProductivity();

	int32 m_AssignedCommonerCount;

	int32 m_AssignedChildrenCount;

	float m_ProductivityCoeff;

	
};
