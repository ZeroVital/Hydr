#include "Hydr.h"
#include "SButtonWidget.h"
#include "HydrGameHelpers.h"

void SButtonWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	ButtonText = InArgs._ButtonText;
	CenterText = InArgs._CenterText;
	CornerText = InArgs._CornerText;
	OnClicked = InArgs._OnClicked;
	OnClickedDisabled = InArgs._OnClickedDisabled;
	CoinIconVisible = InArgs._CoinIconVisible;
	TextHAlign = InArgs._TextHAlign; 
	TextVAlign = InArgs._TextVAlign; 
	TextMargin = InArgs._TextMargin;
	TextFont = InArgs._TextFont;
	Opacity = InArgs._Opacity;
	bIsUserActionRequired = false;
	bIsMouseButtonDown = false;
	bIsActiveAction = false;
	bIsActionAllowed = true;
	OnMouseEnterDel = InArgs._OnMouseEnterDel;
	OnMouseLeaveDel = InArgs._OnMouseLeaveDel;

	OpacityCurve = WidgetAnimation.AddCurve(0.0f, 0.2f, ECurveEaseFunction::QuadInOut);
	bMouseCursorVisible = true;


	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &SButtonWidget::GetButtonImage)
			.ColorAndOpacity(this,&SButtonWidget::GetImageColor)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &SButtonWidget::GetButtonImage)
			.ColorAndOpacity(this,&SButtonWidget::GetTintColor)
		]
		+SOverlay::Slot()
		.HAlign(TextHAlign.Get().IsSet() ? TextHAlign.Get().GetValue() : EHorizontalAlignment::HAlign_Center)
		.VAlign(TextVAlign.Get().IsSet() ? TextVAlign.Get().GetValue() : EVerticalAlignment::VAlign_Bottom)
		.Padding(TAttribute<FMargin>(this, &SButtonWidget::GetTextMargin))
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(this,&SButtonWidget::GetTextShadowColor)
			.ColorAndOpacity(this,&SButtonWidget::GetTextColor)
			.ShadowOffset(FIntPoint(-1,1))
			.Font(this, &SButtonWidget::GetTextFont)
			.Text(ButtonText)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(this,&SButtonWidget::GetTextShadowColor)
			.ColorAndOpacity(this,&SButtonWidget::GetTextColor)
			.ShadowOffset(FIntPoint(-1,1))
			.Font(this, &SButtonWidget::GetTextFont)
			.Text(CenterText)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Right)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(this,&SButtonWidget::GetTextShadowColor)
			.ColorAndOpacity(this,&SButtonWidget::GetTextColor)
			.ShadowOffset(FIntPoint(-1,1))
			.Text(CornerText)
		]
		+SOverlay::Slot()
		[
			InArgs._Content.Widget
		]
		
	];
}

float SButtonWidget::GetCurrentOpacity() const
{
	 return (Opacity.Get().IsSet() ? Opacity.Get().GetValue() : 1.0f) * FMath::Lerp(0.0f,1.0f,OpacityCurve.GetLerp());
}

FSlateColor	SButtonWidget::GetImageColor() const
{
	return FLinearColor(1,1,1, GetCurrentOpacity() );
}

FSlateColor	SButtonWidget::GetCoinColor() const
{
	return FLinearColor(1,1,1, GetCurrentOpacity() );
}

FSlateColor	SButtonWidget::GetTextColor() const
{
	const FLinearColor GoldColor = FLinearColor(FColor(255,213,160,GetCurrentOpacity()*255));
	return GoldColor;
}

FLinearColor SButtonWidget::GetTextShadowColor() const
{
	return FLinearColor(0,0,0, GetCurrentOpacity() );
}

EVisibility SButtonWidget::GetCoinVisibility() const
{
	return CoinIconVisible.Get().IsSet() ? CoinIconVisible.Get().GetValue() : EVisibility::Collapsed;
}

FMargin	SButtonWidget::GetTextMargin() const
{
	return TextMargin.Get().IsSet() ? TextMargin.Get().GetValue() : FMargin(0,0,0,15);
}

FSlateFontInfo SButtonWidget::GetTextFont() const
{
	return TextFont.Get().IsSet() ? TextFont.Get().GetValue() : FHydrStyle::Get().GetFontStyle(".ButtonFont");
}

const FSlateBrush*  SButtonWidget::GetButtonImage() const
{
	if (ButtonImage.IsValid())
	{
		return ButtonImage.Get();
	} 
	else 
	{
		return FHydrStyle::Get().GetBrush("DefaultActionImageBrush");
	}
}

void SButtonWidget::SetImage(UTexture2D* Texture)
{
	if (Texture != NULL)
	{
		ButtonImage.Reset();
		ButtonImage = TSharedPtr<FSlateDynamicImageBrush>(new FSlateDynamicImageBrush(Texture,
			FVector2D(Texture->GetSizeX(),Texture->GetSizeY()),Texture->GetFName()));
		AlphaMap.Reset();
		AlphaMap = FHydrGameHelpers::CreateAlphaMapFromTexture(Texture);
	}
}

void SButtonWidget::SetActionAllowed(bool bEnabled)
{
	bIsActionAllowed = bEnabled;
}

void SButtonWidget::SetActionActive(bool bIsActive)
{
	bIsActiveAction = bIsActive;
}

void SButtonWidget::SetUserActionRequired(bool bIsWaitingForUser)
{
	bIsUserActionRequired = bIsWaitingForUser;
}


FSlateColor SButtonWidget::GetTintColor() const
{
	FLinearColor ResultTint(0.f,0.f,0.f,0.f);
	FLinearColor NotAllowedTint(0.6f,0.13f,0.06f,0.6f);
	FLinearColor UserInputTint(0.0f,0.6f,0.0f,0.3f);
	FLinearColor PressedTint(0.1f,0.1f,0.1f,0.5f);
	FLinearColor ActiveTint(0.0f,0.5f,0.0f,0.8f);

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
			ResultTint.A = (1.0f+FMath::Sin(OwnerHUD->GetWorld()->GetTimeSeconds()*4.0f))/4.0f;
		}
	}
	return  FLinearColor( 
		FMath::Clamp(ResultTint.R, 0.0f, 1.0f ), 
		FMath::Clamp(ResultTint.G, 0.0f, 1.0f ),
		FMath::Clamp(ResultTint.B, 0.0f, 1.0f ),
		FMath::Clamp(ResultTint.A * GetCurrentOpacity(), 0.0f, 1.0f));
}

FReply SButtonWidget::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	bIsMouseButtonDown = true;
	return FReply::Handled();
}

FReply SButtonWidget::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	// we handle this message so that the game doesn't get mouse moves without mouse downs/ups
	if (bIsMouseButtonDown)
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}


void SButtonWidget::OnMouseEnter(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	OnMouseEnterDel.ExecuteIfBound(Geometry,MouseEvent);
}

void SButtonWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsMouseButtonDown = false;
	OnMouseLeaveDel.ExecuteIfBound(MouseEvent);
}


FCursorReply SButtonWidget::OnCursorQuery( const FGeometry& Geometry, const FPointerEvent& CursorEvent ) const
{
	return FCursorReply::Cursor(EMouseCursor::Default);
}

FReply SButtonWidget::OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (bIsMouseButtonDown)
	{
		bIsMouseButtonDown = false;
		if (bIsActionAllowed)
		{
			//execute our "OnClicked" delegate, if we have one
			if(OnClicked.IsBound() == true)
			{
				return OnClicked.Execute();
			}
		}
		else
		{
			//execute our "OnClickedDisabled" delegate, if we have one
			if(OnClickedDisabled.IsBound() == true)
			{
				return OnClickedDisabled.Execute();
			}
		}
	}
	return FReply::Handled();
}

void SButtonWidget::DeferredShow()
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

void SButtonWidget::DeferredHide(bool bInstant)
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

bool SButtonWidget::IsAnimating() const
{
	return WidgetAnimation.IsPlaying();
}