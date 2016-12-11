#include "Hydr.h"
#include "SPortraitWidget.h"
#include "HydrGameHelpers.h"

void SPortraitWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	OnClicked = InArgs._OnClicked;
	OnClickedDisabled = InArgs._OnClickedDisabled;
	OnMouseEnterDel = InArgs._OnMouseEnterDel;
	OnMouseLeaveDel = InArgs._OnMouseLeaveDel;
	TextHAlign = InArgs._TextHAlign;
	TextVAlign = InArgs._TextVAlign;
	TextMargin = InArgs._TextMargin;
	TextFont = InArgs._TextFont;
	Opacity = InArgs._Opacity;

	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &SPortraitWidget::GetPortraitImage)
			.ColorAndOpacity(this, &SPortraitWidget::GetImageColor)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &SPortraitWidget::GetPortraitImage)
			.ColorAndOpacity(this, &SPortraitWidget::GetTintColor)
		]
		+ SOverlay::Slot()
		.HAlign(TextHAlign.Get().IsSet() ? TextHAlign.Get().GetValue() : EHorizontalAlignment::HAlign_Center)
		.VAlign(TextVAlign.Get().IsSet() ? TextVAlign.Get().GetValue() : EVerticalAlignment::VAlign_Bottom)
		.Padding(TAttribute<FMargin>(this, &SPortraitWidget::GetTextMargin))
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(this, &SPortraitWidget::GetTextShadowColor)
			.ColorAndOpacity(this, &SPortraitWidget::GetTextColor)
			.ShadowOffset(FIntPoint(-1, 1))
			.Font(this, &SPortraitWidget::GetTextFont)
			.Text(PortraitText)
		]
		+ SOverlay::Slot()
		[
			InArgs._Content.Widget
		]
	];
}

float SPortraitWidget::GetCurrentOpacity() const
{
	return (Opacity.Get().IsSet() ? Opacity.Get().GetValue() : 1.0f) * FMath::Lerp(0.0f, 1.0f, OpacityCurve.GetLerp());
}

FSlateColor	SPortraitWidget::GetImageColor() const
{
	return FLinearColor(1, 1, 1, GetCurrentOpacity());
}

FSlateColor	SPortraitWidget::GetTextColor() const
{
	const FLinearColor GoldColor = FLinearColor(FColor(255, 213, 160, GetCurrentOpacity() * 255));
	return GoldColor;
}

FLinearColor SPortraitWidget::GetTextShadowColor() const
{
	return FLinearColor(0, 0, 0, GetCurrentOpacity());
}



FMargin	SPortraitWidget::GetTextMargin() const
{
	return TextMargin.Get().IsSet() ? TextMargin.Get().GetValue() : FMargin(0, 0, 0, 15);
}

FSlateFontInfo SPortraitWidget::GetTextFont() const
{
	return TextFont.Get().IsSet() ? TextFont.Get().GetValue() : FHydrStyle::Get().GetFontStyle(".PortraitFont");
}

const FSlateBrush*  SPortraitWidget::GetPortraitImage() const
{
	if (PortraitImage.IsValid())
	{
		return PortraitImage.Get();
	}
	else
	{
		return FHydrStyle::Get().GetBrush("DefaultActionImageBrush");
	}
}

void SPortraitWidget::SetImage(UTexture2D* Texture)
{
	if (Texture != NULL)
	{
		PortraitImage.Reset();
		PortraitImage = TSharedPtr<FSlateDynamicImageBrush>(new FSlateDynamicImageBrush(Texture,
		FVector2D(Texture->GetSizeX(), Texture->GetSizeY()), Texture->GetFName()));
		AlphaMap.Reset();
		AlphaMap = FHydrGameHelpers::CreateAlphaMapFromTexture(Texture);
	}
}

void SPortraitWidget::SetActionAllowed(bool bEnabled)
{
	bIsActionAllowed = bEnabled;
}

void SPortraitWidget::SetActionActive(bool bIsActive)
{
	bIsActiveAction = bIsActive;
}

void SPortraitWidget::SetUserActionRequired(bool bIsWaitingForUser)
{
	bIsUserActionRequired = bIsWaitingForUser;
}


FSlateColor SPortraitWidget::GetTintColor() const
{
	FLinearColor ResultTint(0.f, 0.f, 0.f, 0.f);
	FLinearColor NotAllowedTint(0.6f, 0.13f, 0.06f, 0.6f);
	FLinearColor UserInputTint(0.0f, 0.6f, 0.0f, 0.3f);
	FLinearColor PressedTint(0.1f, 0.1f, 0.1f, 0.5f);
	FLinearColor ActiveTint(0.0f, 0.5f, 0.0f, 0.8f);

	if (!bIsActionAllowed)
	{
		ResultTint += NotAllowedTint;
	}
	if (bIsMouseButtonDown)
	{
		ResultTint += PressedTint;
	}
	if (bIsActiveAction)
	{
		ResultTint += ActiveTint;
	}
	if (bIsUserActionRequired)
	{
		ResultTint += UserInputTint;
		if (OwnerHUD.IsValid())
		{
			ResultTint.A = (1.0f + FMath::Sin(OwnerHUD->GetWorld()->GetTimeSeconds()*4.0f)) / 4.0f;
		}
	}
	return  FLinearColor(
		FMath::Clamp(ResultTint.R, 0.0f, 1.0f),
		FMath::Clamp(ResultTint.G, 0.0f, 1.0f),
		FMath::Clamp(ResultTint.B, 0.0f, 1.0f),
		FMath::Clamp(ResultTint.A * GetCurrentOpacity(), 0.0f, 1.0f));
}

FReply SPortraitWidget::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	bIsMouseButtonDown = true;
	return FReply::Handled();
}

FReply SPortraitWidget::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	// we handle this message so that the game doesn't get mouse moves without mouse downs/ups
	if (bIsMouseButtonDown)
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}


void SPortraitWidget::OnMouseEnter(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	OnMouseEnterDel.ExecuteIfBound(Geometry, MouseEvent);
}

void SPortraitWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsMouseButtonDown = false;
	OnMouseLeaveDel.ExecuteIfBound(MouseEvent);
}


FCursorReply SPortraitWidget::OnCursorQuery(const FGeometry& Geometry, const FPointerEvent& CursorEvent) const
{
	return FCursorReply::Cursor(EMouseCursor::Default);
}

FReply SPortraitWidget::OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (bIsMouseButtonDown)
	{
		bIsMouseButtonDown = false;
		if (bIsActionAllowed)
		{
			//execute our "OnClicked" delegate, if we have one
			if (OnClicked.IsBound() == true)
			{
				return OnClicked.Execute();
			}
		}
		else
		{
			//execute our "OnClickedDisabled" delegate, if we have one
			if (OnClickedDisabled.IsBound() == true)
			{
				return OnClickedDisabled.Execute();
			}
		}
	}
	return FReply::Handled();
}

void SPortraitWidget::DeferredShow()
{
	if (WidgetAnimation.IsAtStart() && !WidgetAnimation.IsPlaying())
	{
		WidgetAnimation.Play(this->AsShared());
	}
	else if (WidgetAnimation.IsInReverse())
	{
		WidgetAnimation.Reverse();
	}
}

void SPortraitWidget::DeferredHide(bool bInstant)
{
	if (bInstant)
	{
		WidgetAnimation.JumpToStart();
	}
	else
	{
		if (WidgetAnimation.IsAtEnd() && !WidgetAnimation.IsPlaying())
		{
			WidgetAnimation.PlayReverse(this->AsShared());
		}
		else if (WidgetAnimation.IsForward())
		{
			WidgetAnimation.Reverse();
		}
	}
}

bool SPortraitWidget::IsAnimating() const
{
	return WidgetAnimation.IsPlaying();
}