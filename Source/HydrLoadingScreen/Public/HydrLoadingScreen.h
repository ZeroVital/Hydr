#pragma once

#include "ModuleInterface.h"


/** Module interface for this game's loading screens */
class IHydrLoadingScreenModule : public IModuleInterface
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen() = 0;
};
