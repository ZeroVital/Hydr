#include "Hydr.h"

//#include "HUDSoundsWidgetStyle.h"
//#include "HUDWidgetStyle.h"
//#include "MenuWidgetStyle.h"



class FHydrModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		//Hot reload hack
		FSlateStyleRegistry::UnRegisterSlateStyle(FHydrStyle::GetStyleSetName());
		FHydrStyle::Initialize();
	}

	virtual void ShutdownModule() override
	{
		FHydrStyle::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FHydrModule, Module, "");

DEFINE_LOG_CATEGORY(LogGame)
