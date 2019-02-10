#include "pch.h"
#include "gtest/gtest.h"
#include "../../hdr/RadialTransform.h"

#include <iostream>


TEST(TransformTests, ToParent_Arbitrary)
{
	//Arbitrary case

	PolarMath::SPolarPoint SubFactory{ 2, 240 };
	PolarMath::SPolarPoint RelativePoint{ 1, 15 };

	auto PointRootRelative{ SubFactory.InverseTransform(RelativePoint) };

	EXPECT_NEAR(14.051913114226, PointRootRelative.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(1.06597218296, PointRootRelative.Radius, PolarMath::ReqPrecision);
		   	 

}

TEST(TransformTests, ToParent_Collinear)
{
	PolarMath::SPolarPoint SubFactory{ 4, 180 };
	PolarMath::SPolarPoint RelativePoint{ 2, 180 };

	auto PointRootRelative{ SubFactory.InverseTransform(RelativePoint) };

	EXPECT_NEAR(180, PointRootRelative.Angle, PolarMath::ReqPrecision);
	EXPECT_NEAR(6, PointRootRelative.Radius, PolarMath::ReqPrecision);
	

}

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
	EXPECT_FALSE(Col_90_10.DoCollidersIntersect(Col_45_5));
	EXPECT_FALSE(Col_45_5.DoCollidersIntersect(Col_90_10));

	//Self intersection 
	EXPECT_TRUE(Col_90_10.DoCollidersIntersect(Col_90_10));
	EXPECT_TRUE(Col_45_5.DoCollidersIntersect(Col_45_5));

	//One nonfractional unit difference
	PolarMath::SPolarPoint Center32{ UniformRadius, 32 };
	PolarMath::CPolarCollider Col_32_8{ Center32, 8, UniformDepth };

	EXPECT_TRUE(Col_32_8.DoCollidersIntersect(Col_45_5));

	//Wrapping independence
	PolarMath::SPolarPoint Center359{ UniformRadius, 359 };
	PolarMath::CPolarCollider Col_359_11{ Center359, 11, UniformDepth };

	PolarMath::SPolarPoint Center14{ UniformRadius, 14 };
	PolarMath::CPolarCollider Col_14_13{ Center14, 13, UniformDepth };

		//2nd without wrapping
	EXPECT_TRUE(Col_359_11.DoCollidersIntersect(Col_14_13));

	PolarMath::CPolarCollider Col_14_20{ Center14, 20, UniformDepth };

	EXPECT_TRUE(Col_359_11.DoCollidersIntersect(Col_14_20));


}

TEST(CollisionTests, CommonCases)
{
	{
		PolarMath::SPolarPoint Center90_R4{ 4, 90 };
		PolarMath::SPolarPoint Center96_R1{ 1, 96 };

		PolarMath::CPolarCollider Col_90_R4_10_D2{ Center90_R4, 10, 2 };
		PolarMath::CPolarCollider Col_96_R1_14_D3{ Center96_R1, 14, 3 };

		EXPECT_TRUE(Col_90_R4_10_D2.DoCollidersIntersect(Col_96_R1_14_D3));

	}

	{
		PolarMath::SPolarPoint Center1{ 18.42, 77.4 };
		PolarMath::SPolarPoint Center2{ 5.222211, 5 };

		PolarMath::CPolarCollider Col_1{ Center1, 180, 2 };
		PolarMath::CPolarCollider Col_2{ Center2, 14, 3 };

		EXPECT_FALSE(Col_1.DoCollidersIntersect(Col_2));

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
