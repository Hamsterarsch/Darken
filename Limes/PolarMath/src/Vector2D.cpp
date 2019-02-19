#include <algorithm>
#include <cmath>
#include "hdr/Constants.h"
#include "hdr/Vector2D.h"
#include "hdr/PolarPoint.h"

using namespace PolarMath;

const SVector2D SVector2D::AxisX{ 1, 0 };
const SVector2D SVector2D::AxisY{ 0, 1 };

double SVector2D::GetLength() const
{
	return std::sqrt(X * X + Y * Y);


}

double SVector2D::Dot(const SVector2D &Other) const
{
	return this->X * Other.X + this->Y * Other.Y;


}

void SVector2D::Normalize()
{
	auto Length{ GetLength() };
	X /= Length;
	Y /= Length;


}

SVector2D SVector2D::GetNormalized() const
{
	auto Out{ *this };
	Out.Normalize();

	return Out;


}

SPolarPoint SVector2D::ToPolar() const
{
	SPolarPoint Out{};
	Out.Radius = this->GetLength();
	Out.Angle = std::abs(SVector2D::AxisX.GetAngleTo( this->GetNormalized() ));

	return Out;


}

double SVector2D::GetAngleTo(const SVector2D &Other) const
{
	auto DotP{ this->Dot(Other) };
	auto Det{ this->X * Other.Y - this->Y * Other.X };

	auto Angle{ std::atan2(Det, DotP) * PolarMath::ToDegCoeff };
	if (Angle < 0)
	{
		return 360 + Angle;
	}
	return Angle;


}

SVector2D &SVector2D::operator-=(const SVector2D &Rhs)
{
	this->X -= Rhs.X;
	this->Y -= Rhs.Y;

	return *this;


}

SVector2D &SVector2D::operator+=(const SVector2D &Rhs)
{
	this->X += Rhs.X;
	this->Y += Rhs.Y;

	return *this;


}

SVector2D &SVector2D::operator*=(double Scalar)
{
	X *= Scalar;
	Y *= Scalar;

	return *this;


}

bool SVector2D::operator==(const SVector2D &Rhs) const
{
	return
	(this->X + PolarMath::ReqPrecision > Rhs.X && Rhs.X > this->X - PolarMath::ReqPrecision)
	&&
	(this->Y + PolarMath::ReqPrecision > Rhs.Y && Rhs.Y > this->Y - PolarMath::ReqPrecision);


}

SVector2D PolarMath::operator+(SVector2D Lhs, const SVector2D &Rhs)
{
	return Lhs += Rhs;


}

SVector2D PolarMath::operator-(SVector2D Lhs, const SVector2D &Rhs)
{
	return Lhs -= Rhs;


}

SVector2D PolarMath::operator*(SVector2D Lhs, double Scalar)
{
	return Lhs *= Scalar;


}

PolarMath::SPolarPoint PolarMath::ToPolar(SVector2D Vector)
{
	SPolarPoint Out{};
	Out.Radius = Vector.GetLength();
	Out.Angle = std::abs(SVector2D::AxisX.GetAngleTo(Vector.GetNormalized()));

	return Out;


}
