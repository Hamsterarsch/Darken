// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSpectatorPawn.h"


ARTSSpectatorPawn::ARTSSpectatorPawn(const FObjectInitializer &Initializer):Super(Initializer)
{
	bAddDefaultMovementBindings = false;
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = Initializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("MainCamera"));
	CameraComponent->SetupAttachment(this->GetRootComponent());
	CameraComponent->bUsePawnControlRotation = false;	
	
	
}

void ARTSSpectatorPawn::PostInitProperties()
{
	Super::PostInitProperties();

	ViewAngleRad = FMath::DegreesToRadians(ViewAngle);
	ViewAngleSin = FMath::Sin(ViewAngleRad);
	ViewAngleCos = FMath::Cos(ViewAngleRad);

	CameraComponent->SetRelativeLocationAndRotation(FVector(-CameraRadius * ViewAngleCos, 0, CameraRadius * ViewAngleSin), FQuat(FVector::RightVector, ViewAngleRad));


}

#if WITH_EDITOR
void ARTSSpectatorPawn::PostEditChangeProperty(FPropertyChangedEvent &e)
{
	Super::PostEditChangeProperty(e);
	ViewAngleRad = FMath::DegreesToRadians(ViewAngle);
	ViewAngleSin = FMath::Sin(ViewAngleRad);
	ViewAngleCos = FMath::Cos(ViewAngleRad);

	CameraComponent->SetRelativeLocationAndRotation(FVector(-CameraRadius * ViewAngleCos, 0, CameraRadius * ViewAngleSin), FQuat(FVector::RightVector, ViewAngleRad));

	//prevent messed up property settings
	CameraRadiusMin = CameraRadiusMin > CameraRadiusMax ? CameraRadiusMax : CameraRadiusMin;
	CameraRadius = CameraRadius > CameraRadiusMax ? CameraRadiusMax : CameraRadius;
	CameraRadius = CameraRadiusMin > CameraRadius ? CameraRadiusMin : CameraRadius;
	
		
}
#endif

void ARTSSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	GameViewportClient = GetGameInstance()->GetGameViewportClient();
	check(GameViewportClient);

}

bool ARTSSpectatorPawn::IsMovementAllowed() const { return bIsMovementAllowed; }
void ARTSSpectatorPawn::AllowMovement(bool bIsAllowed) { bIsMovementAllowed = bIsAllowed; }

//Manipulates Spectator Pawn's local offset. 
//The speed depends on the distance to the clicked point, whereas the maximum
//speed is reached if the specified percentage of the smallest viewport dimension is exceeded
//(Part of the Controller/Camera interface)
//@param: Takes in a screen coordinate vector. Specifies the direction of the displacement
void ARTSSpectatorPawn::MoveCamera(FVector2D Direction)
{	
	if (bIsMovementAllowed)
	{
		GameViewportClient->GetViewportSize(ViewportSize);
		Direction /= (FMath::Min(ViewportSize.X,ViewportSize.Y) * MousebasedScrollingScreenPercentage);
		if (Direction.Size() > 1)
		{
			Direction.Normalize();

		}
		Direction *= DeltaSecondsCoefficient;
		AddActorLocalOffset(FVector(-Direction.Y, Direction.X, 0) * CameraMovementSpeed, true);

	}
	

}

void ARTSSpectatorPawn::Tick(float DeltaSeconds)
{
	DeltaSecondsCoefficient = DeltaSeconds;

}

//Manipulates Spectator Pawn's local position
//(Part of the Controller/Camera interface)
//@param: Designated for input axis value. Negative scale produces movement into the opposite direction
void ARTSSpectatorPawn::MoveCameraForward(float AxisValue)
{
	if (AxisValue != 0 && bIsMovementAllowed)
	{
		AddActorLocalOffset(FVector(AxisValue, 0, 0) * CameraMovementSpeed * DeltaSecondsCoefficient, true);
	
	}


}

//Manipulates Spectator Pawn's local position
//(Part of the Controller/Camera interface)
//@param: Designated for input axis value. Negative scale produces movement into the opposite direction
void ARTSSpectatorPawn::MoveCameraRight(float AxisValue)
{
	if (AxisValue != 0 && bIsMovementAllowed)
	{
		AddActorLocalOffset(FVector(0, AxisValue, 0) * CameraMovementSpeed * DeltaSecondsCoefficient, true);

	}


}

//Manipulates the local position of the camera component. Negative axis values will result in a increasing distance
void ARTSSpectatorPawn::IncreaseCameraDistance(float AxisValue)
{
	if (AxisValue > 0 && CameraRadius <= CameraRadiusMax )
	{
		float DisplacementValue = CameraZoomSpeed * AxisValue * DeltaSecondsCoefficient;
		CameraRadius += DisplacementValue;
		CameraComponent->AddRelativeLocation(FVector(-DisplacementValue * ViewAngleCos, 0, DisplacementValue * ViewAngleSin), true);

	}
	else if (AxisValue < 0 && CameraRadius >= CameraRadiusMin)
	{
		float DisplacementValue = CameraZoomSpeed * AxisValue * DeltaSecondsCoefficient;
		CameraRadius += DisplacementValue;
		CameraComponent->AddRelativeLocation(FVector(-DisplacementValue * ViewAngleCos, 0, DisplacementValue * ViewAngleSin), true);

	}


}

//Rotates the Spectator Pawn about the world up axis
//(Part of the Controller/Camera interface)
//@param: Designated for input axis value. Negative scale will result in movement into the opposite direction.
void ARTSSpectatorPawn::RotateCameraRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		AddActorWorldRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(CameraTurnSpeed * AxisValue * DeltaSecondsCoefficient)), true);
	
	}


}
