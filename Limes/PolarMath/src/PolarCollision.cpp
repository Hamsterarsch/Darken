#include "hdr/PolarCollision.h"

#include <algorithm>
#include "hdr/PolarTransform.h"
#include "hdr/PolarCollider.h"
#include "hdr/Vector2D.h"


using namespace PolarMath;

CPolarCollision::CPolarCollision(const CPolarCollider &Hull, const CPolarTransform &Source, double CellWidthAngle) :
	m_SourceHull{ Hull },
	m_SourceTf{ Source },
	m_CellWidthAngle{ CellWidthAngle }
{
}

std::vector<class CPolarCollider> CPolarCollision::GenerateHullsForTarget(const CPolarTransform &Target) const
{
	//Check for bad ratio
	auto DepthCircRatio{ m_SourceHull.GetDepth() / (m_SourceHull.GetHalfCircumference() * 2) };
	auto CircDepthRatio{ m_SourceHull.GetHalfCircumference() * 2 / m_SourceHull.GetDepth() };
	auto BiggestRatio{ std::max(DepthCircRatio, CircDepthRatio) };

	//Determine slice count
	auto MaxSliceCount{ static_cast<unsigned int>(std::floor(BiggestRatio)) };	
	MaxSliceCount = std::max(1u, MaxSliceCount);

	std::vector<CPolarCollider> OutHulls(MaxSliceCount);
	//simple case
	if (MaxSliceCount <= 1)
	{
		OutHulls.at(0) = m_SourceTf.TransformColliderTo(Target, m_SourceHull);
		return OutHulls;


	}
		
	//get source domain bounds
	constexpr unsigned int RightMin{ 0 }, LeftMin{ 1 }, RightMax{ 2 }, LeftMax{ 3 };
	SPolarPoint SourceBounds[4];
	SourceBounds[RightMax] = m_SourceHull.GetRightMin();
	SourceBounds[LeftMax] = m_SourceHull.GetLeftMin();

	SPolarPoint SourceCenterMid{ m_SourceHull.GetCenterMid() };

	//get depth/angle increment per slice
	bool bSlicingAlongDepth{ m_SourceHull.GetDepth() > m_SourceHull.GetHalfCircumference() * 2 };
	double Increment = bSlicingAlongDepth ? m_SourceHull.GetDepth() / MaxSliceCount : m_SourceHull.GetHalfAngle() * 2 / MaxSliceCount;

	//generate hulls
	for (unsigned int HullIndex{ 0 }; HullIndex < MaxSliceCount; ++HullIndex)
	{
		SourceBounds[RightMin] = SourceBounds[RightMax];
		SourceBounds[LeftMin] = SourceBounds[LeftMax];

		//depth has the bad ratio so slice along depth
		if (bSlicingAlongDepth)
		{
			SourceBounds[RightMax].Radius += Increment;
			SourceBounds[LeftMax].Radius += Increment;
			SourceCenterMid.Radius += Increment;
		}
		//angle has the bad ratio so slice along angle
		else
		{
			SourceBounds[RightMax].Angle = fmod(SourceBounds[RightMax].Angle + Increment, 360);
			SourceBounds[LeftMax].Angle = fmod(SourceBounds[LeftMax].Angle + Increment, 360);
			SourceCenterMid.Angle += fmod(SourceCenterMid.Angle + Increment, 360);
		}

		//transform to target domain
		SPolarPoint TargetPoints[4];
		m_SourceTf.TranformPointsTo(Target, SourceBounds, TargetPoints);
		OutHulls.at(HullIndex) = CPolarCollider{ TargetPoints, m_SourceTf.TransformPointTo(Target, SourceCenterMid) };


	}
	return OutHulls;


}

bool CPolarCollision::HasIntersectionsWith(const CPolarCollision &Other) const
{
	//trivial
	if (m_SourceTf == Other.m_SourceTf)
	{
		return m_SourceHull.HasIntersectionsWith(Other.m_SourceHull, m_CellWidthAngle * .9);
	}

	auto vTargetHulls{ GenerateHullsForTarget(Other.m_SourceTf) };

	for (auto &&Collider : vTargetHulls)
	{
		if (Collider.HasIntersectionsWith(Other.m_SourceHull, m_CellWidthAngle * .9))
		{
			return true;

		}

	}
	return false;


}

double PolarMath::CPolarCollision::GetMainHullDepth() const noexcept
{
	return m_SourceHull.GetDepth();


}

double PolarMath::CPolarCollision::GetMainHullRightAngle() const noexcept
{
	return m_SourceHull.GetRightMin().Angle;


}

double PolarMath::CPolarCollision::GetMainHullLeftAngle() const noexcept
{
	return m_SourceHull.GetLeftMin().Angle;


}

double PolarMath::CPolarCollision::GetMainHullMinRadius() const noexcept
{
	return m_SourceHull.GetLeftMin().Radius;


}

double PolarMath::CPolarCollision::GetMainHullCenterAngle() const noexcept
{
	return m_SourceHull.GetCenterAngle();


}

double PolarMath::CPolarCollision::GetMainHullHalfWidthAngle() const noexcept
{
	return m_SourceHull.GetHalfAngle();


}

SVector2D PolarMath::CPolarCollision::GetCartesianCenter() const noexcept
{
	return m_SourceTf.GetCartesianPos();


}

