#pragma once

namespace PolarMath
{
	struct SPolarPoint
	{
		SPolarPoint() = default;

		SPolarPoint(double radius, double angle) :
			Radius{ radius },
			Angle{ angle }
		{
		}

		double Radius;
		double Angle;

		struct SVector2D ToCartesian() const;

		//Transforms the point into the polar system of this point's parent
		//(this point and the input point are expressed in different polar systems)
		SPolarPoint InverseTransform(const SPolarPoint &Point) const;
		

		//Transforms the point into the polar system with this polar point at its origin
		//(this and the input point are expressed in the same polar system)
		SPolarPoint Transform(const SPolarPoint &Point) const;
		
		double GetShortestAngleTo(const SPolarPoint &Other) const;

		double GetShortestAngleToSigned(const SPolarPoint &Other) const;

		/*

		CPolarCollider Transform(const CPolarCollider &Collider) const
		{



		}
		*/



	};


}
