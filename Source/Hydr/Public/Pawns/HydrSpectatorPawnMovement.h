#pragma once

//#include "Types.h"
#include "HydrSpectatorPawnMovement.generated.h"

UCLASS()
class UHydrSpectatorPawnMovement : public USpectatorPawnMovement
{
	GENERATED_UCLASS_BODY()

		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	bool bInitialLocationSet;
};

