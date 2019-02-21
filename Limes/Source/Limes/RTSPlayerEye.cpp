// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerEye.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "RTSPlayerController.h"
#include "Limes.h"
#include "BuildingPreview.h"
#include "RTSGameInstance.h"
#include "RTSStructureFactory.h"


const FName ARTSPlayerEye::s_AxisMouseX{ TEXT("MouseX") };
const FName ARTSPlayerEye::s_AxisMouseY{ TEXT("MouseY") };

//Public--------------

ARTSPlayerEye::ARTSPlayerEye() :
	m_MouseShuffleSpeed{ 1 },
	m_MouseTurnSpeed{ 1 },
	m_KeyShuffleSpeed{ 1 },
	m_CameraMaxPitch{ 90 },
	m_CameraMinPitch{ 0 },
	m_ZoomTargetPitch{ -30 },
	m_ZoomTargetDist{ 300 },
	m_bBuildingPreviewWasPlacable{ false },
	m_CameraState{ this },
	m_PlacementState{ this }
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	GetMovementComponent()->SetUpdatedComponent(GetRootComponent());

	GetCollisionComponent()->SetRelativeLocation({ 0,0,0 });

	m_pCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	m_pCameraSpringArm->SetupAttachment(GetRootComponent());
	m_pCameraSpringArm->SetRelativeLocation({ 0,0,0 });
	m_pCameraSpringArm->SetWorldRotation(FRotator{ -30, 0, 0 });

	m_pCameraSpringArm->bEditableWhenInherited = true;
	m_pCameraSpringArm->bEnableCameraLag = true;
	m_pCameraSpringArm->bEnableCameraRotationLag = true;

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_pCamera->SetupAttachment(m_pCameraSpringArm);
	m_pCamera->bEditableWhenInherited = true;

	m_aZoomNodes.Add(FZoomNode{ 300, 30 });

	m_pCursorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CursorRoot"));
	m_pCursorRoot->SetupAttachment(GetRootComponent());
	

}

void ARTSPlayerEye::NotifyNewBuildingPreview(class ABuildingPreview *pNewPreview, class ARTSStructureFactory *pFactory)
{
	DiscardBuildingPreview();
	UE_LOG(RTS_StructurePlacement, Log, TEXT("OnNewPreviewBuilding"));
	
	m_pBuildingPreviewCurrent = pNewPreview;
	m_pBuildingPreviewCurrent->AttachToComponent(m_pCursorRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	m_pCurrentTargetFactory = pFactory;
	m_PlacementState.HandleInput(EAbstractInputEvent::PlaceBuilding_Start);
	

}

#pragma region Input
void ARTSPlayerEye::AddForwardMovement(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}
	auto Forward = m_pCameraSpringArm->GetForwardVector();
	//todo: delta time
	AddActorWorldOffset(Forward.GetSafeNormal2D() * AxisValue * m_KeyShuffleSpeed);




}

void ARTSPlayerEye::AddRightMovement(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}
	auto Right = m_pCameraSpringArm->GetRightVector();
	//todo: delta time
	AddActorWorldOffset(Right.GetSafeNormal2D() * AxisValue * m_KeyShuffleSpeed);
	   

}

void ARTSPlayerEye::AddForwardMovementFromMouse(float AxisValue)
{
	auto Forward = m_pCameraSpringArm->GetForwardVector();
	//Mouse based shuffle (todo: delta time)
	AddActorWorldOffset(Forward.GetSafeNormal2D() * -AxisValue * m_MouseShuffleSpeed * m_pCamera->AspectRatio);
	

}

void ARTSPlayerEye::AddRightMovementFromMouse(float AxisValue)
{
	auto Right = m_pCameraSpringArm->GetRightVector();
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}
	AddActorWorldOffset(Right.GetSafeNormal2D() * -AxisValue * m_MouseShuffleSpeed);

}

void ARTSPlayerEye::AddCameraYaw(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}
	m_pCameraSpringArm->AddRelativeRotation(FRotator{ 0, AxisValue * m_KeyTurnSpeed, 0 });


}

void ARTSPlayerEye::AddCameraYawFromMouse(float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}
	m_pCameraSpringArm->AddRelativeRotation(FRotator{ 0, AxisValue * m_MouseTurnSpeed, 0 });


}

void ARTSPlayerEye::AddCameraPitchFromMouse(float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	auto SummedPitch = m_pCameraSpringArm->RelativeRotation.Pitch + AxisValue * m_MouseTurnSpeed * m_pCamera->AspectRatio;
	if (SummedPitch > -m_CameraMaxPitch && SummedPitch < -m_CameraMinPitch)
	{
		m_pCameraSpringArm->RelativeRotation.Pitch = SummedPitch;
	}


}

void ARTSPlayerEye::ZoomOut()
{
	if (m_ZoomIndex < (m_aZoomNodes.Num() - 1))
	{
		++m_ZoomIndex;
		auto RelativeRot = m_pCameraSpringArm->RelativeRotation;
		if (RelativeRot.Pitch > -m_aZoomNodes[m_ZoomIndex].m_PitchMax)
		{
			m_pCameraSpringArm->SetRelativeRotation(FRotator{ -m_aZoomNodes[m_ZoomIndex].m_PitchMax, m_pCameraSpringArm->RelativeRotation.Yaw, 0 });

		}


	}


}

void ARTSPlayerEye::ZoomIn()
{
	if (m_ZoomIndex > 0)
	{
		--m_ZoomIndex;
		auto RelativeRot = m_pCameraSpringArm->RelativeRotation;
		if (RelativeRot.Pitch < -m_aZoomNodes[m_ZoomIndex].m_PitchMax)
		{
			m_pCameraSpringArm->SetRelativeRotation(FRotator{ -m_aZoomNodes[m_ZoomIndex].m_PitchMax, RelativeRot.Yaw, 0 });

		}


	}


}

void ARTSPlayerEye::SetPreviewCursorPosWs(const FVector &NewPos)
{
	m_pCursorRoot->SetWorldLocation(NewPos);


}

void ARTSPlayerEye::UpdatePreviewCursorPos()
{
	auto *pCtrl = Cast<APlayerController>(GetController());
	if (!pCtrl || !m_pCurrentTargetFactory)
	{
		return;
	}

	//Hit queries --		THIS SHOULD BE REPLACED WITH POLAR COLLISION FUNCTIONS		--
	FHitResult LayerPlaceableHit;
	pCtrl->GetHitResultUnderCursor(ARTSPlayerEye::s_CollisionLayerPlacable, false, LayerPlaceableHit);
	if (LayerPlaceableHit.IsValidBlockingHit())
	{
		auto GridPosition{ m_pCurrentTargetFactory->Discretize(LayerPlaceableHit.Location) };
		m_pCursorRoot->SetWorldLocation(GridPosition);
	}


}

void ARTSPlayerEye::UpdateBuildingPreviewProperties()
{
	auto *pCtrl = Cast<APlayerController>(GetController());
	if (!pCtrl || !m_pCurrentTargetFactory)
	{
		return;
	}

	auto *pGInst = Cast<URTSGameInstance>(GetGameInstance());
	if (!pGInst)
	{
		return;

	}

	//Update cursor root rotation
	auto NewRot{ (m_pCurrentTargetFactory->GetActorLocation() - m_pBuildingPreviewCurrent->GetActorLocation()).GetSafeNormal2D().ToOrientationQuat() };
	m_pBuildingPreviewCurrent->SetActorRotation(NewRot);

	//If placement loacation is occluded -- USE POLAR METHODS ---
	FHitResult LayerNonPlacableHit;
	pCtrl->GetHitResultUnderCursor(s_CollisionLayerNonPlacable, false, LayerNonPlacableHit);
	if (LayerNonPlacableHit.IsValidBlockingHit() || !pGInst->GetMainStructureFactory()->IsPlacableAtPosition(m_pBuildingPreviewCurrent))
	{
		UE_LOG(RTS_InputDebug, VeryVerbose, TEXT("Non-placable cursor hit result: %s"), *LayerNonPlacableHit.GetActor()->GetName());
		//first occluded frame
		if (m_bBuildingPreviewWasPlacable)
		{
			//defer feedback to actor
			m_pBuildingPreviewCurrent->NotifyNonPlacable();
			m_bBuildingPreviewWasPlacable = false;

		}

	}
	else
	{
		//first unoccluded frame
		if (!m_bBuildingPreviewWasPlacable)
		{
			//defer feedback to actor
			m_pBuildingPreviewCurrent->NotifyPlacable();
			m_bBuildingPreviewWasPlacable = true;

		}

	}


}

bool ARTSPlayerEye::TryCommitBuildingPreview()
{
	m_pBuildingPreviewCurrent->SetActorLocation(m_pCursorRoot->GetComponentLocation());
	return m_pCurrentTargetFactory->TryCommitPreviewBuilding(m_pBuildingPreviewCurrent);


}

void ARTSPlayerEye::DiscardBuildingPreview()
{
	UE_LOG(RTS_StructurePlacement, Log, TEXT("Stopping building preview"));
	if (m_pBuildingPreviewCurrent)
	{
		m_pBuildingPreviewCurrent->Destroy();

	}
	//Set to unplacable so a new building is checked in the first frame
	m_bBuildingPreviewWasPlacable = false;


}

#pragma endregion


//Protected------------------

void ARTSPlayerEye::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//m_pViewportClient = GetGameInstance()->GetGameViewportClient();
	//check(m_pViewportClient);
	//only on begin play
	

}

void ARTSPlayerEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_pCameraSpringArm->TargetArmLength = FMath::FInterpTo(m_pCameraSpringArm->TargetArmLength, m_aZoomNodes[m_ZoomIndex].m_Distance, DeltaTime, 5);

	m_CameraState.Update();
	m_PlacementState.Update();

	//UpdateCursorRoot();

}

void ARTSPlayerEye::BeginPlay()
{
	Super::BeginPlay();


}


#pragma region Input
void ARTSPlayerEye::SetupPlayerInputComponent(UInputComponent *InputComponent)
{
	//Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(s_AxisMouseX);
	InputComponent->BindAxis(s_AxisMouseY);

	InputComponent->BindAxis(TEXT("MoveX"), this, &ARTSPlayerEye::AddRightMovement);
	InputComponent->BindAxis(TEXT("MoveY"), this, &ARTSPlayerEye::AddForwardMovement);

	InputComponent->BindAxis(TEXT("RotateCamera"), this, &ARTSPlayerEye::AddCameraYaw);

	InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &ARTSPlayerEye::ActionSelectStart);
	InputComponent->BindAction(TEXT("Select"), IE_Released, this, &ARTSPlayerEye::ActionSelectEnd);

	InputComponent->BindAction(TEXT("ContextAction"), IE_Pressed, this, &ARTSPlayerEye::ActionContextStart);
	InputComponent->BindAction(TEXT("ContextAction"), IE_Released, this, &ARTSPlayerEye::ActionContextEnd);

	InputComponent->BindAction(TEXT("SeamlessRotate"), IE_Pressed, this, &ARTSPlayerEye::EnterSeamlessRotation);
	InputComponent->BindAction(TEXT("SeamlessRotate"), IE_Released, this, &ARTSPlayerEye::LeaveSeamlessRotation);
	
	InputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &ARTSPlayerEye::ZoomOut);
	InputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &ARTSPlayerEye::ZoomIn);

}

void ARTSPlayerEye::ActionSelectStart()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionSelect_Start);
	m_PlacementState.HandleInput(EAbstractInputEvent::ActionSelect_Start);

}

void ARTSPlayerEye::ActionSelectEnd()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionSelect_End);
	m_PlacementState.HandleInput(EAbstractInputEvent::ActionSelect_End);

}

void ARTSPlayerEye::ActionContextStart()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionContext_Start);
	m_PlacementState.HandleInput(EAbstractInputEvent::ActionContext_Start);


}

void ARTSPlayerEye::ActionContextEnd()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionContext_End);
	m_PlacementState.HandleInput(EAbstractInputEvent::ActionContext_End);


}

void ARTSPlayerEye::EnterSeamlessRotation()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionRotate_Start);


}

void ARTSPlayerEye::LeaveSeamlessRotation()
{
	m_CameraState.HandleInput(EAbstractInputEvent::ActionRotate_End);


}
#pragma endregion
