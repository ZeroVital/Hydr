#include "Hydr.h"
#include "SHUDMenuWidget.h"
#include "SActionGrid.h"
#include "SButtonWidget.h"
#include "HydrGameHelpers.h"
#include "HUDSoundsWidgetStyle.h"
#include "HUDWidgetStyle.h"
//#include "CheatManager.h"

#include "Engine/Console.h"


void SHUDMenuWidget::Construct(const FArguments& InArgs)
{
	HUDStyle = &FHydrStyle::Get().GetWidgetStyle<FHUDStyle>("DefaultHUDStyle");

	OwnerHUD = InArgs._OwnerHUD;
	bIsPauseMenuActive = false;
	PauseMenuButtons.AddZeroed((FPlatformProperties::SupportsQuit() ? 3 : 2));
	Visibility.Bind(this, &SHUDMenuWidget::GetSlateVisibility);
	UIScale.Bind(this, &SHUDMenuWidget::GetUIScale);
	MiniMapBorderMargin = 20;

	TSharedPtr<SVerticalBox> MenuBox;
	int32 ButtonIndex = 0;
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SDPIScaler)
		.DPIScale(UIScale)
		[

			SNew(SOverlay)
			+SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SCanvas)
				+SCanvas::Slot()
				.Position(TAttribute<FVector2D>(this,&SHUDMenuWidget::GetActionsWidgetPos))
				.Size(FVector2D(600,800))
				[
					SAssignNew(ActionButtonsWidget,SActionGrid)
					.OwnerHUD(OwnerHUD)
				]
			]
			+SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Center)
			[
				SNew(SBorder)
				.BorderImage(&HUDStyle->ResourcesBackgroundBrush)
				.Padding(FMargin(30.0f,10.0f))
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.WidthOverride(200)
					.HeightOverride(60)
					[
						SNew(SHorizontalBox)
						.Visibility(this, &SHUDMenuWidget::GetResourcesVisibility)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.TextStyle(FHydrStyle::Get(), ".ResourcesTextStyle")
							.Text(this, &SHUDMenuWidget::GetResourcesAmount)
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.VAlign(EVerticalAlignment::VAlign_Center)
							[
								SNew(SBox)
								.WidthOverride(48)
								.HeightOverride(48)
								[
									SNew(SImage)
									//.Image(&HUDStyle->ResourcesImage)
								]
							]
						]
					]
				]
			]
			+SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				SNew(STextBlock)
				.TextStyle(FHydrStyle::Get(), ".ResourcesTextStyle")
				.Text(this, &SHUDMenuWidget::GetGameTime)
			]
			/* Result screen { */
			+SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(SImage)
					.Image(this, &SHUDMenuWidget::GetGameResultImage)
					.Visibility(this, &SHUDMenuWidget::GetResultScreenVisibility)
				]

				+SOverlay::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.WidthOverride(675)
					.HeightOverride(310)
					[
						SNew(STextBlock)
						.Visibility(this, &SHUDMenuWidget::GetResultScreenVisibility)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ColorAndOpacity(this, &SHUDMenuWidget::GetGameResultColor)
						.ShadowOffset(FIntPoint(-1,1))
						.Font(this, &SHUDMenuWidget::GetGameResultFont)
						.Text(this, &SHUDMenuWidget::GetGameResultText)
					]
				]
			]

			/* Result screen } */
			+SOverlay::Slot() // pause menu background overlay
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SBorder)
				.Visibility(this, &SHUDMenuWidget::GetPauseMenuBgVisibility)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.BorderImage(FCoreStyle::Get().GetBrush("BlackBrush"))
				.BorderBackgroundColor(this, &SHUDMenuWidget::GetOverlayColor)
				[
					SNew(SOverlay)
					// Return to main
					+SOverlay::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SAssignNew(MenuBox, SVerticalBox)
						+SVerticalBox::Slot()
						[
							SAssignNew(PauseMenuButtons[ButtonIndex++], SButtonWidget)
							.OwnerHUD(OwnerHUD)
							.Visibility(EVisibility::Visible)
							.TextFont(FHydrStyle::Get().GetFontStyle(TEXT(".MenuFont")))
							.TextVAlign(EVerticalAlignment::VAlign_Center)
							.TextMargin(FMargin(0))
							.ButtonText(NSLOCTEXT("SHUDMenuWidget", "MainMenu", "Main Menu"))
							.OnClicked(this, &SHUDMenuWidget::OnReturnToMainMenu)
						]
					]
					
				]
			]
			+SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			.Padding(FMargin(0,0,20,20))
			[
				SAssignNew(PauseButton,SButtonWidget)
				.OwnerHUD(OwnerHUD)
				.Visibility(EVisibility::Visible)
				.OnClicked(this, &SHUDMenuWidget::TogglePauseMenu)
				.ButtonText(FText::GetEmpty())
			]
		]
	];

	{
		// Cheats
		MenuBox->AddSlot()
			[
				SAssignNew(PauseMenuButtons[ButtonIndex++], SButtonWidget)
				.OwnerHUD(OwnerHUD)
				.Visibility(EVisibility::Visible)
				.TextFont(FHydrStyle::Get().GetFontStyle(TEXT(".MenuFont")))
				.TextVAlign(EVerticalAlignment::VAlign_Center)
				.TextMargin(FMargin(0))
				.ButtonText(NSLOCTEXT("SHUDMenuWidget", "CheatGold", "Cheat-Gold"))
				.OnClicked(this, &SHUDMenuWidget::OnCheatAddGold)
			];	
	}

	if (FPlatformProperties::SupportsQuit())
	{
		MenuBox->AddSlot()
		[
			SAssignNew(PauseMenuButtons[ButtonIndex++], SButtonWidget)
			.OwnerHUD(OwnerHUD)
			.Visibility(EVisibility::Visible)
			.TextFont(FHydrStyle::Get().GetFontStyle(TEXT(".MenuFont")))
			.TextVAlign(EVerticalAlignment::VAlign_Center)
			.TextMargin(FMargin(0))
			.ButtonText(NSLOCTEXT("SHUDMenuWidget", "Exit", "Exit"))
			.OnClicked(this, &SHUDMenuWidget::OnExitGame)
		];
	}
}

FSlateColor SHUDMenuWidget::GetOverlayColor() const
{
	const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");

	FLinearColor Result(0,0,0,0.3f);
	const float PosExiting = FMath::Max(MenuHelper::GetSoundPlaybackPosition(OwnerHUD->PlayerOwner->GetWorld(),HUDSounds.ExitGameSound, ExitGameTimerHandle),
						     MenuHelper::GetSoundPlaybackPosition(OwnerHUD->PlayerOwner->GetWorld(),HUDSounds.ExitGameSound, ReturnToMainMenuTimerHandle));
	Result.A = 0.3f + 0.7f * PosExiting;
	if (PosExiting > 0.95f)
	{
		OwnerHUD->ShowBlackScreen();
	}
	return Result;
}

FReply SHUDMenuWidget::OnFocusReceived(const FGeometry& Geometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().LockMouseToWidget(SharedThis(this));
}

float SHUDMenuWidget::GetUIScale() const
{
	float Result = 0.5f;
	if ( GEngine && GEngine->GameViewport )
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		Result = ViewportSize.X / 1024.0f;
	}
	//as a fall back assume 1024x768
	return Result;
}

void SHUDMenuWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	SCompoundWidget::Tick( AllottedGeometry, InCurrentTime, InDeltaTime );
	//ugly code seeing if the console is open
	UConsole* ViewportConsole = (GEngine !=NULL && GEngine->GameViewport != NULL) ? GEngine->GameViewport->ViewportConsole : NULL;
	if (ViewportConsole != NULL && (ViewportConsole->ConsoleState == "Typing" || ViewportConsole->ConsoleState == "Open"))
	{
		if (!bConsoleVisible)
		{
			bConsoleVisible = true;
			FSlateApplication::Get().SetAllUserFocusToGameViewport();
		}
	}
	else
	{
		if (bConsoleVisible)
		{
			bConsoleVisible = false;
			FSlateApplication::Get().SetKeyboardFocus(SharedThis(this));
		}
	}
}

EVisibility SHUDMenuWidget::GetSlateVisibility() const
{
	return bConsoleVisible ? EVisibility::HitTestInvisible : EVisibility::Visible;
}


FCursorReply SHUDMenuWidget::OnCursorQuery( const FGeometry& Geometry, const FPointerEvent& CursorEvent ) const
{
	return FCursorReply::Cursor(EMouseCursor::Default);
}

FText SHUDMenuWidget::GetResourcesAmount() const
{
	const APlayerController1* const PC = Cast<APlayerController1>(OwnerHUD->PlayerOwner);
	AHydrGameState* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State && PC)
	{
		/*FPlayerData* const PlayerData = State->GetPlayerData(PC->GetTeamNum());
		if (PlayerData)
		{
			return FText::AsNumber(PlayerData->ResourcesAvailable);
		}*/
	}
	return FText::GetEmpty();
}

FText SHUDMenuWidget::GetGameTime() const
{
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State != NULL)
	{
		if (State->GameplayState == EGameplayState::Waiting)
		{
			int32 const DisplaySecondsRemaining = FMath::CeilToInt(State->GetRemainingWaitTime());
			return FText::Format( NSLOCTEXT("GameFlow", "GameStartsIn", "Game starts in {0}"), FText::AsNumber(DisplaySecondsRemaining) );
		}
	}
	return FText::GetEmpty();
}

FSlateFontInfo SHUDMenuWidget::GetGameResultFont() const
{
	FSlateFontInfo ResultFont;
	const float AnimTime = 1.0f;
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	const float GameFinishedTime = State ? State->GetGameFinishedTime() : 0.0f;
	float AnimPercentage = FMath::Min(1.0f, (OwnerHUD->GetWorld()->GetRealTimeSeconds() - GameFinishedTime) / AnimTime);
	if (GameFinishedTime > 0)
	{
		const int32 StartFontSize = 8;
		const int32 AnimatedFontSize = 70;
		ResultFont = FSlateFontInfo( FPaths::GameContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), FMath::TruncToInt(StartFontSize + AnimatedFontSize * AnimPercentage));
	}
	else
	{
		ResultFont = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));
	}
	return ResultFont;
}

FSlateColor SHUDMenuWidget::GetGameResultColor() const
{
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	return (State && State->GetWinningTeam() == ETeam::Player) ? HUDStyle->VictoryTextColor : HUDStyle->DefeatTextColor;
}

FText SHUDMenuWidget::GetGameResultText() const
{
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State != NULL)
	{
		return State->GetWinningTeam() == ETeam::Player ? NSLOCTEXT("GameFlow", "GameWon", "VICTORY") : NSLOCTEXT("GameFlow", "GameLost", "DEFEAT");
	}
	return FText::GetEmpty();
}

const FSlateBrush* SHUDMenuWidget::GetGameResultImage() const
{
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	bool const bVictory = (State && State->GetWinningTeam() == ETeam::Player);
	return bVictory ?  &HUDStyle->VictoryImage : &HUDStyle->DefeatImage;
}

EVisibility SHUDMenuWidget::GetResourcesVisibility() const
{
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	return State && State->IsGameActive() ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SHUDMenuWidget::GetResultScreenVisibility() const
{
	EVisibility ResultVisibility = EVisibility::Collapsed;
	AHydrGameState const* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if ( State && (State->GameplayState == EGameplayState::Finished) )
	{
		ResultVisibility = EVisibility::Visible;
	}

	return ResultVisibility;
}

EVisibility SHUDMenuWidget::GetPauseMenuBgVisibility() const
{
	return bIsPauseMenuActive ? EVisibility::Visible : EVisibility::Collapsed;
}

FVector2D SHUDMenuWidget::GetActionsWidgetPos() const
{
	return ActionWidgetPosition.Get();
}

FReply SHUDMenuWidget::TogglePauseMenu()
{
	AHydrGameState* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State != NULL)
	{
		const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
		FSlateApplication::Get().PlaySound(HUDSounds.MenuItemChangeSound);
		bIsPauseMenuActive = !bIsPauseMenuActive;
		//Do not trigger pause when game is already finished
		if (State->GameplayState != EGameplayState::Finished)
		{
			State->SetGamePaused(bIsPauseMenuActive);
		}
		for (uint8 i = 0; i < PauseMenuButtons.Num(); i++)
		{
			if (bIsPauseMenuActive)
			{
				PauseMenuButtons[i]->DeferredShow();
			}
			else 
			{
				PauseMenuButtons[i]->DeferredHide();
			}
		}
	}
	return FReply::Handled();
}

bool SHUDMenuWidget::IsPauseMenuUp() const
{
	return bIsPauseMenuActive;
}

FReply SHUDMenuWidget::OnExitGame()
{
	AHydrGameState* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State)
	{
		State->SetGamePaused(false);
	}

	for (int32 i = 0; i < PauseMenuButtons.Num(); i++)
	{
		PauseMenuButtons[i]->DeferredHide();
	}
	const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
	ExitGameTimerHandle = MenuHelper::PlaySoundAndCallSP(OwnerHUD->PlayerOwner->GetWorld(), HUDSounds.ExitGameSound, this, &SHUDMenuWidget::ExitGame);
	return FReply::Handled();
}


void SHUDMenuWidget::ExitGame() const
{
	AHydrGameMode* const GameMode = OwnerHUD->GetWorld()->GetAuthGameMode<AHydrGameMode>();
	if (GameMode != NULL)
	{
		GameMode->ExitGame();
	}
}

FReply SHUDMenuWidget::OnReturnToMainMenu()
{
	AHydrGameState* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State)
	{
		State->SetGamePaused(false);
	}

	for (uint8 i = 0; i < PauseMenuButtons.Num(); i++)
	{
		PauseMenuButtons[i]->DeferredHide();
	}
	const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
	ReturnToMainMenuTimerHandle = MenuHelper::PlaySoundAndCallSP(OwnerHUD->PlayerOwner->GetWorld(), HUDSounds.ExitGameSound, this, &SHUDMenuWidget::ReturnToMainMenu);
	return FReply::Handled();
}

void SHUDMenuWidget::ReturnToMainMenu() const
{
	AHydrGameMode* const GameMode = OwnerHUD->GetWorld()->GetAuthGameMode<AHydrGameMode>();
	if (GameMode != NULL)
	{
		GameMode->ReturnToMenu();
	}
}

FReply SHUDMenuWidget::OnCheatAddGold() const
{
	FReply Reply = FReply::Unhandled();
	APlayerController1* PlayerController1 = Cast<APlayerController1>(OwnerHUD->GetWorld()->GetFirstPlayerController());
	if (PlayerController1)
	{
		/*UCheatManager* CheatManager = Cast<UCheatManager>(PlayerController1->CheatManager);
		if (CheatManager != nullptr)
		{
			CheatManager->AddGold(10000);
			Reply = FReply::Handled();
		}*/
	}
	return Reply;
}

