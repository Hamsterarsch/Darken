// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SphericalCoords.generated.h"

USTRUCT(BlueprintType)
struct FRadialCoords
{
	GENERATED_BODY()
			
public:
	//UFUNCTION(BlueprintCallable)
	/*	void AddRange(const float RangeToAdd)
		{
			m_Radius + RangeToAdd;

		}*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RadialCoords", DisplayName= "Azimut")
		float m_Azimut{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RadialCoords", DisplayName = "Radius")
		float m_Radius{ 0 };




};


/**
 * 
 */
UCLASS()
class LIMES_API USphericalCoords : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		static FRadialCoords CartesianToRadial(const FVector &ToConvert)
		{
			FRadialCoords OutRadial{};

			auto Normalized = ToConvert.GetSafeNormal2D();			
			OutRadial.m_Azimut = FMath::RadiansToDegrees(FMath::Atan2(Normalized.Y, Normalized.X));
			OutRadial.m_Azimut += OutRadial.m_Azimut < 0 ? 360 : 0;
			OutRadial.m_Radius = ToConvert.Size2D();

			if (FMath::IsNaN(OutRadial.m_Azimut) || FMath::IsNaN(OutRadial.m_Radius))
			{
				UE_LOG(LogTemp, Error, TEXT("ToRadial fail"));

			}

			return OutRadial;


		}

	UFUNCTION(BlueprintCallable)
		static FVector RadialToCartesian(const FRadialCoords &ToConvert)
		{
			FVector OutCartesian{ 0,0,0 };
			auto AsRadians = FMath::DegreesToRadians(ToConvert.m_Azimut);
			

			OutCartesian.X = ToConvert.m_Radius * FMath::Cos(AsRadians);
			OutCartesian.Y = ToConvert.m_Radius * FMath::Sin(AsRadians);

			if (OutCartesian.ContainsNaN())
			{
				UE_LOG(LogTemp, Error, TEXT("ToCartesian fail: %s, input: az: %f, radius: %f"), *OutCartesian.ToString(), ToConvert.m_Azimut, ToConvert.m_Radius);

			}

			return OutCartesian;


		}

	UFUNCTION(BlueprintPure, Meta = (DisplayName= "Radial Coords To Rotator", CompactNodeTitle= "->", BlueprintAutocast))
		static FRotator Conv_RadialCoordsToRotator(const FRadialCoords &InRadialCoords)
		{
			return FRotator{ 0, InRadialCoords.m_Azimut, 0 };

		}

	UFUNCTION(BlueprintPure, Meta = (DisplayName= "Radial Coords To Vector", CompactNodeTitle="->", BlueprintAutocast))
		static FVector Conv_RadialCoordsToVector(const FRadialCoords &InRadialCoords)
		{	
			return RadialToCartesian(InRadialCoords);

		}

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Vector To RadialCoords", CompactNodeTitle = "->", BlueprintAutocast))
		static FRadialCoords Conv_VectorToRadialCoords(const FVector &InVector)
	{
		return CartesianToRadial(InVector);

	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FRadialCoords RotateBy(FRadialCoords Target, float Angle)
		{	
			Target.m_Azimut += Angle;
			return Target;

		}





	
	
};
