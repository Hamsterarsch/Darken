// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveMob.h"

AWaveMob::AWaveMob() :
	m_MaxHealthpoints{ 100 },
	m_AttackRange{ 500 }
{
}

void AWaveMob::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	m_CurrentHealthpoints = m_MaxHealthpoints;


}
