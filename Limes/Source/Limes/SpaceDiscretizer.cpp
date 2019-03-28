// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDiscretizer.h"
#include <algorithm>

#include "../hdr/Vector2D.h"
#include "PolarPoint.h"
#include "Constants.h"


SpaceDiscretizer::SpaceDiscretizer(FVector Origin, float RadiusMin, int32 InnerCellCount, int32 DepthFactor, int32 MaxRingCount) :
	m_Origin{ std::move(Origin) },
	m_RadiusRangeMin{ RadiusMin },
	m_InnermostCellCount{ InnerCellCount },
	m_MaxRingCount{ MaxRingCount }
{
	m_ComputedCellArcWidth = (PI * 2 * m_RadiusRangeMin) / m_InnermostCellCount;

	auto SecondRingRadius = ((m_InnermostCellCount + 3) * m_ComputedCellArcWidth) / (2 * PI);
	m_ComputedCellDepth = SecondRingRadius - m_RadiusRangeMin;

	m_ComputedCellDepth *= DepthFactor;

	m_ComputedMaxRadiusRange = ComputeMaxRadiusRange();


	UE_LOG(LogTemp, Warning, TEXT("depth: %f, 1stRadius: %f, 2ndRadius: %f, MaxRings: %i circ: %f, innerCount: %i"), m_ComputedCellDepth, m_RadiusRangeMin, SecondRingRadius, m_MaxRingCount, m_ComputedCellArcWidth, m_InnermostCellCount);


}

SpaceDiscretizer::SpaceDiscretizer(FVector Origin, double CellArcWidth, double CellDepth, int32 MaxRingCount, int32 InnerCellCount) :
	m_Origin{ std::move(Origin) },
	m_InnermostCellCount{ InnerCellCount },
	m_ComputedCellDepth{ CellDepth },
	m_ComputedCellArcWidth{ CellArcWidth },
	m_MaxRingCount{ MaxRingCount }
{
	m_RadiusRangeMin = ((m_ComputedCellArcWidth * m_InnermostCellCount) / (2 * PI));	
	m_ComputedMaxRadiusRange = ComputeMaxRadiusRange();


}

FVector SpaceDiscretizer::Discretize(const FVector &ToConvert, int32 CellOffset, uint32 RingDepthOffset, bool bIsHalfOff) const
{
	auto OriginRelativeToConvert{ ToConvert - m_Origin };
	auto AsRadialCoords = PolarMath::ToPolar({ OriginRelativeToConvert.X, OriginRelativeToConvert.Y });
	AsRadialCoords.Radius = FMath::Clamp(AsRadialCoords.Radius, m_RadiusRangeMin, TNumericLimits<double>::Max());

	auto RingNumber = FMath::FloorToInt((AsRadialCoords.Radius - m_RadiusRangeMin) / m_ComputedCellDepth);
	if (m_MaxRingCount > 0)
	{
		RingNumber = std::min(RingNumber, m_MaxRingCount);
	}
	//RingNumber = RingNumber > 0 ? RingNumber : 1;

	AsRadialCoords.Radius = m_RadiusRangeMin + RingNumber * m_ComputedCellDepth + RingDepthOffset * m_ComputedCellDepth;
	auto CellsInRing = m_InnermostCellCount + (3 * RingNumber);

	auto RadiusToRing = (RingNumber * m_ComputedCellDepth) + m_RadiusRangeMin;
	auto RingCellCircumference = m_ComputedCellArcWidth / RadiusToRing;

	auto IsRingEven = RingNumber % 2;
	auto CellIndex = FMath::FloorToInt((FMath::DegreesToRadians(AsRadialCoords.Angle) + RingCellCircumference * .5f * IsRingEven) / RingCellCircumference);
	CellIndex += CellOffset;

	float Offset{ 0 };
	if (bIsHalfOff)
	{
		Offset += .5 * RingCellCircumference;
	}
	//todo: check applies offset 2 times ?
	AsRadialCoords.Angle = FMath::RadiansToDegrees(CellIndex * RingCellCircumference + Offset + (RingCellCircumference * .5f * (1 - IsRingEven)));
	

	auto Out2DVec = AsRadialCoords.ToCartesian();
	return { static_cast<float>(Out2DVec.X) + m_Origin.X, static_cast<float>(Out2DVec.Y) + m_Origin.Y, ToConvert.Z };
	



}

double SpaceDiscretizer::GetCellWidthAngle(double LowRangeRadius) const 
{	
	return  360 / ((2 * PI * LowRangeRadius) / m_ComputedCellArcWidth);


}

double SpaceDiscretizer::GetOutmostRadius() const 
{
	return m_ComputedMaxRadiusRange;


}

int32 SpaceDiscretizer::GetMaxRingCount() const 
{
	return m_MaxRingCount;


}


//Protected--------------------

double SpaceDiscretizer::ComputeMaxRadiusRange() const 
{
	if (m_MaxRingCount < 0)
	{
		return std::numeric_limits<decltype(m_ComputedMaxRadiusRange)>::max();
	}
	return m_RadiusRangeMin + m_ComputedCellDepth * m_MaxRingCount;


}

