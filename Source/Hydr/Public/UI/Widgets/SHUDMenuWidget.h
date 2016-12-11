#pragma once

#include "SActionGrid.h"
#include "HUDWidgetStyle.h"

class SButtonWidget;
class AHydrHUD;

//HUD widget base class
class SHUDMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHUDMenuWidget)
	: _OwnerHUD()
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<AHydrHUD>, OwnerHUD)

	SLATE_END_ARGS()

	/** Needed for every widget */
	void Construct(const FArguments& InArgs);

	/** If we want to recieve mouse/keyboard events */
	virtual bool SupportsKeyboardFocus() const override { return true; }

	/** returns what cursor we want when on this widget */
	virtual FCursorReply OnCursorQuery( const FGeometry& Geometry, const FPointerEvent& CursorEvent ) const override;

	/** The menu sets up the appropriate mouse settings upon focus */
	virtual FReply OnFocusReceived(const FGeometry& Geometry, const FFocusEvent& InFocusEvent) override;

	/** executed on every frame */
	void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;

	/** returns if pause menu is currently active */
	bool IsPauseMenuUp() const;

	/** callback function for toggling pause menu */
	FReply TogglePauseMenu();

	/** action buttons widget */
	TSharedPtr<SActionGrid> ActionButtonsWidget;

	/** screen position of action grid widget */
	TAttribute<FVector2D> ActionWidgetPosition;

	/** Button that toggles pause menu */
	TSharedPtr<SButtonWidget> PauseButton;

	/** Buttons inside the pause menu */
	TArray< TSharedPtr<SButtonWidget> > PauseMenuButtons;


protected:

	/** sets hit test invisibility when console is up */
	EVisibility GetSlateVisibility() const;

	/** gets current scale for drawing menu */
	float GetUIScale() const;

	/** returns screen position of action grid widget */
	FVector2D GetActionsWidgetPos() const;

	/** callback function for pause menu exit button */
	FReply OnExitGame();

	/** callback function for pause menu return to main menu button */
	FReply OnReturnToMainMenu();

	/** callback function add gold cheat  */
	FReply OnCheatAddGold() const;

	/** exits the game */
	void ExitGame() const;

	/** returns to the main menu */
	void ReturnToMainMenu() const;

	/** should we display pause menu? */
	EVisibility GetPauseMenuBgVisibility() const;

	/** should we display resource amount? */
	EVisibility GetResourcesVisibility() const;

	/** should we display game results? */
	EVisibility GetResultScreenVisibility() const;

	/** returns resources amount to display */
	FText GetResourcesAmount() const;

	/** returns game timer information to display */
	FText GetGameTime() const;

	/** returns game result font (used for animation) */
	FSlateFontInfo GetGameResultFont() const;

	/** returns game result string to display */
	FText GetGameResultText() const;

	/** returns game result text font color */
	FSlateColor GetGameResultColor() const;

	/** displays win/lose logo */
	const FSlateBrush* GetGameResultImage() const;

	/** gets mini map width */
	FOptionalSize GetMiniMapWidth() const;

	/** gets mini map height */
	FOptionalSize GetMiniMapHeight() const;

	/** gets game menu overlay color and animates it */
	FSlateColor GetOverlayColor() const;

	/** is pause menu active? */
	bool bIsPauseMenuActive;

	/** margin for border */
	float MiniMapBorderMargin;

	/** current UI scale attribute */
	TAttribute<float> UIScale;

	/** if console is currently opened */
	bool bConsoleVisible;

	/** Pointer to our parent HUD */
	TWeakObjectPtr<AHydrHUD> OwnerHUD;

	/** style for this HUD */
	const struct FHUDStyle* HUDStyle;

private:
	/** Handles to various registered timers */
	FTimerHandle ExitGameTimerHandle;
	FTimerHandle ReturnToMainMenuTimerHandle;
};


