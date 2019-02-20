#pragma once
#include <vector>


#include "PolarCollider.h"
#include "PolarTransform.h"

namespace PolarMath
{	
	class CPolarCollision
	{
	public:
		CPolarCollision() = default;

		CPolarCollision(const class CPolarCollider &Hull, const CPolarTransform &Source);

		std::vector<class CPolarCollider> GenerateHullsForTarget(const CPolarTransform &Target) const;

		bool HasIntersectionsWith(const CPolarCollision &Other) const;
		
		double GetMainHullDepth() const noexcept;

		double GetMainHullRightAngle() const noexcept;

		double GetMainHullLeftAngle() const noexcept;

		double GetMainHullMinRadius() const noexcept;

		double GetMainHullCenterAngle() const noexcept;

		double GetMainHullHalfWidthAngle() const noexcept;

		struct SVector2D GetCartesianCenter() const noexcept;



	protected:
		CPolarCollider m_SourceHull;
		CPolarTransform m_SourceTf;


	};

}
