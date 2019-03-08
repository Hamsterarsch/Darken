// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FRTSGlobalData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UMaterialInterface *pMaterialBuildingPlaceable;

	UPROPERTY(EditAnywhere)
		class UMaterialInterface *pMaterialBuildingNonPlaceable;

	UPROPERTY()
		class UStaticMesh *pPlaneMesh;


};

/**
 * 
 */
UCLASS()
class LIMES_API URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		class ARTSStructureFactory *GetSelectedStructureFactory() { return m_pSelectedStructureFactory; }
	
	UFUNCTION(BlueprintCallable)
		class ARTSMainStructureFactory *GetMainStructureFactory() { return m_pMainStructureFactory; }
	
	const FRTSGlobalData &GetGlobalData() const noexcept;
	
	void SetSelectedStructureFactory(class ARTSStructureFactory *pNewFactory);

	void SetMainStructureFactory(class ARTSMainStructureFactory *pMainStructureFactory);

	virtual void Init() override;


protected:
	UPROPERTY(EditDefaultsOnly)
		FRTSGlobalData m_GlobalData;

	UPROPERTY()
		class ARTSStructureFactory *m_pSelectedStructureFactory;

	UPROPERTY()
		class ARTSMainStructureFactory *m_pMainStructureFactory;
	
	
};
