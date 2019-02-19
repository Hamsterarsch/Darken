#include "pch.h"
#include "gtest/gtest.h"
#include "hdr/PolarTransform.h"
#include "hdr/PolarPoint.h"
#include "hdr/Globals.h"
#include "hdr/Vector2D.h"

#include <iostream>


#include "hdr/PolarCollider.h"
#include "hdr/PolarCollision.h"

TEST(TransformTests, ToParent_Arbitrary)
{
	//Arbitrary case
	PolarMath::CPolarTransform MainFactory{ 0,0 };
	PolarMath::CPolarTransform SubFactory{ 3, 3 };
	PolarMath::SPolarPoint SubFactoryPoint{ 3, 270 };
	PolarMath::SPolarPoint SubFactoryPoint2{ 3, 180 };

	auto Transformed{ SubFactory.TransformPointTo(MainFactory, SubFactoryPoint) };
	EXPECT_NEAR(0, Transformed.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(3, Transformed.Radius, PolarMath::ReqPrecision);

	Transformed = SubFactory.TransformPointTo(MainFactory, SubFactoryPoint2);
	EXPECT_NEAR(90, Transformed.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(3, Transformed.Radius, PolarMath::ReqPrecision);


}

TEST(TransformTests, ToParent_Collinear)
{
	PolarMath::CPolarTransform MainFactory{ 0, 0 };
	PolarMath::CPolarTransform SubFactory{ -2, 0 };
	PolarMath::SPolarPoint Point{ 2, 180 };

	auto Transformed{ SubFactory.TransformPointTo(MainFactory, Point) };

	EXPECT_NEAR(180, Point.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(4, Point.Radius, PolarMath::ReqPrecision);
	
	
}

TEST(TransformTests, ToOther_Arbitrary)
{
	//make a more complex test

	PolarMath::CPolarTransform SubFactory1{ 2, 0 };
	PolarMath::CPolarTransform SubFactory2{ -2, 0 };
	PolarMath::SPolarPoint Point{ 2, 0 };

	auto Transformed{ SubFactory1.TransformPointTo(SubFactory2, Point) };

	EXPECT_NEAR(0, Point.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(6, Point.Radius, PolarMath::ReqPrecision);


}

/*polar approaches
TEST(TransformTests, ToChild_Arbitrary)
{
	//Arbitrary case
	PolarMath::SPolarPoint TargetSubFactory{ 3, 225 };
	PolarMath::SPolarPoint RootPoint{ 2, 180 };

	auto PointFactoryRelative{ TargetSubFactory.Transform(RootPoint) };

	EXPECT_NEAR(41.726765054188, PointFactoryRelative.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(2.124786724771, PointFactoryRelative.Radius, PolarMath::ReqPrecision);


}

TEST(TransformTests, ToChild_Collinear)
{
	//Colinnear case
	PolarMath::SPolarPoint TargetSubFactory{ 1, 180 };
	PolarMath::SPolarPoint RootPoint{ 2, 0 };

	auto PointFactoryRelative{ TargetSubFactory.Transform(RootPoint) };

	EXPECT_NEAR(0, PointFactoryRelative.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(3, PointFactoryRelative.Radius, PolarMath::ReqPrecision);


}
*/

TEST(GlobalFunctTests, ShortestAngleBetween)
{
	EXPECT_DOUBLE_EQ(5, PolarMath::GetShortestAngleBetween(10, 15));
	EXPECT_DOUBLE_EQ(10, PolarMath::GetShortestAngleBetween(355, 5));
	EXPECT_DOUBLE_EQ(PolarMath::GetShortestAngleBetween(355, 5), PolarMath::GetShortestAngleBetween(5, 355));
	EXPECT_DOUBLE_EQ(180, PolarMath::GetShortestAngleBetween(0, 180));


}

TEST(GlobalFunctTests, ShortestAngleBetweenSigned)
{
	EXPECT_DOUBLE_EQ(5, PolarMath::GetShortestAngleBetweenSigned(10, 15));
	EXPECT_DOUBLE_EQ(10, PolarMath::GetShortestAngleBetweenSigned(355, 5));
	EXPECT_DOUBLE_EQ(-10, PolarMath::GetShortestAngleBetweenSigned(5, 355));
	EXPECT_DOUBLE_EQ(-21, PolarMath::GetShortestAngleBetweenSigned(43, 22));
	EXPECT_DOUBLE_EQ(180, PolarMath::GetShortestAngleBetweenSigned(0, 180));
	EXPECT_DOUBLE_EQ(-180, PolarMath::GetShortestAngleBetweenSigned(180, 0));
	EXPECT_DOUBLE_EQ(-90, PolarMath::GetShortestAngleBetweenSigned(90, 360));


}

TEST(CollisionTests, PolarOnly)
{
	constexpr double UniformRadius{ 2 };
	constexpr double UniformDepth{ 2 };

	PolarMath::SPolarPoint Center90{ UniformRadius, 90 };
	PolarMath::CPolarCollider Col_90_10{ Center90, 10, UniformDepth };

	PolarMath::SPolarPoint Center45{ UniformRadius, 45 };
	PolarMath::CPolarCollider Col_45_5{ Center45, 5, UniformDepth };

	//Commutativity check
	EXPECT_FALSE(Col_90_10.HasIntersectionsWith(Col_45_5));
	EXPECT_FALSE(Col_45_5.HasIntersectionsWith(Col_90_10));

	//Self intersection 
	EXPECT_TRUE(Col_90_10.HasIntersectionsWith(Col_90_10));
	EXPECT_TRUE(Col_45_5.HasIntersectionsWith(Col_45_5));

	//One nonfractional unit difference
	PolarMath::SPolarPoint Center32{ UniformRadius, 32 };
	PolarMath::CPolarCollider Col_32_8{ Center32, 8, UniformDepth };

	EXPECT_TRUE(Col_32_8.HasIntersectionsWith(Col_45_5));

	//Wrapping independence
	PolarMath::SPolarPoint Center359{ UniformRadius, 359 };
	PolarMath::CPolarCollider Col_359_11{ Center359, 11, UniformDepth };

	PolarMath::SPolarPoint Center14{ UniformRadius, 14 };
	PolarMath::CPolarCollider Col_14_13{ Center14, 13, UniformDepth };

		//2nd without wrapping
	EXPECT_TRUE(Col_359_11.HasIntersectionsWith(Col_14_13));

	PolarMath::CPolarCollider Col_14_20{ Center14, 20, UniformDepth };

	EXPECT_TRUE(Col_359_11.HasIntersectionsWith(Col_14_20));


}


TEST(CollisionTests, Intersystem)
{
	PolarMath::CPolarTransform CartesianOrigin{ 0, 0 };
	PolarMath::SPolarPoint MainLowCenter{ 1 , 0 };
	PolarMath::CPolarCollider MainHull{ MainLowCenter, 5, 1 };
	PolarMath::CPolarCollision MainCollision{ MainHull, CartesianOrigin };

	PolarMath::CPolarTransform SubPos{ 3, 0 };
	PolarMath::SPolarPoint SubLowCenter{ 1 , 180 };
	PolarMath::CPolarCollider SubHull{ SubLowCenter, 5, 1 };
	PolarMath::CPolarCollision SubCollision{ SubHull, SubPos };

	EXPECT_TRUE(MainCollision.HasIntersectionsWith(SubCollision));

	PolarMath::SPolarPoint SubLowCenter2{ 1, 0 };
	PolarMath::CPolarCollider SubHull2{ SubLowCenter2, 5, 1 };
	PolarMath::CPolarCollision SubCollision2{ SubHull2, SubPos };

	EXPECT_FALSE(MainCollision.HasIntersectionsWith(SubCollision2));


}

TEST(CollisionTests, Intrasystem)
{
	PolarMath::CPolarTransform CartesianOrigin{ 0, 0 };
	PolarMath::SPolarPoint MainLowCenter{ 1 , 0 };
	PolarMath::CPolarCollider MainHull2{ MainLowCenter, 45, 1 };
	PolarMath::SPolarPoint MainLowCenter2{ 1, 270 };
	PolarMath::CPolarCollider MainHull3{ MainLowCenter2, 45, 1 };

	PolarMath::CPolarCollision Col1{ MainHull2, CartesianOrigin };
	PolarMath::CPolarCollision Col2{ MainHull3, CartesianOrigin };

	EXPECT_TRUE(Col1.HasIntersectionsWith(Col2));



}

TEST(CollisionTests, CommonCases)
{
	{
		PolarMath::SPolarPoint Center90_R4{ 4, 90 };
		PolarMath::SPolarPoint Center96_R1{ 1, 96 };

		PolarMath::CPolarCollider Col_90_R4_10_D2{ Center90_R4, 10, 2 };
		PolarMath::CPolarCollider Col_96_R1_14_D3{ Center96_R1, 14, 3 };

		EXPECT_TRUE(Col_90_R4_10_D2.HasIntersectionsWith(Col_96_R1_14_D3));

	}

	{
		PolarMath::SPolarPoint Center1{ 18.42, 77.4 };
		PolarMath::SPolarPoint Center2{ 5.222211, 5 };

		PolarMath::CPolarCollider Col_1{ Center1, 180, 2 };
		PolarMath::CPolarCollider Col_2{ Center2, 14, 3 };

		EXPECT_FALSE(Col_1.HasIntersectionsWith(Col_2));

	}
	

}

TEST(AngleTests, CommonCases)
{
	PolarMath::FAngle Angle360{ 360 };
	PolarMath::FAngle Angle0{ 360 };

	EXPECT_TRUE(Angle360 == Angle0);

	PolarMath::FAngle Angle90{ 90 };

	EXPECT_NEAR(270, Angle0 - Angle90, PolarMath::ReqPrecision);


}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();


}
