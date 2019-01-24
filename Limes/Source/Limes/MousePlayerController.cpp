// Fill out your copyright notice in the Description page of Project Settings.

#include "MousePlayerController.h"
#include "BuildingBase.h"
#include "BuildingPreview.h"

AMousePlayerController::AMousePlayerController(const FObjectInitializer &Initializer) :
	m_Discretizer{ {0,0,0}, 600, 30, 3 },
	Super(Initializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
		
	//UE_LOG(LogTemp, Warning, TEXT("azm: %f, depth: %f"), m_Discretizer.GetAzimutDeg(), m_Discretizer.GetCellDepth());
	m_pCursorDummy = CreateDefaultSubobject<USceneComponent>(TEXT("CursorDummy"));
	/*
	static ConstructorHelpers::FObjectFinder<UBlueprint> LoadedBP_RTSSpectator(TEXT("Blueprint'/Game/Blueprints/BP_RTSSpectatorPawn.BP_RTSSpectatorPawn'"));
	if (LoadedBP_RTSSpectator.Object)
	{
		RTSSpectatorBP = Cast<UClass>(LoadedBP_RTSSpectator.Object->GeneratedClass);
		UE_LOG(LogLoad, Log, TEXT("RTSSpectatorClass Loaded"));
	}
	*/
		

	
}


void AMousePlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UClass *pPawnClass = nullptr;
	if ((pPawnClass = m_SpectatorPawnClass.Get()) == nullptr)
	{
		pPawnClass = m_SpectatorPawnClass.LoadSynchronous();

	}

	if (pPawnClass)
	{
		
		if (auto *pGamemode = GetWorld()->GetAuthGameMode())
		{
			auto Transform = pGamemode->ChoosePlayerStart(this)->GetActorTransform();
			RTSSpectator = Cast<std::remove_pointer_t<decltype(RTSSpectator)>>(GetWorld()->SpawnActor(pPawnClass, &Transform));
		
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load spectator pawn"));

	}



	if (bIsSingleHeroScenario)
	{
		bDisableUnitSelection = true;
		if (!HeroPawnPath.IsNull())
		{
			HeroBP = Cast<UClass>(Cast<UBlueprint>(HeroPawnPath.TryLoad())->GeneratedClass);
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HERO PAWN PATH NULL"));

		}

	}


}

void AMousePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("LMB", IE_Pressed, this, &AMousePlayerController::SelectSingleUnit);
	InputComponent->BindAction("LMB+Shift", IE_Pressed, this, &AMousePlayerController::AddUnitToSelection);
	InputComponent->BindAction("RMB", IE_Pressed, this, &AMousePlayerController::IssueAIMovement);
	InputComponent->BindAction("Jump To Unit", IE_Pressed, this, &AMousePlayerController::JumpToLastSelected);
	InputComponent->BindAction("Jump To Unit", IE_DoubleClick, this, &AMousePlayerController::ToggleFocusLastSelected);
	InputComponent->BindAction("Mousebased Scrolling", IE_Pressed, this, &AMousePlayerController::ActivateMousebasedScrolling);
	InputComponent->BindAction("Mousebased Scrolling", IE_Released, this, &AMousePlayerController::DeactivateMousebasedScrolling);
	InputComponent->BindAxis("Rotate Camera Right", this, &AMousePlayerController::RotateCameraRight);
	InputComponent->BindAxis("Scroll Camera Y", this, &AMousePlayerController::ScrollCameraForward);
	InputComponent->BindAxis("Scroll Camera X", this, &AMousePlayerController::ScrollCameraLeft);
	InputComponent->BindAxis("Zoom Camera Out", this, &AMousePlayerController::ZoomCameraOut);
	
	InputComponent->BindAction("DebugEndPlaceBuilding", IE_Pressed, this, &AMousePlayerController::PlaceBuilding);
	InputComponent->BindAction("DebugStartPlaceBuilding", IE_Pressed, this, &AMousePlayerController::EnterBuildingPlacementMode);

}

void AMousePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	GameViewportClient = GetGameInstance()->GetGameViewportClient();
	check(GameViewportClient);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->SetViewTarget(RTSSpectator);

	}
	else
	{
		UE_LOG(LogInit, Error, TEXT("COULDNT SET VIEW TARGET"));

	}

	if (bIsSingleHeroScenario)
	{
		if (HeroBP)
		{
			FActorSpawnParameters Params;
			SelectedUnits.SetNum(0);
			SelectedUnits.EmplaceAt(0, GetWorld()->SpawnActor<APawn>
				(
					HeroBP,
					GetWorld()->GetAuthGameMode()->ChoosePlayerStart(this)->GetActorLocation(),
					FRotator::ZeroRotator,
					Params
					));
			UE_LOG(LogTemp, Log, TEXT("Hero BP Spawned"));
			bHasFocusedSelection = true;

		}
		else
		{
			UE_LOG(LogLoad, Error, TEXT("HERO BP NULL ON BEGIN PLAY"));

		}
		RTSSpectator->AllowMovement(false);

	}


}

void AMousePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetMousePosition(MousePosition.X, MousePosition.Y);
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	
	////
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		if (m_BuildingState != EBuildingState::InPlacement)
		{
			m_pCursorDummy->SetWorldLocation(HitResult.ImpactPoint);

			if (m_BuildingState == EBuildingState::Invalid)
			{
				m_BuildingState = HitResult.GetActor()->IsA<ABuildingBase>() ? EBuildingState::Invalid : EBuildingState::InPlacement;

			}

		}
		else
		{
			if (HitResult.GetActor()->IsA<ABuildingBase>())
			{
				m_BuildingState = EBuildingState::Invalid;

			}
			else
			{
				auto c = m_Discretizer.Discretize(HitResult.ImpactPoint);
				m_pCursorDummy->SetWorldLocation(c);

			}

		}

		if (auto *p = HitResult.GetActor())
		{			
			UE_LOG(LogTemp, Log, TEXT("is building: %i, name: %s"), p->IsA<ABuildingBase>(), *p->GetName());
		
		}
		
	
	}

	if (m_BuildingState == EBuildingState::Invalid)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid"));

	}


	if (m_pCurrentBuildingPreview && (m_BuildingState == EBuildingState::InPlacement || m_BuildingState == EBuildingState::Invalid))
	{
		FRotator Rot{ 0,0,0 };


		m_pCurrentBuildingPreview->SetActorRotation((-m_pCurrentBuildingPreview->GetActorLocation()).GetSafeNormal2D().ToOrientationQuat());

	}

	////

	if (bHasFocusedSelection && SelectedUnits.Num())
	{
		RTSSpectator->SetActorLocation(SelectedUnits.Last()->GetActorLocation());

	}
	
	if (bIsMousebasedScrollingInProgress && RTSSpectator->IsMovementAllowed())
	{
		//todo: Mouse y coordinates inverted due to screen coords - does this change depending on graphics api or is it engine related
		FVector2D Direction = MousePosition - ClickPosition;
		RTSSpectator->MoveCamera(Direction);
		
	}
	else if (GameViewportClient->IsFocused(GameViewportClient->Viewport) && RTSSpectator->IsMovementAllowed())
	{		
		//Left Screen Border
		if (MousePosition.X < ScreenScrollBorderWidth)
		{
			RTSSpectator->MoveCameraRight(-1.f);

		}
		//Right Screen Border
		else if (ViewportSizeX - MousePosition.X < ScreenScrollBorderWidth)
		{
			RTSSpectator->MoveCameraRight(1.f);

		}

		//Top Screen Border
		if (MousePosition.Y < ScreenScrollBorderWidth)
		{
			RTSSpectator->MoveCameraForward(1.f);

		}
		//Bottom Screen Border
		else if (ViewportSizeY - MousePosition.Y < ScreenScrollBorderWidth)
		{
			RTSSpectator->MoveCameraForward(-1.f);

		}

	}

}

//Accesses AIControllers of Units in SelectedUnits/ConstantlySelectedUnits Arrays and uses NavMeshMovement to move to cursor click
//Input Hook
//todo: collision channel setup, save AI cast results
void AMousePlayerController::IssueAIMovement()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		if (SelectedUnits.Num())
		{
			for (APawn* Unit : SelectedUnits)
			{
				AAIController *Temp = Cast<AAIController>(Unit->GetController());
				if (Temp)
				{
					Temp->MoveTo(HitResult.Location);

				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("NO CONTROLLER ACCESSIBLE DYNAMIC SELECT"))

				}

			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SelectionEmpty"));

		}

	}
	UE_LOG(LogTemp, Warning, TEXT("LMB Triggered %s UnitCount: %i"), *(RTSSpectator->GetHumanReadableName()), SelectedUnits.Num());


}

//Modifies unit selection so that the clicked unit is selected exclusively
//Input Hook
//todo: setup collision channels
void AMousePlayerController::SelectSingleUnit()
{
	if (!bDisableUnitSelection)
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);
		if (HitResult.IsValidBlockingHit())
		{
			APawn *temp = Cast<APawn>(HitResult.GetActor());
			if (temp)
			{
				SelectedUnits.SetNum(0);
				SelectedUnits.Insert(Cast<APawn>(HitResult.GetActor()), 0);
				UE_LOG(LogTemp, Warning, TEXT("ValidHitPawn SelectOne %s SelectSize: %i"), *(HitResult.GetActor()->GetHumanReadableName()), SelectedUnits.Num());

			}
			else
			{
				SelectedUnits.SetNum(0);
				UE_LOG(LogTemp, Warning, TEXT("ValidHitNoPawn %s SelectNum: %i"), *(HitResult.GetActor()->GetHumanReadableName()), SelectedUnits.Num());

			}

		}
		else
		{
			SelectedUnits.SetNum(0);
			UE_LOG(LogTemp, Warning, TEXT("InvalidHit SelectZero"));

		}

	}

}

//Adds the clicked Pawn to the unit selection if enabled
//Input Hook
//todo: setup collision channels
void AMousePlayerController::AddUnitToSelection()
{
	if (!bDisableUnitSelection)
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);
		if (HitResult.IsValidBlockingHit())
		{
			APawn *temp = Cast<APawn>(HitResult.GetActor());
			if (temp)
			{
				SelectedUnits.Emplace(Cast<APawn>(HitResult.GetActor()));
				UE_LOG(LogTemp, Warning, TEXT("ValidHitPawn, AddedPawn %s SelectNum: %i"), *(HitResult.GetActor()->GetHumanReadableName()), SelectedUnits.Num());

			}
			else
			{
				SelectedUnits.SetNum(0);
				UE_LOG(LogTemp, Warning, TEXT("ValidHitNoPawn %s SelectNum: %i"), *(HitResult.GetActor()->GetHumanReadableName()), SelectedUnits.Num());

			}

		}
		else
		{
			SelectedUnits.SetNum(0);
			UE_LOG(LogTemp, Warning, TEXT("InvalidHit SelectZero"));

		}

	}


}

//Input Hook
void AMousePlayerController::ZoomCameraOut(float AxisValue)
{
	RTSSpectator->IncreaseCameraDistance(AxisValue);


}

//Input Hook
void AMousePlayerController::ToggleFocusLastSelected()
{
	if (!bIsSingleHeroScenario)
	{
		if (bHasFocusedSelection)
		{
			bHasFocusedSelection = false;

		}
		else
		{
			bHasFocusedSelection = SelectedUnits.Num() ? true : false;
			if (SelectedUnits.Num()) { UE_LOG(LogInput, Warning, TEXT("Attempting Focus: No Unit Selected")) }
			else{ UE_LOG(LogInput, Log, TEXT("Focused Actor: %s"), *(SelectedUnits.Last()->GetHumanReadableName())); }

		}

	}


}

//Input Hook
void AMousePlayerController::JumpToLastSelected()
{
	if (SelectedUnits.Num())
	{
		RTSSpectator->SetActorLocation(SelectedUnits.Last()->GetActorLocation());

	}


}

//Input Hook
void AMousePlayerController::RotateCameraRight(float AxisValue)
{
	RTSSpectator->RotateCameraRight(AxisValue);
	

}

//Input Hook
void AMousePlayerController::ActivateMousebasedScrolling()
{
	SaveClickPosition();
	//prevents other movement actions from being executed
	bIsMousebasedScrollingInProgress = true;


}

//Input Hook
void AMousePlayerController::DeactivateMousebasedScrolling()
{
	bIsMousebasedScrollingInProgress = false;

}

//Input Hook
void AMousePlayerController::ScrollCameraForward(float AxisValue)
{
	if (!bIsMousebasedScrollingInProgress)
	{
		RTSSpectator->MoveCameraForward(AxisValue);

	}


}

//Input Hook
void AMousePlayerController::ScrollCameraLeft(float AxisValue)
{
	if (!bIsMousebasedScrollingInProgress)
	{
		RTSSpectator->MoveCameraRight(AxisValue);
	
	}


}

//Used in Mousebased Scrolling
//todo: Reasonable ? Maybe just do it inline
void AMousePlayerController::SaveClickPosition()
{
	GetMousePosition(ClickPosition.X, ClickPosition.Y);
	UE_LOG(LogInput, Log, TEXT("ClickPosition: %s"), *(ClickPosition.ToString()));


}

void AMousePlayerController::EnterBuildingPlacementMode()
{
	if (m_BuildingState != EBuildingState::InPlacement)
	{
		if (auto *pClass = SafeLoadClassPtr(m_PreviewActorClass))
		{
			m_pCurrentBuildingPreview = GetWorld()->SpawnActor<ABuildingPreview>(pClass, m_pCursorDummy->GetComponentTransform());
			if (m_pCurrentBuildingPreview)
			{
				m_pCurrentBuildingPreview->AttachToComponent(m_pCursorDummy, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				m_BuildingState = EBuildingState::InPlacement;
			}

		}
		/*if (m_PreviewActorClass)
		{
			pActorInPlacement = NewObject<AActor>(nullptr, SafeLoadClassPtr(m_PreviewActorClass));
			UE_LOG(LogTemp, Log, TEXT("%s"), *pActorToPlace->GetName());

		}*/
	}
	

}

void AMousePlayerController::PlaceBuilding()
{
	if (m_BuildingState == EBuildingState::InPlacement && m_pCurrentBuildingPreview)
	{
		
		auto pClassToSpawn = m_pCurrentBuildingPreview->GetPreviewedClass();
		GetWorld()->SpawnActor<ABuildingBase>(pClassToSpawn, m_pCurrentBuildingPreview->GetActorTransform());
		m_pCurrentBuildingPreview->Destroy();// DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		

		m_pCurrentBuildingPreview = nullptr;
		m_BuildingState = EBuildingState::Placed;

	}

}

void AMousePlayerController::LeaveBuildingPlacementMode()
{

}
