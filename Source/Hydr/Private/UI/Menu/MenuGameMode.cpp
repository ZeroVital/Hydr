#include "Hydr.h"
#include "MenuGameMode.h"
#include "MenuPlayerController.h"
#include "BattleHUD.h"
#include "HydrSpectatorPawn.h"

AMenuGameMode::AMenuGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//setup our custom PC and HUD
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = ABattleHUD::StaticClass();
	SpectatorClass = AHydrSpectatorPawn::StaticClass();
}

APawn* AMenuGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, class AActor* StartSpot)
{
	return NULL;
}

