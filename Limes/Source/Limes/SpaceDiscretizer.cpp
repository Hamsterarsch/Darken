// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceDiscretizer.h"


FVector SpaceDiscretizer::Discretize(const FVector &ToConvert, int32 CellOffset, uint32 RingDepthOffset, bool bIsHalfOff) const
{
	auto AsRadialCoords = USphericalCoords::CartesianToRadial(ToConvert);
	AsRadialCoords.m_Radius = FMath::Clamp(AsRadialCoords.m_Radius, m_RadiusRangeMin, TNumericLimits<float>::Max());

	auto RingNumber = FMath::FloorToInt((AsRadialCoords.m_Radius - m_RadiusRangeMin) / m_ComputedCellDepth);
	//RingNumber = RingNumber > 0 ? RingNumber : 1;

	AsRadialCoords.m_Radius = m_RadiusRangeMin + RingNumber * m_ComputedCellDepth + m_ComputedCellDepth * 0.5 + RingDepthOffset * m_ComputedCellDepth;
	auto CellsInRing = m_InnermostCellCount + (3 * RingNumber);

	auto RadiusToRing = (RingNumber * m_ComputedCellDepth) + m_RadiusRangeMin;
	auto RingCellCircumference = m_ComputedCellCircumference / RadiusToRing;

	auto IsRingEven = RingNumber % 2;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), AsRadialCoords.m_Azimut);
	auto CellIndex = FMath::FloorToInt((FMath::DegreesToRadians(AsRadialCoords.m_Azimut) + RingCellCircumference * .5f * IsRingEven) / RingCellCircumference);
	CellIndex += CellOffset;

	float Offset{ 0 };
	if (bIsHalfOff)
	{
		Offset += .5 * RingCellCircumference;
	}
	//todo: check applies offset 2 times ?
	AsRadialCoords.m_Azimut = FMath::RadiansToDegrees(CellIndex * RingCellCircumference + Offset + (RingCellCircumference * .5f * (1 - IsRingEven)));


	//UE_LOG(LogTemp, Log, TEXT("RingCellCirc: %f, Cells: %i, even?: %i, cellIndex: %i"), RingCellCircumference, CellsInRing, IsRingEven, CellIndex);

	auto OutVec = USphericalCoords::RadialToCartesian(AsRadialCoords);
	OutVec.Z = ToConvert.Z;

	//UE_LOG(LogTemp, Log, TEXT("Radius: %f, Azim: %f"), AsRadialCoords.m_Radius, AsRadialCoords.m_Azimut);

	return OutVec;


}