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
		

	protected:
		CPolarCollider m_SourceHull;
		CPolarTransform m_SourceTf;


	};

}
