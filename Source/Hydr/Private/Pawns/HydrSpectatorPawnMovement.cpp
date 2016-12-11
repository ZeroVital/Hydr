#include "Hydr.h"
#include "HydrSpectatorPawnMovement.h"
#include "ZoomCameraComponent.h"
#include "HydrSpectatorPawn.h"


UHydrSpectatorPawnMovement::UHydrSpectatorPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bInitialLocationSet(false)
{
	MaxSpeed = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}


void UHydrSpectatorPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	APlayerController1* PlayerController1 = Cast<APlayerController1>(PawnOwner->GetController());
	if (PlayerController1 && PlayerController1->IsLocalController())
	{
		if (!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController1->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController1->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector Location = UpdatedComponent->GetComponentLocation();
		AHydrSpectatorPawn* HydrSpectatorPawn = Cast<AHydrSpectatorPawn>(PlayerController1->GetSpectatorPawn());
		if ((HydrSpectatorPawn != NULL) && (HydrSpectatorPawn->GetCameraComponent() != NULL))
		{
			HydrSpectatorPawn->GetCameraComponent()->ClampCameraLocation(PlayerController1, Location);
		}
		UpdatedComponent->SetWorldLocation(Location, false);
	}
}

