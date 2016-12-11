#pragma once

#include "MenuPlayerController.generated.h"

UCLASS(config=Game)
class AMenuPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
public:

	// Begin Controller interface
	/** We know we won't have a pawn, so we don't care */
	virtual void FailedToSpawnPawn() override {}
	// End Controller interface

protected:

	virtual void SetupInputComponent() override;
};

