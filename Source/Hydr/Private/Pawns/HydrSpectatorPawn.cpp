#include "Hydr.h"
#include "HydrSpectatorPawn.h"
#include "ZoomCameraComponent.h"
#include "HydrSpectatorPawnMovement.h"

AHydrSpectatorPawn::AHydrSpectatorPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UHydrSpectatorPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	CameraComponent = CreateDefaultSubobject<UZoomCameraComponent>(TEXT("CameraComponent"));
}

void AHydrSpectatorPawn::OnMouseScrollUp()
{
	CameraComponent->OnZoomIn();
}

void AHydrSpectatorPawn::OnMouseScrollDown()
{
	CameraComponent->OnZoomOut();
}


void AHydrSpectatorPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AHydrSpectatorPawn::OnMouseScrollUp);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AHydrSpectatorPawn::OnMouseScrollDown);

	InputComponent->BindAxis("MoveForward", this, &AHydrSpectatorPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHydrSpectatorPawn::MoveRight);
}


void AHydrSpectatorPawn::MoveForward(float Val)
{
	CameraComponent->MoveForward(Val);
}


void AHydrSpectatorPawn::MoveRight(float Val)
{
	CameraComponent->MoveRight(Val);
}

UZoomCameraComponent* AHydrSpectatorPawn::GetCameraComponent()
{
	check(CameraComponent != NULL);
	return CameraComponent;
}
