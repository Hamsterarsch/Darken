// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "RTSPlayerEye.generated.h"

enum class ERTSInputState
{
	Idle,
	Placement,
	CameraMouseTurn,
	CameraMouseShuffle,
	PlacementCameraMouseTurn,
	Menu


};

USTRUCT(BlueprintType)
struct FZoomNode
{
	GENERATED_BODY()

public:	
	FZoomNode(float Distance = 300, float Pitch = -30) 
	{
		m_Distance = Distance;
		m_PitchMax =  Pitch;
	};
		
	UPROPERTY(EditAnywhere, DisplayName="Distance")
		float m_Distance;

	UPROPERTY(EditAnywhere, DisplayName="Max Pitch")
		float m_PitchMax;


};

/**
 * 
 */
UCLASS(CustomConstructor)
class LIMES_API ARTSPlayerEye : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	ARTSPlayerEye();
	
	void NotifyNewBuildingPreview(class ABuildingPreview *pNewPreview);

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;

	void InputAddX(float AxisValue);
	void InputAddY(float AxisValue);
	void ActionSelectStart();
	void ActionSelectEnd();
	void ActionContextStart();
	void EnterSeamlessRotation();
	void LeaveSeamlessRotation();

	void AddForwardMovement(float AxisValue);
	void AddRightMovement(float AxisValue);
	void AddCameraYaw(float AxisValue);
	void AddCameraPitch(float AxisValue);
	void ZoomOut();
	void ZoomIn();

	void UpdateCursorRoot();
	void StopBuildingPreview();

	ERTSInputState m_InputState;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent *m_pCameraSpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent *m_pCamera;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="Mouse Shuffle Speed")
		float m_MouseShuffleSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Key Shuffle Speed")
		float m_KeyShuffleSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Mouse Turn Speed")
		float m_MouseTurnSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Key Turn Speed")
		float m_KeyTurnSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Max Camera Pitch")
		float m_CameraMaxPitch;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Min Camera Pitch")
		float m_CameraMinPitch;
	
	UPROPERTY()
		class UGameViewportClient *m_pViewportClient;

	UPROPERTY(EditDefaultsOnly)
		TArray<FZoomNode> m_aZoomNodes;

	UPROPERTY()
		USceneComponent *m_pCursorRoot;

	UPROPERTY()
		class ABuildingPreview *m_pBuildingPreviewCurrent;

	bool m_bBuildingPreviewWasPlacable;
	FVector2D m_MouseShufflePreMousePos;
	float m_ZoomTargetDist;
	float m_ZoomTargetPitch;
	int32 m_ZoomIndex;
	FVector m_SeamlessRotationPrePos;

};
