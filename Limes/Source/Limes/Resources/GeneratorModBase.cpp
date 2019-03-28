// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneratorModBase.h"


UGeneratorModBase::UGeneratorModBase()
{
	PrimaryComponentTick.bCanEverTick = false;


}

float UGeneratorModBase::GetModDelta(EBuildingResourceTypes Resource, float Basis) const
{
	float Out{ 0 };
	if(Resource == m_ModifiedResource)
	{
		Out = GetModDelta_Internal(Basis);
	}
	return Out;


}
