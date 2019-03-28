// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthLostSignature, float, RemainingHpPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoidMultiDelegate);

UCLASS( ClassGroup=(Combat), meta=(BlueprintSpawnableComponent) )
class LIMES_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	
	virtual void InitializeComponent() override;

	float GetRemainingHp() const  { return m_HpMax; }

	float GetRemainingHpPercent() const ;

	void TakeDamage(float Damage);


	UPROPERTY(BlueprintAssignable, DisplayName="On Health Lost")
		FOnHealthLostSignature m_eOnHealthLost;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Health Zero")
		FVoidMultiDelegate m_eOnHealthZero;


protected:
	virtual void BeginPlay() override;

	void UpdateWidgetState();

	UPROPERTY(EditAnywhere, Meta=(ClampMin="1"))
		float m_HpMax;

	UPROPERTY(EditAnywhere, DisplayName="Bar Draw Size")
		FVector2D m_BarDrawSize;

	UPROPERTY(EditAnywhere, DisplayName="Z Offset From Root")
		float m_ZOffsetFromRoot;

	UPROPERTY()
		class UWidgetComponent *m_pBarComp;

	float m_HpCurrent;
		
	
};
