#include "Hydr.h"
#include "MenuPlayerController.h"
#include "Camera/PlayerCameraManager.h"

AMenuPlayerController::AMenuPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// just use the normal camera...fine for now
	PlayerCameraManagerClass = APlayerCameraManager::StaticClass();
}

void AMenuPlayerController::SetupInputComponent()
{
	// Skip APlayerController1::SetupInputComponent
	APlayerController::SetupInputComponent();
}