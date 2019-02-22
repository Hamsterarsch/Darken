//
#pragma once

#include "CoreMinimal.h"
#include "SphericalCoords.h"

class LIMES_API SpaceDiscretizer
{
public:
	SpaceDiscretizer() {};

	SpaceDiscretizer(FVector Origin, float RadiusMin, int32 InnerCellCount, int32 DepthFactor = 1) :
		m_RadiusRangeMax{},
		m_Origin{ std::move(Origin) },
		m_RadiusRangeMin{ RadiusMin },
		m_InnermostCellCount{ InnerCellCount }
	{
		m_ComputedCellArcWidth = (PI * 2 * m_RadiusRangeMin) / m_InnermostCellCount;

		auto SecondRingRadius = ((m_InnermostCellCount + 3) * m_ComputedCellArcWidth) / (2 * PI);
		m_ComputedCellDepth = SecondRingRadius - m_RadiusRangeMin;

		m_ComputedCellDepth *= DepthFactor;

		UE_LOG(LogTemp, Warning, TEXT("depth: %f, 2nd: %f, circ: %f, innerCount: %i"), m_ComputedCellDepth, SecondRingRadius, m_ComputedCellArcWidth, m_InnermostCellCount);

	};

	SpaceDiscretizer(FVector Origin, double CellArcWidth, double CellDepth, int32 InnerCellCount, float MinRadiusMultiplier) :
		m_Origin{ std::move(Origin) },
		m_InnermostCellCount{ InnerCellCount },
		m_ComputedCellDepth{ CellDepth },
		m_ComputedCellArcWidth{ CellArcWidth }
	{
		m_RadiusRangeMin = ((CellArcWidth * m_InnermostCellCount) / (2 * PI)) * MinRadiusMultiplier;
		

	}
		
	FVector Discretize(const FVector &ToConvert, int32 CellOffset = 0, uint32 RingOffset = 0, bool bIsHalfOff = false) const;

	double GetCellWidthAngle(double LowRangeRadius) const noexcept;

	double GetCellDepth() const noexcept { return m_ComputedCellDepth; }

	double GetCellArcWidth() const noexcept { return m_ComputedCellArcWidth; }


protected:
	double m_RadiusRangeMin;

	double m_RadiusRangeMax;

	int32 m_InnermostCellCount;

	FVector m_Origin;

	double m_ComputedCellArcWidth;

	double m_ComputedCellDepth;


};
