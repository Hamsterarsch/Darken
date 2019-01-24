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
#include "BuildingFactory.h"


//Public--------------

ARTSPlayerEye::ARTSPlayerEye() :
	m_MouseShuffleSpeed{ 1 },
	m_MouseTurnSpeed{ 1 },
	m_KeyShuffleSpeed{ 1 },
	m_CameraMaxPitch{ 90 },
	m_CameraMinPitch{ 0 },
	m_ZoomTargetPitch{ -30 },
	m_ZoomTargetDist{ 300 },
	m_bBuildingPreviewWasPlacable{ false }
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

void ARTSPlayerEye::NotifyNewBuildingPreview(class ABuildingPreview *pNewPreview)
{
	StopBuildingPreview();
	UE_LOG(RTS_StructurePlacement, Log, TEXT("OnNewPreviewBuilding"));

	m_pBuildingPreviewCurrent = pNewPreview;
	m_InputState = ERTSInputState::Placement;
	m_pBuildingPreviewCurrent->AttachToComponent(m_pCursorRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


}

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

	UpdateCursorRoot();

}

void ARTSPlayerEye::BeginPlay()
{
	Super::BeginPlay();


}


#pragma region Input
void ARTSPlayerEye::SetupPlayerInputComponent(UInputComponent *InputComponent)
{
	//Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(TEXT("MouseX"), this, &ARTSPlayerEye::InputAddX);
	InputComponent->BindAxis(TEXT("MouseY"), this, &ARTSPlayerEye::InputAddY);

	InputComponent->BindAxis(TEXT("MoveX"), this, &ARTSPlayerEye::AddRightMovement);
	InputComponent->BindAxis(TEXT("MoveY"), this, &ARTSPlayerEye::AddForwardMovement);

	InputComponent->BindAxis(TEXT("RotateCamera"), this, &ARTSPlayerEye::AddCameraYaw);

	InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &ARTSPlayerEye::ActionSelectStart);
	InputComponent->BindAction(TEXT("Select"), IE_Released, this, &ARTSPlayerEye::ActionSelectEnd);

	InputComponent->BindAction(TEXT("ContextAction"), IE_Pressed, this, &ARTSPlayerEye::ActionContextStart);

	InputComponent->BindAction(TEXT("SeamlessRotate"), IE_Pressed, this, &ARTSPlayerEye::EnterSeamlessRotation);
	InputComponent->BindAction(TEXT("SeamlessRotate"), IE_Released, this, &ARTSPlayerEye::LeaveSeamlessRotation);
	
	InputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &ARTSPlayerEye::ZoomOut);
	InputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &ARTSPlayerEye::ZoomIn);

}

void ARTSPlayerEye::InputAddX(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	switch (m_InputState)
	{
	case ERTSInputState::CameraMouseShuffle:
		AddRightMovement(AxisValue); 
		break;
	case ERTSInputState::CameraMouseTurn:
		if (auto *pController = Cast<ARTSPlayerController>(GetController()))
		{
			pController->SetMouseLocation(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);

		}
		AddCameraYaw(AxisValue);
		break;
	case ERTSInputState::PlacementCameraMouseTurn:
		if (auto *pController = Cast<ARTSPlayerController>(GetController()))
		{
			pController->SetMouseLocation(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);

		}
		AddCameraYaw(AxisValue);
		break;

	}

	


}

void ARTSPlayerEye::InputAddY(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	switch (m_InputState)
	{
	case ERTSInputState::CameraMouseShuffle:
		AddForwardMovement(AxisValue);
		break;
	case ERTSInputState::CameraMouseTurn:
		if (auto *pController = Cast<ARTSPlayerController>(GetController()))
		{
			pController->SetMouseLocation(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);

		}
		AddCameraPitch(AxisValue);
		break;
	case ERTSInputState::PlacementCameraMouseTurn:
		if (auto *pController = Cast<ARTSPlayerController>(GetController()))
		{
			pController->SetMouseLocation(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);

		}
		AddCameraPitch(AxisValue);
		break;

	}


}

void ARTSPlayerEye::ActionSelectStart()
{
	switch (m_InputState)
	{
	case ERTSInputState::Idle:
		m_InputState = ERTSInputState::CameraMouseShuffle;
		break;
	//Placing a building while seamless-rotating the camera is unsupported
	case ERTSInputState::Placement:
		if (auto *pGameInstance = Cast<URTSGameInstance>(GetGameInstance()))
		{
			pGameInstance->GetMainStructureFactory()->CommitPreviewBuilding(m_pBuildingPreviewCurrent);
			StopBuildingPreview();

		}
		break;

	}


}

void ARTSPlayerEye::ActionSelectEnd()
{
	switch (m_InputState)
	{
	case ERTSInputState::CameraMouseShuffle:
		m_InputState = ERTSInputState::Idle;
		break;

	}	


}

void ARTSPlayerEye::ActionContextStart()
{
	switch (m_InputState)
	{
	case ERTSInputState::Placement:
		StopBuildingPreview();
		break;
	case ERTSInputState::PlacementCameraMouseTurn:
		StopBuildingPreview();
		break;

	}


}

void ARTSPlayerEye::EnterSeamlessRotation()
{
	if (auto *pController = Cast<ARTSPlayerController>(GetController()))
	{
		pController->GetMousePosition(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);
		//pController->HideMouseCursor(); //hiding cursor needs an additional click into the game to capture following click events (fixable ?)

	}
	UE_LOG(RTS_InputDebug, Log, TEXT("Entering seamless rotation"));
	m_InputState = m_InputState == ERTSInputState::Placement ? ERTSInputState::PlacementCameraMouseTurn : ERTSInputState::CameraMouseTurn;

}

void ARTSPlayerEye::LeaveSeamlessRotation()
{
	if (auto *pController = Cast<ARTSPlayerController>(GetController()))
	{
		pController->SetMouseLocation(m_SeamlessRotationPrePos.X, m_SeamlessRotationPrePos.Y);
		//pController->ShowMouseCursor();

	}
	UE_LOG(RTS_InputDebug, Log, TEXT("Leaving seamless rotation"));
	m_InputState = m_InputState == ERTSInputState::PlacementCameraMouseTurn ? ERTSInputState::Placement : ERTSInputState::Idle;

}

void ARTSPlayerEye::AddForwardMovement(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	auto Forward = m_pCameraSpringArm->GetForwardVector();
	if (m_InputState == ERTSInputState::CameraMouseShuffle)
	{
		//Mouse based shuffle
		AddActorWorldOffset(Forward.GetSafeNormal2D() * -AxisValue * m_MouseShuffleSpeed * m_pCamera->AspectRatio);

	}
	else
	{
		//Key based branch
		AddActorWorldOffset(Forward.GetSafeNormal2D() * AxisValue * m_KeyShuffleSpeed);

	}


}

void ARTSPlayerEye::AddRightMovement(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	auto Right = m_pCameraSpringArm->GetRightVector();
	if (m_InputState == ERTSInputState::CameraMouseShuffle)
	{
		//Mouse based shuffle		
		AddActorWorldOffset(Right.GetSafeNormal2D() * -AxisValue * m_MouseShuffleSpeed);
		
	}
	else
	{
		//Key based branch, todo: delta time
		AddActorWorldOffset(Right.GetSafeNormal2D() * AxisValue * m_KeyShuffleSpeed);

	}


}

void ARTSPlayerEye::AddCameraYaw(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	if (m_InputState == ERTSInputState::CameraMouseTurn || m_InputState == ERTSInputState::PlacementCameraMouseTurn)
	{
		m_pCameraSpringArm->AddRelativeRotation(FRotator{ 0, AxisValue * m_MouseTurnSpeed, 0 });

	}
	else
	{
		m_pCameraSpringArm->AddRelativeRotation(FRotator{ 0, AxisValue * m_KeyTurnSpeed, 0 });


	}


}

void ARTSPlayerEye::AddCameraPitch(const float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;

	}

	auto SummedPitch = m_pCameraSpringArm->RelativeRotation.Pitch + AxisValue * m_MouseTurnSpeed * m_pCamera->AspectRatio;
	if (SummedPitch > -m_CameraMaxPitch && SummedPitch < -m_CameraMinPitch)
	{
		if (m_InputState == ERTSInputState::CameraMouseTurn || m_InputState == ERTSInputState::PlacementCameraMouseTurn)
		{
			m_pCameraSpringArm->RelativeRotation.Pitch = SummedPitch;

		}


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
#pragma endregion

void ARTSPlayerEye::UpdateCursorRoot()
{
	FHitResult PlaceableCursorResult;
	FHitResult NotPlaceableCursorResult;

	auto *pController = Cast<APlayerController>( GetController() );
	if (!pController)
	{
		return;

	}
		
	//Hit queries
	pController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, PlaceableCursorResult);
	pController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, NotPlaceableCursorResult);

	//On placable surfaces
	if (PlaceableCursorResult.IsValidBlockingHit())
	{
		FVector DiscrPos;
		auto *pGInst = Cast<URTSGameInstance>(GetGameInstance());

		if (!pGInst)
		{
			return;

		}
		DiscrPos = pGInst->GetMainStructureFactory()->Discretize(PlaceableCursorResult.Location);
		
		//Synch the cursor roots position
		m_pCursorRoot->SetWorldLocation(DiscrPos);
		UE_LOG(RTS_InputDebug, VeryVerbose, TEXT("Placable Cursor hit result: %s"), *PlaceableCursorResult.GetActor()->GetName());
	
		//While placing buildings
		if ( m_pBuildingPreviewCurrent && (m_InputState == ERTSInputState::Placement || m_InputState == ERTSInputState::PlacementCameraMouseTurn) )
		{
			//Update cursor root rotation
			//(fixed location is debug)
			auto NewRot = (-m_pBuildingPreviewCurrent->GetActorLocation()).GetSafeNormal2D().ToOrientationQuat();
			m_pBuildingPreviewCurrent->SetActorRotation(NewRot);
			
			//If placement loacation is occluded
			if (NotPlaceableCursorResult.IsValidBlockingHit() || !pGInst->GetMainStructureFactory()->IsPlacableAtPosition(m_pBuildingPreviewCurrent))
			{
				UE_LOG(RTS_InputDebug, VeryVerbose, TEXT("Non-placable cursor hit result: %s"), *NotPlaceableCursorResult.GetActor()->GetName());
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
	
	}
		

}

void ARTSPlayerEye::StopBuildingPreview()
{
	UE_LOG(RTS_StructurePlacement, Log, TEXT("Stopping building preview"));
	if (m_pBuildingPreviewCurrent)
	{
		m_pBuildingPreviewCurrent->Destroy();

	}
	//Also disrupts any camera movement
	m_InputState =  ERTSInputState::Idle;
	//Set to unplacable so a new building is checked in the first frame
	m_bBuildingPreviewWasPlacable = false;

}
