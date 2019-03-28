// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
#include "IPropertyTypeCustomization.h"
#endif

#include "Daytime.generated.h"

#if WITH_EDITOR
class FDaytimeCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();


	virtual void CustomizeHeader
	(
		TSharedRef<IPropertyHandle> PropertyHandle, 
		FDetailWidgetRow& HeaderRow, 
		IPropertyTypeCustomizationUtils &CustomizationUtils
	)
	override;

	virtual void CustomizeChildren
	(
		TSharedRef<IPropertyHandle> PropertyHandle,
		IDetailChildrenBuilder &ChildBuilder, 
		IPropertyTypeCustomizationUtils& CustomizationUtils
	)
	override;


private:



};
#endif

USTRUCT()
struct LIMES_API FDaytime
{
	GENERATED_BODY()
	
public:
	FDaytime() = default;

	FDaytime(float Seconds);

	FDaytime(int32 Days, int32 Hours);
	
	float ToSeconds() const;

	explicit operator float() const;


	static constexpr int32 GetHours(float FromSeconds);


	UPROPERTY(EditAnywhere, Meta=(ClampMin="0"))
		int32 Day;

	UPROPERTY(EditAnywhere, Meta=(ClampMin="0", ClampMax="24"))
		int32 Hour;

	
	static constexpr float s_DayToMinutesCoeff{ 4 };

	static constexpr float s_HourInSeconds{ (s_DayToMinutesCoeff * 60.f) / 24 };
	   
	
};

constexpr int32 FDaytime::GetHours(float FromSeconds)
{
	return static_cast<int32>(FromSeconds / s_HourInSeconds);


}

bool operator<(const FDaytime &Lhs, const FDaytime &Rhs);

bool operator>(const FDaytime &Lhs, const FDaytime &Rhs);

bool operator<=(const FDaytime &Lhs, const FDaytime &Rhs);

bool operator>=(const FDaytime &Lhs, const FDaytime &Rhs);

