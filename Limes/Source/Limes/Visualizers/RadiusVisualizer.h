// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RadiusVisualizer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIMES_API URadiusVisualizer : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	URadiusVisualizer();


	UPROPERTY(EditAnywhere)
		float m_Radius;


protected:
	virtual void BeginPlay() override;

		
};
