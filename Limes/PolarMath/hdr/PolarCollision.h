#pragma once
#include <vector>
#include <forward_list>

#include "PolarCollider.h"
#include "PolarTransform.h"

namespace PolarMath
{	
	class CPolarCollision
	{
	public:
		CPolarCollision() = default;

		CPolarCollision(const CPolarCollider &Hull, const CPolarTransform &Source, double HalfCellWidthArc);

		CPolarCollision(std::forward_list<CPolarCollider> &&mov_Hulls, const CPolarTransform &Source, double HalfCellWidthArc);

		std::forward_list<CPolarCollider> GenerateHullsForTarget(const CPolarTransform& Target, const CPolarCollider& SourceHull) const;

		std::forward_list<CPolarCollider> GenerateHullsForTarget(const CPolarTransform& Target) const;

		//Checks for an intersection of this collision instance with the other
		//@param Other:
		//@param out_pFirstHit: If not nullptr contains the first hit collider from this collision, if any
		//@return: Whether or not the collision instances intersect
		bool HasIntersectionsWith(const CPolarCollision &Other, CPolarCollider *out_pFirstHit = nullptr) const;
		
		bool HasSingleSourceHull() const;

		double GetMainHullDepth() const ;

		double GetMainHullRightAngle() const ;

		double GetMainHullLeftAngle() const ;

		double GetMainHullMinRadius() const ;

		double GetMainHullCenterAngle() const ;

		double GetMainHullHalfWidthAngle() const ;

		const std::forward_list<CPolarCollider> &GetTempHullReference() const { return m_lSourceHulls; }
		
		struct SVector2D GetCartesianCenter() const ;



	protected:
		std::forward_list<CPolarCollider> m_lSourceHulls;
		CPolarTransform m_SourceTf;
		double m_HalfCellWidthArc;


	};

}
