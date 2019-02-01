#include "pch.h"
#include "gtest/gtest.h"
#include "../RadialMath/hdr/RadialTransform.h"

#include <iostream>

void Print(int Number)
{
	std::cout << "\nGlobal function invocation: " << Number;

}

class TestClass
{
public:
	void Print(int Number)
	{
		std::cout << "\nMember function invocation: " << Number + m_Summand;

	}

	TestClass(int Summand) : 
		m_Summand{Summand}
	{
	}

	int m_Summand;


};

TEST(DelegateTests, AddRemoval) 
{
	auto DelegateToGlobal = RadialMath::TDelegate<void(int)>::Create<&Print>();

	auto DelegateClassTestInstance = TestClass{3};
	
	//Member destruction invokes exception
	auto DelegateToMember = RadialMath::TDelegate<void(int)>::Create<TestClass, &TestClass::Print>(&DelegateClassTestInstance);

	DelegateToGlobal(19);
	DelegateToMember(3);

	RadialMath::TMultiDelegate<void(int)> TestEvent{};
	TestEvent.Add(DelegateToMember);
	TestEvent.Add(DelegateToGlobal);
	
	std::cout << "\nBroadcast:\n";
	TestEvent.Broadcast(7);
	std::cout << "\n";



}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}
