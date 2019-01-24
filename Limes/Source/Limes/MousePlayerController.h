// 
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "RTSSpectatorPawn.h"
#include "Engine/World.h"
#include "PlayerAIController.h"
#include "ConstructorHelpers.h"
#include "Camera/CameraActor.h"
#include "Engine/GameViewportClient.h"
#include "SpaceDiscretizer.h"
#include "Limes.h"

#include "MousePlayerController.generated.h"

enum class EBuildingState
{
	None,
	InPlacement,
	Placed,
	Invalid

};


UCLASS()
class LIMES_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMousePlayerController(const class FObjectInitializer &Initializer);


protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	void IssueAIMovement();

	void SelectSingleUnit();

	void AddUnitToSelection();

	void ZoomCameraOut(float AxisValue);

	void RotateCameraRight(float AxisValue);

	void JumpToLastSelected();

	void ToggleFocusLastSelected();

	void SaveClickPosition();

	void ActivateMousebasedScrolling();

	void DeactivateMousebasedScrolling();

	void ScrollCameraLeft(float AxisValue);

	void ScrollCameraForward(float AxisValue);

	void EnterBuildingPlacementMode();

	void PlaceBuilding();

	void LeaveBuildingPlacementMode();

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ABuildingPreview> m_PreviewActorClass;

	EBuildingState m_BuildingState{};

	UPROPERTY()
		class ABuildingPreview *m_pCurrentBuildingPreview;

	UPROPERTY()
		USceneComponent *m_pCursorDummy;

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<ARTSSpectatorPawn> m_SpectatorPawnClass;

	UPROPERTY()
		ASpectatorPawn *m_pSpecatorPawn;

	UPROPERTY(EditAnywhere, Meta = (InlineEditConditionToggle))
		bool bIsSingleHeroScenario = false;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = bIsSingleHeroScenario, AllowedClasses = Blueprint))
		FSoftObjectPath HeroPawnPath;

	
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "30", ClampMax = "100"))
		int32 ScreenScrollBorderWidth = 40;

	bool bDisableUnitSelection = false,
		 bHasFocusedSelection = false,
		 bIsMousebasedScrollingInProgress = false;

	TArray<APawn*> SelectedUnits;

	TSubclassOf<APawn> HeroBP;

	TSubclassOf<ASpectatorPawn> RTSSpectatorBP;

	ARTSSpectatorPawn *RTSSpectator;

	FVector2D MousePosition, ClickPosition;

	int32 ViewportSizeX, ViewportSizeY;

	UGameViewportClient *GameViewportClient;
		
	SpaceDiscretizer m_Discretizer;


};
