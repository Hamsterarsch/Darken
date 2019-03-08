//
#pragma once

#include "CoreMinimal.h"
#include "SphericalCoords.h"

#include <limits>


class LIMES_API SpaceDiscretizer
{
public:
	SpaceDiscretizer() {};
	
	SpaceDiscretizer(FVector Origin, float RadiusMin, int32 InnerCellCount, int32 DepthFactor = 1, int32 MaxRingCount = -1);

	SpaceDiscretizer(FVector Origin, double CellArcWidth, double CellDepth, int32 MaxRingCount, int32 InnerCellCount);
		
	FVector Discretize(const FVector &ToConvert, int32 CellOffset = 0, uint32 RingOffset = 0, bool bIsHalfOff = false) const;

	double GetCellWidthAngle(double LowRangeRadius) const noexcept;

	double GetCellDepth() const noexcept { return m_ComputedCellDepth; }

	double GetCellArcWidth() const noexcept { return m_ComputedCellArcWidth; }

	double GetOutmostRadius() const noexcept;

	double GetInnermostRadius() const noexcept { return m_RadiusRangeMin; }

	double GetInnerCellCount() const noexcept { return m_InnermostCellCount; }

	FVector GetCartesianOrigin() const noexcept { return m_Origin; }

	int32 GetMaxRingCount() const noexcept;

protected:
	double ComputeMaxRadiusRange() const noexcept;

	double m_RadiusRangeMin;

	int32 m_MaxRingCount;

	double m_ComputedMaxRadiusRange;

	int32 m_InnermostCellCount;

	FVector m_Origin;

	double m_ComputedCellArcWidth;

	double m_ComputedCellDepth;


};
