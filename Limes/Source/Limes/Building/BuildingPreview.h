// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.h"
#include "BuildingPreview.generated.h"

UCLASS()
class LIMES_API ABuildingPreview : public ABuildingBase
{
	GENERATED_BODY()
	
public:
	static ABuildingPreview *SpawnNewBuildingPreview(class UWorld *pWorld, const TSoftClassPtr<ABuildingBase> &PreviewedBuilding);

	//todo: recheck for protected ctors
	ABuildingPreview();
	
	UFUNCTION(BlueprintCallable)
		UClass *GetPreviewedClass() const;

	void NotifyNonPlacable();

	void NotifyPlacable();

	void ResetPolarCollision();


protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY()
		class UMaterialInterface *m_pMaterialPlacable;

	UPROPERTY()
		class UMaterialInterface *m_pMaterialNonPlacable;
	
	UPROPERTY()
		UClass *m_pPreviewedClass;


};
