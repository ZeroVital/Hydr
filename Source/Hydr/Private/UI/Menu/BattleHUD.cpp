#include "Hydr.h"
#include "BattleHUD.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SMenuWidget.h"
#include "MenuItem.h"
#include "HydrGameHelpers.h"
#include "HydrLoadingScreen.h"
#include "HUDSoundsWidgetStyle.h"


#define LOCTEXT_NAMESPACE "Hydr.HUD.Menu"

ABattleHUD::ABattleHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MenuButtonTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/MainMenu/MenuButton.MenuButton"), nullptr, LOAD_None, nullptr);

	AddMenuItem(MainMenu,LOCTEXT("Start", "START"))->OnConfirmMenuItem.BindUObject(this, &ABattleHUD::ExecuteEnterSelectScreen);

	if (FPlatformProperties::SupportsQuit())
	{
		AddMenuItem(MainMenu, LOCTEXT("Quit", "QUIT"))->OnConfirmMenuItem.BindUObject(this, &ABattleHUD::ExecuteQuitAction);
	}

}

FMenuItem* ABattleHUD::AddMenuItem(TSharedPtr<TArray<class FMenuItem>> &Menu, FText Text)
{
	if (!Menu.IsValid())
	{
		Menu = MakeShareable(new TArray<FMenuItem>());
	}
	FMenuItem MenuItem(Text);
	Menu->Add(MenuItem);
	return &Menu->Last();
}

void ABattleHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//Now that we are here, build our menu widget
	RebuildWidgets();
}

void ABattleHUD::ExecuteQuitAction()
{
	MenuWidget->HideMenu();
	const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
	MenuHelper::PlaySoundAndCall(PlayerOwner->GetWorld(),HUDSounds.ExitGameSound,this, &ABattleHUD::Quit);
	MenuWidget->LockControls(true);
}

void ABattleHUD::Quit()
{
	GetOwningPlayerController()->ConsoleCommand("quit");
}

void ABattleHUD::ExecuteEnterSelectScreen()
{
	MenuWidget->HideMenu();
	MenuWidget->LockControls(true);
	const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
	MenuHelper::PlaySoundAndCall(PlayerOwner->GetWorld(),HUDSounds.StartGameSound,this,&ABattleHUD::EnterSelectScreen);
}

void ABattleHUD::EnterSelectScreen()
{
	FString StartStr = FString::Printf(TEXT("/Game/Maps/SelectScreenMap"));
	GetWorld()->ServerTravel(StartStr);
	ShowLoadingScreen();
}

void ABattleHUD::RebuildWidgets(bool bHotReload)
{
	MenuWidget.Reset();

	if (GEngine && 
		GEngine->GameViewport)
	{
		UGameViewportClient* GVC = GEngine->GameViewport;
		SAssignNew(MenuWidget, SMenuWidget)
			.Cursor(EMouseCursor::Default)
			.BattleHUD(TWeakObjectPtr<ABattleHUD>(this));

		GVC->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MenuWidget.ToSharedRef()));
	}
}

void ABattleHUD::ShowLoadingScreen()
{
	IHydrLoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<IHydrLoadingScreenModule>("LoadingScreen");
	if( LoadingScreenModule != nullptr )
	{
		LoadingScreenModule->StartInGameLoadingScreen();
	}
}

#undef LOCTEXT_NAMESPACE