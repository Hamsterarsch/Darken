// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceDiscretizer.h"
#include "../hdr/Vector2D.h"
#include "PolarPoint.h"
#include "Constants.h"



FVector SpaceDiscretizer::Discretize(const FVector &ToConvert, int32 CellOffset, uint32 RingDepthOffset, bool bIsHalfOff) const
{
	auto AsRadialCoords = PolarMath::ToPolar({ ToConvert.X, ToConvert.Y });
	AsRadialCoords.Radius = FMath::Clamp(AsRadialCoords.Radius, m_RadiusRangeMin, TNumericLimits<double>::Max());

	auto RingNumber = FMath::FloorToInt((AsRadialCoords.Radius - m_RadiusRangeMin) / m_ComputedCellDepth);
	//RingNumber = RingNumber > 0 ? RingNumber : 1;

	AsRadialCoords.Radius = m_RadiusRangeMin + RingNumber * m_ComputedCellDepth + RingDepthOffset * m_ComputedCellDepth;
	auto CellsInRing = m_InnermostCellCount + (3 * RingNumber);

	auto RadiusToRing = (RingNumber * m_ComputedCellDepth) + m_RadiusRangeMin;
	auto RingCellCircumference = m_ComputedCellArcWidth / RadiusToRing;

	auto IsRingEven = RingNumber % 2;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), AsRadialCoords.m_Azimut);
	auto CellIndex = FMath::FloorToInt((FMath::DegreesToRadians(AsRadialCoords.Angle) + RingCellCircumference * .5f * IsRingEven) / RingCellCircumference);
	CellIndex += CellOffset;

	float Offset{ 0 };
	if (bIsHalfOff)
	{
		Offset += .5 * RingCellCircumference;
	}
	//todo: check applies offset 2 times ?
	AsRadialCoords.Angle = FMath::RadiansToDegrees(CellIndex * RingCellCircumference + Offset + (RingCellCircumference * .5f * (1 - IsRingEven)));


	//UE_LOG(LogTemp, Log, TEXT("RingCellCirc: %f, Cells: %i, even?: %i, cellIndex: %i"), RingCellCircumference, CellsInRing, IsRingEven, CellIndex);

	auto Out2DVec = AsRadialCoords.ToCartesian();
	return { static_cast<float>(Out2DVec.X), static_cast<float>(Out2DVec.Y), ToConvert.Z };
	

	//UE_LOG(LogTemp, Log, TEXT("Radius: %f, Azim: %f"), AsRadialCoords.m_Radius, AsRadialCoords.m_Azimut);



}

double SpaceDiscretizer::GetCellWidthAngle(double LowRangeRadius) const noexcept
{	
	return  360 / ((2 * PI * LowRangeRadius) / m_ComputedCellArcWidth);


}
