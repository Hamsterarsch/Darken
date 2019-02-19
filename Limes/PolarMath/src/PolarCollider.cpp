#include "hdr/PolarCollider.h"
#include "hdr/Constants.h"

using namespace PolarMath;

double CPolarCollider::GetHalfCircumference() const
{
	return m_LowrangeCenter.Radius * m_HalfWidthAngle * PolarMath::ToRadCoeff;


}
