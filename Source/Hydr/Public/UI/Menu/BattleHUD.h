#pragma once

#include "BattleHUD.generated.h"

UCLASS()
class ABattleHUD : public AHUD
{
	GENERATED_UCLASS_BODY()
public:

	/** Main menu. */
	TSharedPtr<TArray<class FMenuItem>> MainMenu;

	// Begin HUD interface
	virtual void PostInitializeComponents() override;
	// End HUD interface

	/** 
	 * Clears out the old widgets, rebuilds them
	 *
	 * @param	bHotRelead	Not used.
	 */
	void RebuildWidgets(bool bHotReload = false);

	/** Menu button texture. */
	UPROPERTY()
	UTexture2D* MenuButtonTexture;

private:

	/** Menu widget pointer. */
	TSharedPtr<class SMenuWidget> MenuWidget;

	/** 
	 * Helper for adding menu items .
	 *
	 * @param	SubMenu	The item to add.
	 * @param	Text	The item text.
	 */
	FMenuItem* AddMenuItem(TSharedPtr<TArray<class FMenuItem>> &SubMenu, FText Text);

	/** Callback for quit button. */
	void ExecuteQuitAction();

	 /** Callback for start button. */
	void ExecuteEnterSelectScreen();

	/** Starts the game with selected difficulty. */
	void EnterSelectScreen();

	/** Quits the game. */
	void Quit();

	/** Show the loading screen. */
	void ShowLoadingScreen();
};
