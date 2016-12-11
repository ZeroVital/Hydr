#pragma once

#include "Types.h"
#include "HydrSpectatorPawn.generated.h"
//@TODO: Write a comment here
UCLASS(Blueprintable, BlueprintType)
class AHydrSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()

		// Begin ADefaultPawn interface

		/** event call on move forward input */
		virtual void MoveForward(float Val) override;

	/** event call on strafe right input */
	virtual void MoveRight(float Val) override;

	/** Add custom key bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// End ADefaultPawn interface

	// The camera component for this camera
private:
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UZoomCameraComponent* CameraComponent;
public:

	/** Handles the mouse scrolling down. */
	void OnMouseScrollUp();

	/** Handles the mouse scrolling up. */
	void OnMouseScrollDown();

	/* Returns a pointer to the  camera component the pawn has. */
	UZoomCameraComponent* GetCameraComponent();
};

