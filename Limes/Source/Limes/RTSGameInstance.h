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
		class ABuildingFactory *GetSelectedStructureFactory() { return m_pSelectedStructureFactory; }

	UFUNCTION(BlueprintCallable)
		class ABuildingFactory *GetMainStructureFactory() { return m_pMainStructureFactory; }
	
	void SetSelectedStructureFactory(class ABuildingFactory *pNewFactory);

	void SetMainStructureFactory(class ABuildingFactory *pMainStructureFactory);

protected:
	UPROPERTY()
		class ABuildingFactory *m_pSelectedStructureFactory;

	UPROPERTY()
		class ABuildingFactory *m_pMainStructureFactory;
	
	
};
