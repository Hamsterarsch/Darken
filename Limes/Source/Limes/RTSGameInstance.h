// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

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
		class ARTSStructureFactory *GetMainStructureFactory() { return m_pMainStructureFactory; }
	
	void SetSelectedStructureFactory(class ARTSStructureFactory *pNewFactory);

	void SetMainStructureFactory(class ARTSStructureFactory *pMainStructureFactory);




protected:
	UPROPERTY()
		class ARTSStructureFactory *m_pSelectedStructureFactory;

	UPROPERTY()
		class ARTSStructureFactory *m_pMainStructureFactory;
	
	
};
