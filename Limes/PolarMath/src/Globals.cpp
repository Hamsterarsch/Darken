#include <algorithm>

#include "hdr/Globals.h"

using namespace PolarMath;

double PolarMath::GetShortestAngleBetween(double FirstAngle, double SecondAngle)
{
	auto WrappedAngle{ std::abs(SecondAngle - FirstAngle) };
	return WrappedAngle > 180 ? 360 - WrappedAngle : WrappedAngle;


}

double PolarMath::GetShortestAngleBetweenSigned(double FirstAngle, double SecondAngle)
{
	auto SignedWrappedAngle{ SecondAngle - FirstAngle };

	auto UnsignedWrappedAngle{ std::abs(SignedWrappedAngle) };

	auto UnsignedUnwrappedAngle{ UnsignedWrappedAngle };
	if (UnsignedWrappedAngle > 180)
	{
		if (std::signbit(SignedWrappedAngle))
		{
			UnsignedUnwrappedAngle = 360 - UnsignedWrappedAngle;

		}
		else
		{
			UnsignedUnwrappedAngle = UnsignedWrappedAngle - 360;

		}
		return UnsignedUnwrappedAngle;

	}
	//auto UnsignedUnwrappedAngle{ UnsignedWrappedAngle > 180 ? 360 - UnsignedWrappedAngle : UnsignedWrappedAngle };

	return std::copysign(UnsignedUnwrappedAngle, SignedWrappedAngle);


}
