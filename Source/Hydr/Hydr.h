#pragma once
// Unreal Engine
#include "Engine.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SoundDefinitions.h"
// Hydr classes
#include "HydrGameState.h"
#include "HydrGameMode.h"
#include "PlayerController1.h"
#include "HydrHUD.h"
#include "HydrStyle.h"


DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);

/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel2
#define COLLISION_PANCAMERA		ECC_GameTraceChannel3