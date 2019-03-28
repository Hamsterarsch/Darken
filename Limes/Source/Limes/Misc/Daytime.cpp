// Fill out your copyright notice in the Description page of Project Settings.

#include "Daytime.h"


#pragma region EditorStyle
#if WITH_EDITOR
#include "PropertyEditing.h"
#include "PropertyHandle.h"

#include "SGridPanel.h"
#include "SBox.h"
#include "SScaleBox.h"
#include "STextBlock.h"
#include "SNumericEntryBox.h"
#include "ObjectEditorUtils.h"


#define LOCTEXT_NAMESPACE "DaytimeCustomization"

TSharedRef<IPropertyTypeCustomization> FDaytimeCustomization::MakeInstance()
{
	return MakeShareable(new FDaytimeCustomization());


}

void FDaytimeCustomization::CustomizeHeader
(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow &HeaderRow,
	IPropertyTypeCustomizationUtils &CustomizationUtils
)
{
}

void FDaytimeCustomization::CustomizeChildren
(
	TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder &ChildBuilder, 
	IPropertyTypeCustomizationUtils &CustomizationUtils
)
{
	auto DayHandle{ PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDaytime, Day)) };
	auto HourHandle{ PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDaytime, Hour)) };

	if(!DayHandle->IsValidHandle() || !HourHandle->IsValidHandle() || !PropertyHandle->IsValidHandle())
	{
		return;
	}

	if(PropertyHandle->GetNumPerObjectValues() != 1)
	{
		return;
	}
		
	auto &Row{ ChildBuilder.AddCustomRow(FText::FromString(TEXT("Content"))) };
		
	//Desc text
	Row.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Daytime")))
		];

	//Content
	//layout grid
	auto MainBox{ SNew(SHorizontalBox) };
	Row.ValueContent()
		[
			MainBox
		];

	float DayVal;
	DayHandle->GetValue(DayVal);

	int32 DayMinVal;
	TTypeFromString<int32>::FromString(DayMinVal, *DayHandle->GetProperty()->GetMetaData(TEXT("ClampMin")));
	
	MainBox->AddSlot()
		[
			SNew(SNumericEntryBox<float>)
				.MinSliderValue(DayMinVal)
				.AllowSpin(true)
				.Value_Lambda([hDayProperty = DayHandle]()
				{
					int32 Val{0};
					if(hDayProperty->GetNumPerObjectValues() == 1)
					{
						FString SVal{};
						hDayProperty->GetPerObjectValue(0, SVal);
						TTypeFromString<int32>::FromString(Val, *SVal);
					}
					return TOptional<float>{static_cast<float>(Val)};


				})
				.LabelPadding(0)
				.OnValueCommitted_Lambda([hDayProperty = DayHandle](float NewVal, ETextCommit::Type Type)
				{					
					if(hDayProperty->IsValidHandle())
					{
						if(hDayProperty->GetNumPerObjectValues() == 1)
						{
							//hDayProperty->SetPerObjectValue(0, FString::FromInt(static_cast<int32>(NewVal)));

							FString SVal{};
							hDayProperty->GetPerObjectValue(0, SVal);
							UE_LOG(LogTemp, Log, TEXT("input val: %f new set value %s"), NewVal, *SVal);							
						}
					}


				})
				.Label()
				[
					SNumericEntryBox<float>::BuildLabel(FText::FromString(TEXT("Day")), FLinearColor::Black, FLinearColor::White)
				]

		];


	//Hour
	float HourVal;
	HourHandle->GetValue(HourVal);

	int32 HourMaxVal, HourMinVal;
	TTypeFromString<int32>::FromString(HourMinVal, *HourHandle->GetProperty()->GetMetaData(TEXT("ClampMin")));
	TTypeFromString<int32>::FromString(HourMaxVal, *HourHandle->GetProperty()->GetMetaData(TEXT("ClampMax")));

	MainBox->AddSlot()
		[
			SNew(SNumericEntryBox<float>)
				.MaxSliderValue(HourMaxVal)
				.MinSliderValue(HourMinVal)
				.AllowSpin(true)
				.Value(HourVal)
				.OnValueChanged_Lambda([hHourProperty = HourHandle](float NewVal)
				{
					if(hHourProperty.IsValid())
					{
						hHourProperty->SetValue(NewVal);
					}
				})
				.Label()
				[
					SNumericEntryBox<float>::BuildLabel(FText::FromString(TEXT("Hour")), FLinearColor::Black, FLinearColor::White)
				]

		];

	
}

#undef LOCTEXT_NAMESPACE
#endif
#pragma endregion

FDaytime::FDaytime(float Seconds)
{
	auto Minutes{ Seconds / 60 };
	auto AsDays{ Minutes / s_DayToMinutesCoeff };

	Day = static_cast<int32>(AsDays);
	Hour = static_cast<int32>(FMath::Frac(AsDays) * 24);
	

}

FDaytime::FDaytime(int32 Days, int32 Hours) :
	Day{ std::abs(Days) },
	Hour{ std::abs(Hours) }
{
}

float FDaytime::ToSeconds() const
{
	return Day * s_DayToMinutesCoeff * 60 + s_DayToMinutesCoeff * (60 / 24.f) * Hour;


}

FDaytime::operator float() const
{
	return ToSeconds();

}

bool operator<(const FDaytime& Lhs, const FDaytime& Rhs)
{
	return Lhs.ToSeconds() < Rhs.ToSeconds();


}

bool operator>(const FDaytime& Lhs, const FDaytime& Rhs)
{
	return !(Lhs < Rhs);


}

bool operator<=(const FDaytime& Lhs, const FDaytime& Rhs)
{
	return 		Lhs.Day		<= Rhs.Day 
			&&	Lhs.Hour	<= Rhs.Hour;


}

bool operator>=(const FDaytime& Lhs, const FDaytime& Rhs)
{
	return 		Lhs.Day		>= Rhs.Day 
			&&	Lhs.Hour	>= Rhs.Hour;


}
