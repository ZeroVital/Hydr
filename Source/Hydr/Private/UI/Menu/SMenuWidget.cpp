#include "Hydr.h"
#include "SMenuWidget.h"
#include "Engine/Console.h"
#include "BattleHUD.h"
#include "MenuItem.h"
#include "SMenuItem.h"
#include "HUDSoundsWidgetStyle.h"
#include "MenuWidgetStyle.h"
#include "Runtime/Engine/Classes/Engine/Console.h"
#include "SDPIScaler.h"

#define LOCTEXT_NAMESPACE "SMenuWidget"

void SMenuWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &FHydrStyle::Get().GetWidgetStyle<FMenuStyle>("DefaultMenuStyle");
	
	bMenuHiding = false;
	bControlsLocked = false;
	SelectedIndex = 0;
	BattleHUD = InArgs._BattleHUD;
	Visibility.Bind(this, &SMenuWidget::GetVisibility);
	UIScale.Bind(this, &SMenuWidget::GetUIScale);
	const float BgPadding = 20.0f;
	MenuOffsetX = 0.0f;
	MenuOffsetY = 0.0f;

	ChildSlot
	[
		SNew(SDPIScaler)
		.DPIScale(UIScale)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(FMargin(5.0f))
				+ SUniformGridPanel::Slot(0, 0)
				[
					SNew(SImage)
					.Image(&MenuStyle->TileBackgroundBrush)
				]
				+ SUniformGridPanel::Slot(1, 0)
				[
					SNew(SImage)
					.Image(&MenuStyle->TileBackgroundBrush)
				]
				+ SUniformGridPanel::Slot(2, 0)
				[
					SNew(SImage)
					.Image(&MenuStyle->TileBackgroundBrush)
				]
				+ SUniformGridPanel::Slot(3, 0)
				[
					SNew(SImage)
					.Image(&MenuStyle->TileBackgroundBrush)
				]
				+ SUniformGridPanel::Slot(4, 0)
				[
					SNew(SImage)
					.Image(&MenuStyle->TileBackgroundBrush)
				]
				
			]
		]
	];

	//sets up whole main menu animations and launches them
	SetupAnimations();
}

float SMenuWidget::GetUIScale() const
{
	float Result = 0.5f;
	/*if ( GEngine && GEngine->GameViewport )
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		Result =  ViewportSize.X / 2048.0f;
	}*/
	//as a fall back assume 1024x768
	return Result;
}


void SMenuWidget::HideMenu()
{
	if (!bMenuHiding)
	{
		if(MenuWidgetAnimation.IsAtEnd())
		{
			MenuWidgetAnimation.PlayReverse(this->AsShared());
		}
		else
		{
			MenuWidgetAnimation.Reverse();
		}
		bMenuHiding = true;
	}
}

void SMenuWidget::SetupAnimations()
{
	const float StartDelay = 0.0f;
	const float SecondDelay = 0.3f;
	const float AnimDuration = 0.5f;
	const float MenuChangeDuration = 0.2f;
	MenuWidgetAnimation = FCurveSequence();
	LeftMenuWidgetAnimation = FCurveSequence();
	LeftMenuScrollOutCurve = LeftMenuWidgetAnimation.AddCurve(0,MenuChangeDuration,ECurveEaseFunction::QuadInOut);
	LeftMenuWidgetAnimation.Play(this->AsShared());

	//Define the fade in animation
	TopColorCurve = MenuWidgetAnimation.AddCurve(StartDelay, AnimDuration, ECurveEaseFunction::QuadInOut);

	//now, we want these to animate some time later

	//rolling out
	BottomScaleYCurve = MenuWidgetAnimation.AddCurve(StartDelay+SecondDelay, AnimDuration, ECurveEaseFunction::QuadInOut);
	//fading in
	BottomColorCurve = MenuWidgetAnimation.AddCurve(StartDelay+SecondDelay, AnimDuration, ECurveEaseFunction::QuadInOut);
	//moving from left side off screen
	ButtonsPosXCurve = MenuWidgetAnimation.AddCurve(StartDelay+SecondDelay, AnimDuration, ECurveEaseFunction::QuadInOut);

	FSlateApplication::Get().PlaySound(MenuStyle->MenuEnterSound);

}

void SMenuWidget::LockControls(bool bEnable)
{
	bControlsLocked = bEnable;
}

void SMenuWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	//Always tick the super
	SCompoundWidget::Tick( AllottedGeometry, InCurrentTime, InDeltaTime );
}


FVector2D SMenuWidget::GetBottomScale() const
{
	return FVector2D(BottomScaleYCurve.GetLerp(), BottomScaleYCurve.GetLerp());
}

FLinearColor SMenuWidget::GetBottomColor() const
{
	return FMath::Lerp(FLinearColor(1,1,1,0), FLinearColor(1,1,1,1), BottomColorCurve.GetLerp());
}

FLinearColor SMenuWidget::GetTopColor() const
{
	return FMath::Lerp(FLinearColor(1,1,1,0), FLinearColor(1,1,1,1), TopColorCurve.GetLerp());
}

FReply SMenuWidget::ButtonClicked(int32 ButtonIndex)
{
	if (bControlsLocked)
	{
		return FReply::Handled();
	}

	if (SelectedIndex != ButtonIndex)
	{
		SelectedIndex = ButtonIndex;

		const FHUDSoundsStyle& HUDSounds = FHydrStyle::Get().GetWidgetStyle<FHUDSoundsStyle>("DefaultHUDSoundsStyle");
		FSlateApplication::Get().PlaySound(HUDSounds.MenuItemChangeSound);
	}

	return FReply::Unhandled();
}

void SMenuWidget::FadeIn()
{
	//Start the menu widget playing
	MenuWidgetAnimation.Play(this->AsShared());

	//Go into UI mode
	FSlateApplication::Get().SetKeyboardFocus(SharedThis(this));
}

FReply SMenuWidget::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	//If we clicked anywhere, jump to the end
	if(MenuWidgetAnimation.IsPlaying())
	{
		MenuWidgetAnimation.JumpToEnd();
	}

	return FReply::Unhandled();
}

FReply SMenuWidget::OnFocusReceived(const FGeometry& Geometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().LockMouseToWidget(SharedThis(this));
}

EVisibility SMenuWidget::GetVisibility() const
{
	return bConsoleVisible ? EVisibility::HitTestInvisible : EVisibility::Visible;
}

#undef LOCTEXT_NAMESPACE
