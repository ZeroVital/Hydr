#pragma once

#include "HydrInputInterface.generated.h"

/** Interface for actors which can be selected */
UINTERFACE()
class UHydrInputInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IHydrInputInterface
{
	GENERATED_IINTERFACE_BODY()

	/** receive input: tap */
	UFUNCTION(BlueprintNativeEvent)
	void OnInputTap();

	/** receive input: hold */
	UFUNCTION(BlueprintNativeEvent)
	void OnInputHold();

	/** receive input: hold released */
	UFUNCTION(BlueprintNativeEvent)
	void OnInputHoldReleased(float DownTime);

	/** receive input: swipe update (world space, not screen space)*/
	UFUNCTION(BlueprintNativeEvent)
	void OnInputSwipeUpdate(const FVector& DeltaPosition);

	/** receive input: swipe finished (world space, not screen space) */
	UFUNCTION(BlueprintNativeEvent)
	void OnInputSwipeReleased(const FVector& DeltaPosition, float DownTime);
};
