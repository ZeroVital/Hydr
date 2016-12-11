#pragma once

#include "HeroesSelectionInterface.generated.h"

/** Interface for actors which receive notifies about their selected state */
UINTERFACE()
class UHeroesSelectionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IHeroesSelectionInterface
{
	GENERATED_IINTERFACE_BODY()

	/** tries to select actor */
	UFUNCTION(BlueprintNativeEvent)
	bool OnSelectionGained();

	/** tries to deselect actor */
	UFUNCTION(BlueprintNativeEvent)
	bool OnSelectionLost(const FVector& NewPosition, AActor* NewActor);
};
