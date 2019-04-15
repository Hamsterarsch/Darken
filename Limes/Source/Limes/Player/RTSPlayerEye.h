// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "CameraState.h"
#include "PlacementState.h"
#include "RTSPlayerEye.generated.h"

enum class EAbstractInputEvent : size_t
{
	ActionSelect_Start,
	ActionSelect_End,
	ActionContext_Start,
	ActionContext_End,
	ActionRotate_Start,
	ActionRotate_End,
	PlaceBuilding_Start,
	PlaceInfra_Start,
	PlayeInfra_End


};

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
	
	void NotifyNewBuildingPreview(class ABuildingPreview *pNewPreview, class ARTSStructureFactory *pFactory);

	void AddForwardMovement(float AxisValue);

	void AddRightMovement(float AxisValue);

	void AddForwardMovementFromMouse(float AxisValue);

	void AddRightMovementFromMouse(float AxisValue);

	void AddCameraYaw(float AxisValue);

	void AddCameraYawFromMouse(float AxisValue);

	void AddCameraPitchFromMouse(float AxisValue);

	void ZoomOut();

	void ZoomIn();


	void SetPreviewCursorPosWs(const FVector &NewPos);

	void UpdatePreviewCursorPos();

	void UpdateBuildingPreviewProperties();

	bool TryCommitBuildingPreview();

	void DiscardBuildingPreview();

	void ShowMenuItemOnClick();


	const static FName s_AxisMouseX;

	const static FName s_AxisMouseY;

	constexpr static ECollisionChannel s_CollisionLayerPlaceable{ ECC_GameTraceChannel1 };

	constexpr static ECollisionChannel s_CollisionLayerNonPlaceable{ ECC_GameTraceChannel2 };


protected:
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(UInputComponent *pInputComponent) override;

	void ActionSelectStart();

	void ActionSelectEnd();

	void ActionContextStart();

	void ActionContextEnd();

	void EnterSeamlessRotation();

	void LeaveSeamlessRotation();

	
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent *m_pCameraSpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent *m_pCamera;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="Mouse Shuffle Speed", Category="Controls")
		float m_MouseShuffleSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Key Shuffle Speed", Category="Controls")
		float m_KeyShuffleSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Mouse Turn Speed", Category="Controls")
		float m_MouseTurnSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Key Turn Speed", Category="Controls")
		float m_KeyTurnSpeed;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Max Camera Pitch", Category="Controls")
		float m_CameraMaxPitch;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Min Camera Pitch", Category="Controls")
		float m_CameraMinPitch;
	
	UPROPERTY(EditDefaultsOnly, Category="Controls")
		TArray<FZoomNode> m_aZoomNodes;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UBuildingWidgetBase> m_BuildingWidgetClass;

	UPROPERTY()
		class UGameViewportClient *m_pViewportClient;

	UPROPERTY()
		USceneComponent *m_pCursorRoot;

	UPROPERTY()
		class ABuildingPreview *m_pBuildingPreviewCurrent;

	UPROPERTY()
		class ARTSStructureFactory *m_pCurrentTargetFactory;

	UPROPERTY()
		class UBuildingWidgetBase *m_pBuildingWidget;

	bool m_bBuildingPreviewWasPlacable;
	FVector2D m_MouseShufflePreMousePos;
	float m_ZoomTargetDist;
	float m_ZoomTargetPitch;
	int32 m_ZoomIndex;
	FVector m_SeamlessRotationPrePos;
	FVector m_CursorLastPosition;

	CCameraStateMachine m_CameraState;
	CPlacementStateMachine m_PlacementState;



};
