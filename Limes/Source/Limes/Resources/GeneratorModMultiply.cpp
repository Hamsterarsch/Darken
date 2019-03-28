// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneratorModMultiply.h"


UGeneratorModMultiply::UGeneratorModMultiply() :
	m_Multiplier{ 1 }
{
}

float UGeneratorModMultiply::GetModDelta_Internal(float Basis) const
{
	return Basis * m_Multiplier - Basis;


}
