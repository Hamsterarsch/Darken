// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialActorBase.h"
#include "BuildingPreview.generated.h"

UCLASS()
class LIMES_API ABuildingPreview : public ARadialActorBase
{
	GENERATED_BODY()
	
public:	
	ABuildingPreview();
	
	UFUNCTION(BlueprintCallable)
		UClass *GetPreviewedClass() const;

	void NotifyNonPlacable();

	void NotifyPlacable();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;


	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ARadialActorBase> m_PreviewedClass;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent *m_pStaticMesh;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr <UMaterialInterface> m_MaterialPlacable;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr <UMaterialInterface> m_MaterialNonPlacable;
	
	
};
