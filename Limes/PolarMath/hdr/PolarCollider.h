#pragma once

#include <algorithm>

#include "PolarPoint.h"
#include "Globals.h"

namespace PolarMath
{

	class CPolarCollider
	{
	public:
		CPolarCollider() = default;

		CPolarCollider(const SPolarPoint &LowrangeCenter, double HalfWidth, double Depth) :
			m_LowrangeCenter{ LowrangeCenter }
		{
			m_HalfWidthAngle = HalfWidth < 0 ? 0 : (HalfWidth > 180 ? 180 : HalfWidth);
			m_Depth = std::abs(Depth);

			m_BoundingPoints.LeftMin.Radius = m_BoundingPoints.RightMin.Radius = m_LowrangeCenter.Radius;
			m_BoundingPoints.LeftMax.Radius = m_BoundingPoints.RightMax.Radius = m_LowrangeCenter.Radius + Depth;

			m_BoundingPoints.LeftMin.Angle = m_BoundingPoints.LeftMax.Angle = std::fmod(m_LowrangeCenter.Angle + HalfWidth, 360);

			auto UnsafeAngle{ std::fmod(m_LowrangeCenter.Angle - HalfWidth, 360) };
			if (UnsafeAngle < 0)
			{
				UnsafeAngle = 360 - std::abs(UnsafeAngle);

			}
			m_BoundingPoints.RightMin.Angle = m_BoundingPoints.RightMax.Angle = UnsafeAngle;


		}



		CPolarCollider(const SPolarPoint(&aBoundingPoints)[4], const SPolarPoint &CenterMid)
		{
			const SPolarPoint *aAngleSorted[4]{ &aBoundingPoints[0], &aBoundingPoints[1], &aBoundingPoints[2], &aBoundingPoints[3] };
			const SPolarPoint *aRadiusSorted[4]{ &aBoundingPoints[0], &aBoundingPoints[1], &aBoundingPoints[2], &aBoundingPoints[3] };

			
			std::sort(std::begin(aRadiusSorted), std::end(aRadiusSorted), [](const SPolarPoint * &Lhs, const SPolarPoint * &Rhs) { return Lhs->Radius < Rhs->Radius; });
			std::sort(std::begin(aAngleSorted), std::end(aAngleSorted), [&Center = CenterMid](const SPolarPoint * &Lhs, const SPolarPoint * &Rhs)
			{
				return Center.GetShortestAngleToSigned(*Lhs) < Center.GetShortestAngleToSigned(*Rhs);
			});
			
			auto LargestRadius{ aRadiusSorted[3]->Radius };
			auto SmallestRadius{ aRadiusSorted[0]->Radius };

			auto MaxAngle{ aAngleSorted[3]->Angle };
			auto MinAngle{ aAngleSorted[0]->Angle };

			m_BoundingPoints.LeftMin.Radius = m_BoundingPoints.RightMin.Radius = SmallestRadius;
			m_BoundingPoints.LeftMax.Radius = m_BoundingPoints.RightMax.Radius = LargestRadius;

			m_BoundingPoints.RightMin.Angle = m_BoundingPoints.RightMax.Angle = MinAngle;
			m_BoundingPoints.LeftMin.Angle = m_BoundingPoints.LeftMax.Angle = MaxAngle;

			m_HalfWidthAngle = m_BoundingPoints.LeftMin.GetShortestAngleTo(m_BoundingPoints.RightMin) * .5;
			m_Depth = LargestRadius - SmallestRadius;

			m_LowrangeCenter.Radius = SmallestRadius;
			m_LowrangeCenter.Angle = std::fmod(m_BoundingPoints.RightMin.Angle + m_HalfWidthAngle, 360);



		}

		CPolarCollider(const SPolarPoint &RightMin, const SPolarPoint &LeftMin, const SPolarPoint &RightMax, const SPolarPoint &LeftMax, double HalfWidthAngle, double Depth)
		{
			m_BoundingPoints.RightMin = RightMin;
			m_BoundingPoints.RightMax = RightMax;
			m_BoundingPoints.LeftMin = LeftMin;
			m_BoundingPoints.LeftMax = LeftMax;

			m_HalfWidthAngle = HalfWidthAngle;
			m_Depth = Depth;

			m_LowrangeCenter.Radius = m_BoundingPoints.RightMin.Radius;
			m_LowrangeCenter.Angle = m_BoundingPoints.RightMin.Angle + m_HalfWidthAngle;


		}



		bool HasIntersectionsWith(const CPolarCollider &Other, double AngleBias = 0) const
		{
			double FinalAngleBias{ AngleBias *.125 };
			constexpr double RadiusBias{ 5e-4 };

			auto PolarDist{ GetShortestAngleBetween(this->GetCenterAngle(), Other.GetCenterAngle()) };
				
			if
			(
				(
				this->GetRadiusMax() <= Other.GetRadiusMin() + RadiusBias
				|| this->GetRadiusMin() >= Other.GetRadiusMax() - RadiusBias
				)
				|| PolarDist + FinalAngleBias >= Other.GetHalfAngle() + this->GetHalfAngle()
			)
			{
				return false;
			}
			else
			{
				return true;
			}


		}

		double GetHalfCircumference() const;

		double GetDepth() const	{ return m_Depth; }

		double GetRadiusMin() const	{ return m_LowrangeCenter.Radius; }

		double GetRadiusMax() const	{ return m_LowrangeCenter.Radius + m_Depth;	}

		double GetHalfAngle() const	{ return m_HalfWidthAngle; }

		double GetCenterAngle() const { return m_LowrangeCenter.Angle; }

		SPolarPoint GetLeftMin() const { return m_BoundingPoints.LeftMin; }

		SPolarPoint GetLeftMax() const { return m_BoundingPoints.LeftMax; }

		SPolarPoint GetRightMin() const { return m_BoundingPoints.RightMin;	}

		SPolarPoint GetRightMax() const	{ return m_BoundingPoints.RightMax; }

		SPolarPoint GetCenterMin() const { return m_LowrangeCenter; }

		SPolarPoint GetCenterMax() const
		{
			auto Out{ m_LowrangeCenter };
			Out.Radius += m_Depth;

			return Out;


		}

		SPolarPoint GetCenterMid() const
		{
			auto Out{ m_LowrangeCenter };
			Out.Radius += .5 * m_Depth;

			return Out;

		}
		

	protected:
		SPolarPoint m_LowrangeCenter;
		double	m_HalfWidthAngle,
				m_Depth;

		union
		{
			struct
			{
				SPolarPoint RightMin,
							LeftMin,
							RightMax,
							LeftMax;
			};
			SPolarPoint Mem[4];
		}m_BoundingPoints;


	};

}