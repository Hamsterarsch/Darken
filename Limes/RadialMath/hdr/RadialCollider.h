#pragma once
#include "RadialTransform.h"

namespace RadialMath
{
	class CRadialCollider
	{
	public:
		bool IntersectsWith(const CRadialCollider &Collider)
		{
			auto ColliderUnwrappedMaxAngle = Collider.m_Min.Angle + m_OffsetToMax.Angle;
			auto thisUnwrappedMaxAngle = this.m_Min.Angle + this.m_OffsetToMax.Angle;

			if
			(
				thisMax.Radius < Collider.m_Min.Radius
				&& this.m_Min.Radius > ColliderMax.Radius
				&& thisUnwrappedMaxAngle < Collider.m_Min.Angle
				&& this.m_Min.Angle > ColliderUnwrappedMaxAngle
			)
			{
				return false;
			}
			else
			{
				return true;
			}


		}


	protected:
		SRadialTransform GetWrappedMaxTransform() const noexcept
		{
			SRadialTransform Out{};
			Out.Radius = m_Min.Radius + m_OffsetToMax.Radius;
			Out.Angle = (m_Min.Angle + m_OffsetToMax.Angle) % 360.d;

			return Out;


		}

		SRadialTransform m_Min, m_OffsetToMax;


	};

}
