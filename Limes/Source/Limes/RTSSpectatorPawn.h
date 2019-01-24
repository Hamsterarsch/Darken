// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "RTSSpectatorPawn.generated.h"


/**
 * 
 */
class AMousePlayerController;

UCLASS()
class LIMES_API ARTSSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	void IncreaseCameraDistance(float AxisValue);
	void RotateCameraRight(float AxisValue);
	void MoveCamera(FVector2D Direction);
	void MoveCameraRight(float AxisValue);
	void MoveCameraForward(float AxisValue);
	void AllowMovement(bool bIsAllowed);//make blueprint callable
	bool IsMovementAllowed() const;
	ARTSSpectatorPawn(const class FObjectInitializer &Initializer);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "0.", ClampMax = "90."))
		float ViewAngle = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin="0.1", DisplayName ="Initial Camera Radius"))
		float CameraRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "1."))
		float CameraRadiusMax = 1700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "1."))
		float CameraRadiusMin = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "1.", DisplayName = "Max Camera Movement Speed"))
		float CameraMovementSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "1."))
		float CameraZoomSpeed = 1250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "1.", ClampMax = "500."))
		float CameraTurnSpeed = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (ClampMin = "0.05", ClampMax = "0.5"))
		float MousebasedScrollingScreenPercentage = 0.3;

	bool bIsMovementAllowed = true;
	float ViewAngleRad;
	float ViewAngleSin = FMath::Sin(FMath::DegreesToRadians(ViewAngle)), ViewAngleCos = FMath::Cos(FMath::DegreesToRadians(ViewAngle));
	float DeltaSecondsCoefficient;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent *CameraComponent;

	UGameViewportClient *GameViewportClient;
	FVector2D ViewportSize;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent &e) override;
	
	
};
