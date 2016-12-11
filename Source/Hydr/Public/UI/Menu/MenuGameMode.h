#pragma once

#include "MenuGameMode.generated.h"

UCLASS(BlueprintType)
class AMenuGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
public:

	// Begin GameMode interface
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, class AActor* StartSpot) override;
	// End GameMode interface
};

