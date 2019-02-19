#include "hdr/PolarTransform.h"
#include "hdr/Vector2D.h"
#include "hdr/PolarPoint.h"
#include "hdr/PolarCollider.h"

using namespace PolarMath;

SPolarPoint CPolarTransform::TransformPointTo(const CPolarTransform &Target, const SPolarPoint &Point) const
{
	auto ToTarget{ Target.m_CartesianPos - m_CartesianPos };

	return (Point.ToCartesian() - ToTarget).ToPolar();


}

CPolarCollider CPolarTransform::TransformColliderTo(const CPolarTransform &Target, const CPolarCollider &Collider) const
{
	return CPolarCollider
	{ 
		{
			this->TransformPointTo(Target, Collider.GetLeftMin()),
			this->TransformPointTo(Target, Collider.GetRightMin()),
			this->TransformPointTo(Target, Collider.GetLeftMax()),
			this->TransformPointTo(Target, Collider.GetRightMax()),
		},
		this->TransformPointTo(Target, Collider.GetCenterMid()) 
	};


}

void CPolarTransform::TranformPointsTo(const CPolarTransform &Target, const SPolarPoint(&Points)[4], SPolarPoint(&OutPoints)[4]) const
{
	for (auto ParallelIndex{ 0 }; ParallelIndex < 4; ++ParallelIndex)
	{
		OutPoints[ParallelIndex] = this->TransformPointTo(Target, Points[ParallelIndex]);

	}
	

}

bool CPolarTransform::operator==(const CPolarTransform Rhs) const
{
	return this->m_CartesianPos == Rhs.m_CartesianPos;


}
