#include "Hydr.h"
#include "ZoomCameraComponent.h"
#include "HydrGameHelpers.h"
#include "HydrSpectatorPawnMovement.h"
#include "HydrInput.h"

UZoomCameraComponent::UZoomCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZoomAlpha = 1.0f;
	CameraScrollSpeed = 4000.0f;
	MinZoomLevel = 0.4f;
	MaxZoomLevel = 1.0f;
	MiniMapBoundsLimit = 0.8f;
	StartSwipeCoords.Set(0.0f, 0.0f, 0.0f);
}

void UZoomCameraComponent::OnZoomIn()
{
	SetZoomLevel(ZoomAlpha - 0.1f);
}

void UZoomCameraComponent::OnZoomOut()
{
	SetZoomLevel(ZoomAlpha + 0.1f);
}

void UZoomCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& OutResult)
{
	APlayerController1* Controller = GetPlayerController();
	if (Controller)
	{
		OutResult.FOV = 30.f;
		const float CurrentOffset = MinCameraOffset + ZoomAlpha * (MaxCameraOffset - MinCameraOffset);
		FVector Pos2 = Controller->GetFocalLocation();
		OutResult.Location = Controller->GetFocalLocation() - FixedCameraAngle.Vector() * CurrentOffset;
		OutResult.Rotation = FixedCameraAngle;
	}
}

void UZoomCameraComponent::UpdateCameraMovement(const APlayerController1* InPlayerController)
{
	// No mouse support on mobile
#if PLATFORM_DESKTOP
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition) == false)
		{
			return;
		}

		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		const float ScrollSpeed = 60.0f;
		const FIntPoint ViewportSize = Viewport->GetSizeXY();
		const uint32 ViewLeft = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.X);
		const uint32 ViewRight = ViewLeft + FMath::TruncToInt(LocalPlayer->Size.X * ViewportSize.X);
		const uint32 ViewTop = FMath::TruncToInt(LocalPlayer->Origin.Y * ViewportSize.Y);
		const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);

		const float MaxSpeed = CameraScrollSpeed * FMath::Clamp(ZoomAlpha, 0.3f, 1.0f);

		AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
		bool bNoScrollZone = false;
		FVector MouseCoords(MousePosition, 0.0f);
		for (int iZone = 0; iZone < NoScrollZones.Num(); iZone++)
		{
			FBox EachZone = NoScrollZones[iZone];
			if (EachZone.IsInsideXY(MouseCoords) == true)
			{
				bNoScrollZone = true;
			}
		}

		const uint32 MouseX = MousePosition.X;
		const uint32 MouseY = MousePosition.Y;
		float SpectatorCameraSpeed = MaxSpeed;
		ASpectatorPawn* HydrSpectatorPawn = NULL;
		if (GetPlayerController() != NULL)
		{
			HydrSpectatorPawn = GetPlayerController()->GetSpectatorPawn();
			if (HydrSpectatorPawn->GetMovementComponent() != NULL)
			{
				SpectatorCameraSpeed = GetDefault<UHydrSpectatorPawnMovement>(HydrSpectatorPawn->GetMovementComponent()->GetClass())->MaxSpeed;
			}
		}
		if (!bNoScrollZone)
		{
			if (MouseX >= ViewLeft && MouseX <= (ViewLeft + CameraActiveBorder))
			{
				const float delta = 1.0f - float(MouseX - ViewLeft) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveRight(-ScrollSpeed * delta);
			}
			else if (MouseX >= (ViewRight - CameraActiveBorder) && MouseX <= ViewRight)
			{
				const float delta = float(MouseX - ViewRight + CameraActiveBorder) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveRight(ScrollSpeed * delta);
			}

			if (MouseY >= ViewTop && MouseY <= (ViewTop + CameraActiveBorder))
			{
				const float delta = 1.0f - float(MouseY - ViewTop) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveForward(ScrollSpeed * delta);
			}
			else if (MouseY >= (ViewBottom - CameraActiveBorder) && MouseY <= ViewBottom)
			{
				const float delta = float(MouseY - (ViewBottom - CameraActiveBorder)) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveForward(-ScrollSpeed * delta);
			}

			if (HydrSpectatorPawn != NULL)
			{
				UFloatingPawnMovement* PawnMovementComponent = Cast<UFloatingPawnMovement>(HydrSpectatorPawn->GetMovementComponent());
				if (PawnMovementComponent)
				{
					PawnMovementComponent->MaxSpeed = SpectatorCameraSpeed;
				}
			}
		}
	}
#endif
	NoScrollZones.Empty();
}

void UZoomCameraComponent::MoveForward(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController1* Controller = GetPlayerController();
		if ((Val != 0.f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::X) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void UZoomCameraComponent::MoveRight(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController1* Controller = GetPlayerController();
		if ((Val != 0.f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Y) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void UZoomCameraComponent::AddNoScrollZone(FBox InCoords)
{
	NoScrollZones.AddUnique(InCoords);
}

void UZoomCameraComponent::ClampCameraLocation(const APlayerController1* InPlayerController, FVector& OutCameraLocation)
{
	if (bShouldClampCamera)
	{
		UpdateCameraBounds(InPlayerController);
		if (CameraMovementBounds.GetSize() != FVector::ZeroVector)
		{
			OutCameraLocation = CameraMovementBounds.GetClosestPointTo(OutCameraLocation);
		}
	}
}

void UZoomCameraComponent::UpdateCameraBounds(const APlayerController1* InPlayerController)
{
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if (LocalPlayer == NULL || LocalPlayer->ViewportClient == NULL)
	{
		return;
	}

	FVector2D CurrentViewportSize;
	LocalPlayer->ViewportClient->GetViewportSize(CurrentViewportSize);

	// calc frustum edge direction, from bottom left corner
	if (CameraMovementBounds.GetSize() == FVector::ZeroVector || CurrentViewportSize != CameraMovementViewportSize)
	{
		// calc frustum edge direction, from bottom left corner
		const FVector FrustumRay2DDir = FVector(1, 1, 0).GetSafeNormal();
		const FVector FrustumRay2DRight = FVector::CrossProduct(FrustumRay2DDir, FVector::UpVector);
		const FQuat RotQuat(FrustumRay2DRight, FMath::DegreesToRadians(90.0f - InPlayerController->PlayerCameraManager->GetFOVAngle() * 0.5f));
		const FVector FrustumRayDir = RotQuat.RotateVector(FrustumRay2DDir);

		// collect 3 world bounds' points and matching frustum rays (bottom left, top left, bottom right)
		AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
		if (State)
		{
			FBox const& WorldBounds = State->WorldBounds;

			if (WorldBounds.GetSize() != FVector::ZeroVector)
			{
				const FVector WorldBoundPoints[] = {
					FVector(WorldBounds.Min.X, WorldBounds.Min.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Min.X, WorldBounds.Max.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Max.X, WorldBounds.Min.Y, WorldBounds.Max.Z)
				};
				const FVector FrustumRays[] = {
					FVector(FrustumRayDir.X,  FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(FrustumRayDir.X, -FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(-FrustumRayDir.X,  FrustumRayDir.Y, FrustumRayDir.Z)
				};

				// get camera plane for intersections
				const FPlane CameraPlane = FPlane(InPlayerController->GetFocalLocation(), FVector::UpVector);

				// get matching points on camera plane
				const FVector CameraPlanePoints[3] = {
					FHydrGameHelpers::IntersectRayWithPlane(WorldBoundPoints[0], FrustumRays[0], CameraPlane)*MiniMapBoundsLimit,
					FHydrGameHelpers::IntersectRayWithPlane(WorldBoundPoints[1], FrustumRays[1], CameraPlane)*MiniMapBoundsLimit,
					FHydrGameHelpers::IntersectRayWithPlane(WorldBoundPoints[2], FrustumRays[2], CameraPlane)*MiniMapBoundsLimit
				};

				// create new bounds
				CameraMovementBounds = FBox(CameraPlanePoints, 3);
				CameraMovementViewportSize = CurrentViewportSize;
			}
		}
	}
}

APlayerController1* UZoomCameraComponent::GetPlayerController()
{
	APlayerController1* Controller = NULL;
	APawn* Owner = GetOwnerPawn();
	if (Owner != NULL)
	{
		Controller = Cast<APlayerController1>(Owner->GetController());
	}
	return Controller;
}

APawn* UZoomCameraComponent::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

void UZoomCameraComponent::OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime)
{
	InitialPinchAlpha = ZoomAlpha;
}

static TAutoConsoleVariable<float> CVarPinchScale(TEXT("PinchScale"), 0.002f, TEXT("How fast  pinch is."));

void UZoomCameraComponent::OnPinchUpdate(UHydrInput* InputHandler, const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	const float AnchorDistance = (InputHandler->GetTouchAnchor(0) - InputHandler->GetTouchAnchor(1)).Size();
	const float CurrentDistance = (ScreenPosition1 - ScreenPosition2).Size();
	const float PinchDelta = AnchorDistance - CurrentDistance;
	const float PinchScale = CVarPinchScale.GetValueOnGameThread();

	SetZoomLevel(InitialPinchAlpha + PinchDelta * PinchScale);
}

void UZoomCameraComponent::SetCameraTarget(const FVector& CameraTarget)
{
	ASpectatorPawn* HydrSpectatorPawn = GetPlayerController()->GetSpectatorPawn();
	if (HydrSpectatorPawn != NULL)
	{
		HydrSpectatorPawn->SetActorLocation(CameraTarget, false);
	}
}

void UZoomCameraComponent::SetZoomLevel(float NewLevel)
{
	ZoomAlpha = FMath::Clamp(NewLevel, MinZoomLevel, MaxZoomLevel);
}

bool UZoomCameraComponent::OnSwipeStarted(const FVector2D& SwipePosition)
{
	bool bResult = false;
	// Ensure we are NOT trying to start a drag/scroll over a no scroll zone (EG mini map)
	if (AreCoordsInNoScrollZone(SwipePosition) == false)
	{
		APlayerController1* Controller = GetPlayerController();
		if (Controller)
		{
			// Get intersection point with the plan used to move around
			FHitResult Hit;
			if (Controller->GetHitResultAtScreenPosition(SwipePosition, COLLISION_PANCAMERA, true, Hit))
			{
				StartSwipeCoords = Hit.ImpactPoint;
				bResult = true;
			}
		}
	}
	else
	{
		EndSwipeNow();
	}
	return bResult;
}

bool UZoomCameraComponent::OnSwipeUpdate(const FVector2D& SwipePosition)
{
	bool bResult = false;
	APlayerController1* Controller = GetPlayerController();
	if ((Controller != NULL) && (StartSwipeCoords.IsNearlyZero() == false))
	{
		FHitResult Hit;
		if (Controller->GetHitResultAtScreenPosition(SwipePosition, COLLISION_PANCAMERA, true, Hit))
		{
			FVector NewSwipeCoords = Hit.ImpactPoint;
			FVector Delta = StartSwipeCoords - NewSwipeCoords;
			// Flatten Z axis - we are not interested in that.
			Delta.Z = 0.0f;
			if (Delta.IsNearlyZero() == false)
			{
				ASpectatorPawn* HydrSpectatorPawn = GetPlayerController()->GetSpectatorPawn();
				if (HydrSpectatorPawn != NULL)
				{
					FVector CurrentCamera = HydrSpectatorPawn->GetActorLocation();
					CurrentCamera += Delta;
					SetCameraTarget(CurrentCamera);
					bResult = true;
				}
			}
		}
	}
	return bResult;
}

bool UZoomCameraComponent::OnSwipeReleased(const FVector2D& SwipePosition)
{
	bool bResult = false;
	if (StartSwipeCoords.IsNearlyZero() == false)
	{
		APlayerController1* Controller = GetPlayerController();
		if (Controller)
		{
			FHitResult Hit;
			if (Controller->GetHitResultAtScreenPosition(SwipePosition, COLLISION_PANCAMERA, true, Hit))
			{
				FVector EndSwipeCoords = Hit.ImpactPoint;
				bResult = true;
			}
		}
		EndSwipeNow();
	}

	return false;
}

void UZoomCameraComponent::EndSwipeNow()
{
	StartSwipeCoords.Set(0.0f, 0.0f, 0.0f);
}

bool UZoomCameraComponent::AreCoordsInNoScrollZone(const FVector2D& SwipePosition)
{
	bool bResult = false;
	FVector MouseCoords(SwipePosition, 0.0f);
	for (int iZone = 0; iZone < NoScrollZones.Num(); iZone++)
	{
		FBox EachZone = NoScrollZones[iZone];
		if (EachZone.IsInsideXY(MouseCoords) == true)
		{
			bResult = true;
		}
	}
	return bResult;
}
