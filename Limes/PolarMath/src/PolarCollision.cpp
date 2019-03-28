#include "hdr/PolarCollision.h"

#include <algorithm>
#include "hdr/PolarTransform.h"
#include "hdr/PolarCollider.h"
#include "hdr/Vector2D.h"


using namespace PolarMath;

CPolarCollision::CPolarCollision(const CPolarCollider &Hull, const CPolarTransform &Source, double HalfCellWidthArc) :
	m_lSourceHulls{ Hull },
	m_SourceTf{ Source },
	m_HalfCellWidthArc{ HalfCellWidthArc }
{
}

CPolarCollision::CPolarCollision(std::forward_list<CPolarCollider>&& mov_Hulls, const CPolarTransform& Source, double HalfCellWidthArc) :
	m_lSourceHulls{ std::move(mov_Hulls) },
	m_SourceTf{ Source },
	m_HalfCellWidthArc{ HalfCellWidthArc }
{
}

std::forward_list<CPolarCollider> CPolarCollision::GenerateHullsForTarget(
	const CPolarTransform& Target, const CPolarCollider& SourceHull) const
{
	//Check for bad ratio
	auto DepthCircRatio{ SourceHull.GetDepth() / (SourceHull.GetHalfCircumference() * 2) };
	auto CircDepthRatio{ SourceHull.GetHalfCircumference() * 2 / SourceHull.GetDepth() };
	auto BiggestRatio{ std::max(DepthCircRatio, CircDepthRatio) };

	//Determine slice count
	auto MaxSliceCount{ static_cast<unsigned int>(std::floor(BiggestRatio)) };	
	MaxSliceCount = std::max(1u, MaxSliceCount);

	std::forward_list<CPolarCollider> lOutHulls(MaxSliceCount);
	//simple case
	if (MaxSliceCount <= 1)
	{
		lOutHulls.push_front( m_SourceTf.TransformColliderTo(Target, SourceHull) );
		return lOutHulls;		
	}
		
	//get source domain bounds
	constexpr unsigned int RightMin{ 0 }, LeftMin{ 1 }, RightMax{ 2 }, LeftMax{ 3 };
	SPolarPoint SourceBounds[4];
	SourceBounds[RightMax] = SourceHull.GetRightMin();
	SourceBounds[LeftMax] = SourceHull.GetLeftMin();

	SPolarPoint SourceCenterMid{ SourceHull.GetCenterMid() };

	//get depth/angle increment per slice
	bool bSlicingAlongDepth{ SourceHull.GetDepth() > SourceHull.GetHalfCircumference() * 2 };
	double Increment = bSlicingAlongDepth ? SourceHull.GetDepth() / MaxSliceCount : SourceHull.GetHalfAngle() * 2 / MaxSliceCount;

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
		lOutHulls.push_front( CPolarCollider{ TargetPoints, m_SourceTf.TransformPointTo(Target, SourceCenterMid) } );
		
	}
	return lOutHulls;


}

std::forward_list<CPolarCollider> CPolarCollision::GenerateHullsForTarget(const CPolarTransform& Target) const
{
	std::forward_list<CPolarCollider> lTargetHulls{};

	for(auto &&SourceHull : m_lSourceHulls)
	{
		lTargetHulls.splice_after(lTargetHulls.end(), GenerateHullsForTarget(Target, SourceHull));

	}
	return lTargetHulls;


}

bool CPolarCollision::HasIntersectionsWith(const CPolarCollision& Other, CPolarCollider *out_pFirstHit) const
{
	//trivial : same systems
	if (m_SourceTf == Other.m_SourceTf)
	{
		//trivial: only one source hull exists in both collisions
		if (this->HasSingleSourceHull() && Other.HasSingleSourceHull())
		{
			auto HalfCellWidthAngle{ 360 / ((m_lSourceHulls.front().GetRadiusMin() * PolarMath_PI * 2) / m_HalfCellWidthArc) };
			return m_lSourceHulls.front().HasIntersectionsWith(Other.m_lSourceHulls.front(), HalfCellWidthAngle);
		}

		for (auto &&ThisHull : this->m_lSourceHulls)
		{
			for (auto &&OtherHull : Other.m_lSourceHulls)
			{
				auto HalfCellWidthAngle{ 360 / ((ThisHull.GetRadiusMin() * PolarMath_PI * 2) / m_HalfCellWidthArc) };
				if (ThisHull.HasIntersectionsWith(OtherHull, HalfCellWidthAngle))
				{
					if(out_pFirstHit)
					{
						*out_pFirstHit = ThisHull;						
					}
					return true;
				}

			}

		}
		return false;
	}

	auto lHullsInTarget{ GenerateHullsForTarget(Other.m_SourceTf) };
	for (auto &&ThisHull : lHullsInTarget)
	{
		for (auto &&OtherHull : Other.m_lSourceHulls)
		{
			auto HalfCellWidthAngle{ 360 / ((ThisHull.GetRadiusMin() * PolarMath_PI * 2) / m_HalfCellWidthArc) };
			if (ThisHull.HasIntersectionsWith(OtherHull, HalfCellWidthAngle))
			{
				if (out_pFirstHit)
				{
					*out_pFirstHit = OtherHull;
				}
				return true;
			}

		}

	}
	return false;


}

bool CPolarCollision::HasSingleSourceHull() const
{
	return !m_lSourceHulls.empty() && m_lSourceHulls.begin() == m_lSourceHulls.end();


}

double PolarMath::CPolarCollision::GetMainHullDepth() const 
{
	return m_lSourceHulls.front().GetDepth();


}

double PolarMath::CPolarCollision::GetMainHullRightAngle() const 
{
	return m_lSourceHulls.front().GetRightMin().Angle;


}

double PolarMath::CPolarCollision::GetMainHullLeftAngle() const 
{
	return m_lSourceHulls.front().GetLeftMin().Angle;


}

double PolarMath::CPolarCollision::GetMainHullMinRadius() const 
{
	return m_lSourceHulls.front().GetLeftMin().Radius;


}

double PolarMath::CPolarCollision::GetMainHullCenterAngle() const 
{
	return m_lSourceHulls.front().GetCenterAngle();


}

double PolarMath::CPolarCollision::GetMainHullHalfWidthAngle() const 
{
	return m_lSourceHulls.front().GetHalfAngle();


}

SVector2D PolarMath::CPolarCollision::GetCartesianCenter() const 
{
	return m_SourceTf.GetCartesianPos();


}

