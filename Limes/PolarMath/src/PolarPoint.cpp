#include <algorithm>

#include "hdr/Constants.h"
#include "hdr/PolarPoint.h"
#include "hdr/Vector2D.h"
#include "hdr/Globals.h"

using namespace PolarMath;

SVector2D SPolarPoint::ToCartesian() const
{
	SVector2D Out{};

	Out.X = Radius * std::cos(Angle * ToRadCoeff);
	Out.Y = Radius * std::sin(Angle * ToRadCoeff);


	return Out;


}

PolarMath::SPolarPoint PolarMath::SPolarPoint::InverseTransform(const SPolarPoint &Point) const
{
	SPolarPoint Out{};

	//At 180 degree the 'triangle' becomes colinear with the line from the
	//reference point to its coordinate system's origin
	//todo: add geometric examples
	if (Point.Angle < 180 + ReqPrecision && Point.Angle > 180 - ReqPrecision)
	{
		Out.Angle = Angle;
		Out.Radius = Point.Radius + Radius;
		return Out;

	}

	auto WrappedAngle{ std::fmod(Point.Angle, 180) };
	Out.Radius = sqrt(Point.Radius * Point.Radius + Radius * Radius - 2 * Point.Radius * Radius * cos(WrappedAngle * ToRadCoeff));

	//null term should not be possible if none of the Radii is null and Angle != 180
	auto Divisor{ 2 * Out.Radius * Radius };

	Out.Angle = acos
	(
		(Radius * Radius + Out.Radius * Out.Radius - Point.Radius * Point.Radius)
		/ Divisor
	) * ToDegCoeff;

	return Out;


}

PolarMath::SPolarPoint PolarMath::SPolarPoint::Transform(const SPolarPoint &Point) const
{
	SPolarPoint Out{};

	auto UnsignedBiasedAngle{ std::abs(Point.Angle - Angle) };

	if (UnsignedBiasedAngle < 180 + ReqPrecision && UnsignedBiasedAngle > 180 - ReqPrecision)
	{
		Out.Angle = 0;
		Out.Radius = Radius + Point.Radius;
		return Out;

	}

	auto CleanAngle{ UnsignedBiasedAngle > 180 ? 360 - UnsignedBiasedAngle : UnsignedBiasedAngle };

	Out.Radius = sqrt(Point.Radius * Point.Radius + Radius * Radius - 2 * Point.Radius * Radius * cos(CleanAngle * ToRadCoeff));

	//null term should not be possible if none of the Radii is null and Angle != 180
	auto Divisor{ 2 * Out.Radius * Radius };

	Out.Angle = acos
	(
		(Radius * Radius + Out.Radius * Out.Radius - Point.Radius * Point.Radius)
		/ Divisor
	) * ToDegCoeff;

	return Out;



}

double SPolarPoint::GetShortestAngleToSigned(const SPolarPoint &Other) const
{
	return GetShortestAngleBetweenSigned(Angle, Other.Angle);


}

double SPolarPoint::GetShortestAngleTo(const SPolarPoint &Other) const
{
	return GetShortestAngleBetween(Angle, Other.Angle);


}
