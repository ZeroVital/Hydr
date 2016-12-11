#include "Hydr.h"
#include "HydrSpectatorPawn.h"
#include "HydrGameHelpers.h"
#include "SButtonWidget.h"
#include "ZoomCameraComponent.h"
#include "HydrInput.h"
#include "HeroesSelectionInterface.h"
#include "HydrInputInterface.h"



APlayerController1::APlayerController1(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIgnoreInput(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bHidden = false;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;
}

void APlayerController1::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputHandler = NewObject<UHydrInput>(this);

	BIND_1P_ACTION(InputHandler, EGameKey::Tap, IE_Pressed, &APlayerController1::OnTapPressed);
	BIND_1P_ACTION(InputHandler, EGameKey::Hold, IE_Pressed, &APlayerController1::OnHoldPressed);
	BIND_1P_ACTION(InputHandler, EGameKey::Hold, IE_Released, &APlayerController1::OnHoldReleased);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Pressed, &APlayerController1::OnSwipeStarted);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Repeat, &APlayerController1::OnSwipeUpdate);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Released, &APlayerController1::OnSwipeReleased);
	BIND_2P_ACTION(InputHandler, EGameKey::SwipeTwoPoints, IE_Pressed, &APlayerController1::OnSwipeTwoPointsStarted);
	BIND_2P_ACTION(InputHandler, EGameKey::SwipeTwoPoints, IE_Repeat, &APlayerController1::OnSwipeTwoPointsUpdate);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Pressed, &APlayerController1::OnPinchStarted);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Repeat, &APlayerController1::OnPinchUpdate);

	FInputActionBinding& ToggleInGameMenuBinding = InputComponent->BindAction("InGameMenu", IE_Pressed, this, &APlayerController1::OnToggleInGameMenu);
	ToggleInGameMenuBinding.bExecuteWhenPaused = true;

}
/*
void APlayerController1::GetAudioListenerPosition(FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir)
{
	Super::GetAudioListenerPosition(OutLocation, OutFrontDir, OutRightDir);

	AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportFrame && State != NULL *//*&& State->MiniMapCamera.IsValid()*//*)
	{
		// Set Listener position to be the center of the viewport, projected into the game world.

		FViewport* const Viewport = GEngine->GameViewport->ViewportFrame->GetViewport();
		if (Viewport)
		{
			FVector2D const ScreenRes = Viewport->GetSizeXY();

			float GroundLevel = State->MiniMapCamera->AudioListenerGroundLevel;
			const FPlane GroundPlane = FPlane(FVector(0, 0, GroundLevel), FVector::UpVector);
			ULocalPlayer* const Player = Cast<ULocalPlayer>(Player);

			// @todo: once PlayerCamera is back in, we can just get the ray origin and dir from that instead of 
			// needing to deproject. will be much simpler.
			FVector RayOrigin, RayDirection;
			FVector2D const ScreenCenterPoint = ScreenRes * 0.5f;
			FHydrGameHelpers::DeprojectScreenToWorld(ScreenCenterPoint, Player, RayOrigin, RayDirection);

			FVector const WorldPoint = FHydrGameHelpers::IntersectRayWithPlane(RayOrigin, RayDirection, GroundPlane);
			FVector const AudioListenerOffset = State->MiniMapCamera->AudioListenerLocationOffset;
			OutLocation = WorldPoint.GetClampedToSize(State->WorldBounds.Min.GetMin(), State->WorldBounds.Max.GetMax()) + AudioListenerOffset;

			bool bUseCustomOrientation = State->MiniMapCamera->bUseAudioListenerOrientation;
			if (bUseCustomOrientation)
			{
				OutFrontDir = State->MiniMapCamera->AudioListenerFrontDir;
				OutRightDir = State->MiniMapCamera->AudioListenerRightDir;
			}
		}
	}
}*/

void APlayerController1::OnToggleInGameMenu()
{
	AHydrHUD* const CoreHUD = Cast<AHydrHUD>(GetHUD());
	if (CoreHUD)
	{
		CoreHUD->TogglePauseMenu();
	}
}

void APlayerController1::UpdateRotation(float DeltaTime)
{
	FRotator ViewRotation(0, 0, 0);
	FRotator DeltaRot(0, 0, 0);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);
	}

	SetControlRotation(ViewRotation);
}

void APlayerController1::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	if (!bGamePaused && PlayerInput && InputHandler && !bIgnoreInput)
	{
		InputHandler->UpdateDetection(DeltaTime);
	}

	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	if (!bIgnoreInput)
	{
		const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		AHydrSpectatorPawn* HydrSpectatorPawn = GetHydrSpectatorPawn();
		if ((HydrSpectatorPawn != NULL) && (LocalPlayer != NULL))
		{
			// Create the bounds for the minimap so we can add it as a 'no scroll' zone.
			AHydrHUD* const CoreHUD = Cast<AHydrHUD>(GetHUD());
			AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
			//const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);
		}
	}
}

void APlayerController1::SetCameraTarget(const FVector& CameraTarget)
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->SetCameraTarget(CameraTarget);
	}
}

AActor* APlayerController1::GetFriendlyTarget(const FVector2D& ScreenPoint, FVector& WorldPoint) const
{
	FHitResult Hit;
	if (GetHitResultAtScreenPosition(ScreenPoint, COLLISION_WEAPON, true, Hit))
	{
		/*if (!AHydrGameMode::OnEnemyTeam(Hit.GetActor(), this))
		{
			WorldPoint = Hit.ImpactPoint;
			return Hit.GetActor();
		}*/
	}

	return NULL;
}

void APlayerController1::SetIgnoreInput(bool bIgnore)
{
	bIgnoreInput = bIgnore;
}

/*uint8 APlayerController1::GetTeamNum() const
{
	return ETeam::Player;
};*/

void APlayerController1::SetSelectedActor(AActor* NewSelectedActor, const FVector& NewPosition)
{
	if (SelectedActor != NewSelectedActor)
	{
		// attempt to unselect current selection
		AActor* const OldSelection = SelectedActor.Get();
		if (OldSelection && OldSelection->GetClass()->ImplementsInterface(UHeroesSelectionInterface::StaticClass()))
		{
			if (IHeroesSelectionInterface::Execute_OnSelectionLost(OldSelection, NewPosition, NewSelectedActor))
			{
				SelectedActor = NULL;
			}
		}

		if (!SelectedActor.IsValid())
		{
			// attempt to select new selection
			if (NewSelectedActor && NewSelectedActor->GetClass()->ImplementsInterface(UHeroesSelectionInterface::StaticClass()))
			{
				if (IHeroesSelectionInterface::Execute_OnSelectionGained(NewSelectedActor))
				{
					SelectedActor = NewSelectedActor;
				}
			}
		}
	}
}

void APlayerController1::OnTapPressed(const FVector2D& ScreenPosition, float DownTime)
{
	FVector WorldPosition(0.f);
	AActor* const HitActor = GetFriendlyTarget(ScreenPosition, WorldPosition);

	SetSelectedActor(HitActor, WorldPosition);

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UHydrInputInterface::StaticClass()))
	{
		IHydrInputInterface::Execute_OnInputTap(HitActor);
	}
}

void APlayerController1::OnHoldPressed(const FVector2D& ScreenPosition, float DownTime)
{
	FVector WorldPosition(0.0f);
	AActor* const HitActor = GetFriendlyTarget(ScreenPosition, WorldPosition);

	SetSelectedActor(HitActor, WorldPosition);

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UHydrInputInterface::StaticClass()))
	{
		IHydrInputInterface::Execute_OnInputHold(HitActor);
	}
}

void APlayerController1::OnHoldReleased(const FVector2D& ScreenPosition, float DownTime)
{
	AActor* const Selected = SelectedActor.Get();
	if (Selected && Selected->GetClass()->ImplementsInterface(UHydrInputInterface::StaticClass()))
	{
		IHydrInputInterface::Execute_OnInputHoldReleased(Selected, DownTime);
	}
}

void APlayerController1::OnSwipeStarted(const FVector2D& AnchorPosition, float DownTime)
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->OnSwipeStarted(AnchorPosition);
	}

	FVector WorldPosition(0.0f);
	AActor* const HitActor = GetFriendlyTarget(AnchorPosition, WorldPosition);

	SetSelectedActor(HitActor, WorldPosition);

	// Get our position in 3d space 
	if (SelectedActor.IsValid())
	{
		SwipeAnchor3D = SelectedActor->GetActorLocation();
	}

	PrevSwipeScreenPosition = AnchorPosition;
}

void APlayerController1::OnSwipeUpdate(const FVector2D& ScreenPosition, float DownTime)
{
	AActor* const Selected = SelectedActor.Get();
	if (Selected && Selected->GetClass()->ImplementsInterface(UHydrInputInterface::StaticClass()))
	{
		ULocalPlayer* const Player1 = Cast<ULocalPlayer>(Player);
		const FPlane GroundPlane = FPlane(FVector(0, 0, SelectedActor->GetActorLocation().Z), FVector(0, 0, 1));

		FVector RayOrigin, RayDirection;
		FHydrGameHelpers::DeprojectScreenToWorld(ScreenPosition, Player1, RayOrigin, RayDirection);
		const FVector ScreenPosition3D = FHydrGameHelpers::IntersectRayWithPlane(RayOrigin, RayDirection, GroundPlane);

		IHydrInputInterface::Execute_OnInputSwipeUpdate(Selected, ScreenPosition3D - SwipeAnchor3D);
	}
	else
	{
		if (GetCameraComponent() != NULL)
		{
			GetCameraComponent()->OnSwipeUpdate(ScreenPosition);
		}
	}

	PrevSwipeScreenPosition = ScreenPosition;
}

void APlayerController1::OnSwipeReleased(const FVector2D& ScreenPosition, float DownTime)
{
	AActor* const Selected = SelectedActor.Get();
	if (Selected && Selected->GetClass()->ImplementsInterface(UHydrInputInterface::StaticClass()))
	{
		ULocalPlayer* const Player1 = Cast<ULocalPlayer>(this->Player);
		const FPlane GroundPlane = FPlane(FVector(0, 0, SelectedActor->GetActorLocation().Z), FVector(0, 0, 1));

		FVector RayOrigin, RayDirection;
		FHydrGameHelpers::DeprojectScreenToWorld(ScreenPosition, Player1, RayOrigin, RayDirection);
		const FVector ScreenPosition3D = FHydrGameHelpers::IntersectRayWithPlane(RayOrigin, RayDirection, GroundPlane);

		IHydrInputInterface::Execute_OnInputSwipeReleased(Selected, ScreenPosition3D - SwipeAnchor3D, DownTime);
	}
	else
	{
		if (GetCameraComponent() != NULL)
		{
			GetCameraComponent()->OnSwipeReleased(ScreenPosition);
		}
	}
}

void APlayerController1::OnSwipeTwoPointsStarted(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	PrevSwipeMidPoint = (ScreenPosition1 + ScreenPosition2) * 0.5f;
}

void APlayerController1::OnSwipeTwoPointsUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	const FVector2D SwipeMidPoint = (ScreenPosition1 + ScreenPosition2) * 0.5f;
	const FVector MoveDir = FVector(SwipeMidPoint - PrevSwipeMidPoint, 0.0f).GetSafeNormal();
	const float SwipeSpeed = 10000.0f;

	const FRotationMatrix R(PlayerCameraManager->GetCameraRotation() + FRotator(0.0, 90.0, 0.0));
	const FVector WorldSpaceAccel = R.TransformVector(MoveDir) * SwipeSpeed;
	if (GetHydrSpectatorPawn())
	{
		GetHydrSpectatorPawn()->AddMovementInput(WorldSpaceAccel, 1.f);
	}

	PrevSwipeMidPoint = SwipeMidPoint;
}

void APlayerController1::OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime)
{
	// Pass the pinch through to the camera component.
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->OnPinchStarted(AnchorPosition1, AnchorPosition2, DownTime);
	}
}

void APlayerController1::OnPinchUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	// Pass the pinch through to the camera component.
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->OnPinchUpdate(InputHandler, ScreenPosition1, ScreenPosition2, DownTime);
	}
}

AHydrSpectatorPawn* APlayerController1::GetHydrSpectatorPawn() const
{
	return Cast<AHydrSpectatorPawn>(GetSpectatorPawn());
}

UZoomCameraComponent* APlayerController1::GetCameraComponent() const
{
	UZoomCameraComponent* CameraComponent = NULL;
	if (GetHydrSpectatorPawn() != NULL)
	{
		CameraComponent = GetHydrSpectatorPawn()->GetCameraComponent();
	}
	return CameraComponent;
}

void APlayerController1::MouseLeftMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}
void APlayerController1::MousePressedOverMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}

void APlayerController1::MouseReleasedOverMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}



