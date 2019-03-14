// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Misc/RTSGameInstance.h"
#include "Misc/Limes.h"
#include "Widgets/BarWidgetBase.h"
#include "WidgetComponent.h"


UHealthComponent::UHealthComponent() :
	m_HpMax{ 100 },
	m_BarDrawSize{ 200, 50 },
	m_ZOffsetFromRoot{ 0 }
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	m_HpCurrent = m_HpMax;

	if (auto *pInst{ URTSGameInstance::s_pInstance })
	{
		auto *pBarClass{ SafeLoadClassPtr(pInst->GetGlobalData().HealthBarWidgetClass) };
		if (pBarClass && pBarClass->IsChildOf<UBarWidgetBase>())
		{
			m_pBarComp = NewObject<UWidgetComponent>(this);
			if(!m_pBarComp)
			{
				return;
			}
			
			m_pBarComp->SetupAttachment(GetOwner()->GetRootComponent());
			m_pBarComp->RegisterComponent();

			m_pBarComp->SetRelativeLocation({ 0,0, m_ZOffsetFromRoot });

			m_pBarComp->SetWidgetSpace(EWidgetSpace::Screen);
			m_pBarComp->SetDrawSize(m_BarDrawSize);

			m_pBarComp->SetWidgetClass(pBarClass);
			m_pBarComp->InitWidget();
		}
	}


}

void UHealthComponent::TakeDamage(const float Damage)
{
	m_HpCurrent -= std::abs(Damage);
	UpdateWidgetState();

	if(m_HpCurrent <= 0)
	{		
		m_eOnHealthZero.Broadcast();
	}
	else
	{
		m_eOnHealthLost.Broadcast(GetRemainingHpPercent());
	}
	

}

float UHealthComponent::GetRemainingHpPercent() const noexcept
{
	auto Out{ m_HpCurrent / m_HpMax };
	return Out < 0 ? 0 : Out;


}


//Protected-----------------

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UHealthComponent::UpdateWidgetState()
{
	if(!m_pBarComp)
	{
		UE_LOG(RTS_CombatSys, Error, TEXT("Health component owned by %s has no widget component for visualization"), *GetOwner()->GetName());
		return;
	}

	if (auto *pWidget{ Cast<UBarWidgetBase>(m_pBarComp->GetUserWidgetObject()) })
	{
		pWidget->SetBarPercent(GetRemainingHpPercent());		
	}
	else
	{
		UE_LOG(RTS_CombatSys, Error, TEXT("Health component owned by %s has no widget for visualization"), *GetOwner()->GetName());
	}


}


