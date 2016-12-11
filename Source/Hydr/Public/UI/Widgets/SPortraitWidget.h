#pragma once

#include "Types.h"

DECLARE_DELEGATE_TwoParams(FOnMouseEnter, const FGeometry&, const FPointerEvent&);
DECLARE_DELEGATE_OneParam(FOnMouseLeave, const FPointerEvent&);

class AHydrHUD;

class SPortraitWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPortraitWidget)
	{}
	/* Owning HUD for getting Game World */
	SLATE_ARGUMENT(TWeakObjectPtr<AHydrHUD>, OwnerHUD)

	SLATE_DEFAULT_SLOT(FArguments, Content)

	SLATE_EVENT(FOnClicked, OnClicked)
	SLATE_EVENT(FOnClicked, OnClickedDisabled)
	SLATE_EVENT(FOnMouseEnter, OnMouseEnterDel)
	SLATE_EVENT(FOnMouseLeave, OnMouseLeaveDel)

	/** text on the button */
	SLATE_ATTRIBUTE(FText, PortraitText)
	/** horizontal alignment of the text on the button (default is centered)*/
	SLATE_ATTRIBUTE(TOptional<EHorizontalAlignment>, TextHAlign)
	/** alignment of the text on the button (default is at the bottom) */
	SLATE_ATTRIBUTE(TOptional<EVerticalAlignment>, TextVAlign)
	/** margins of the button text, (default is 10 points bottom margin) */
	SLATE_ATTRIBUTE(TOptional<FMargin>, TextMargin)
	/** font type and size in the button */
	SLATE_ATTRIBUTE(TOptional<FSlateFontInfo>, TextFont)
	/** button opacity */
	SLATE_ATTRIBUTE(TOptional<float>, Opacity)
	/** should button hide mouse when leaving button area */
	SLATE_ATTRIBUTE(TOptional<bool>, HideMouse)
	SLATE_END_ARGS()

	/** needed for every widget */
	void Construct(const FArguments& InArgs);
	/** says that we can support keyboard focus */
	virtual bool SupportsKeyboardFocus() const override { return true; }
	/** widget sets up the appropriate mouse settings upon focus */
	virtual FReply OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FCursorReply OnCursorQuery(const FGeometry& Geometry, const FPointerEvent& CursorEvent) const override;

	void SetImage(UTexture2D* Texture);
	void SetActionAllowed(bool bIsAllowed);
	void SetActionActive(bool bIsActive);
	void SetUserActionRequired(bool bIsWaitingForUser);

	void DeferredShow();
	void DeferredHide(bool bInstant = false);
	bool IsAnimating() const;

	/** brush resource that represents a button */
	TSharedPtr<FSlateDynamicImageBrush> PortraitImage;
	TSharedPtr<TArray<uint8>> AlphaMap;

protected:
	/** the delegate to execute when the button is clicked */
	FOnClicked OnClicked;
	/** the delegate to execute when the button is clicked, when action is not allowed */
	FOnClicked OnClickedDisabled;
	/** the delegate to execute when mouse enters active button area */
	FOnMouseEnter OnMouseEnterDel;
	/** the delegate to execute when mouse leave active button area */
	FOnMouseLeave OnMouseLeaveDel;

	const FSlateBrush* GetPortraitImage() const;
	FSlateColor GetTintColor() const;
	FSlateColor GetImageColor() const;
	FSlateColor GetTextColor() const;
	FLinearColor GetTextShadowColor() const;
	FMargin GetTextMargin() const;
	FSlateFontInfo GetTextFont() const;
	float GetCurrentOpacity() const;

	TAttribute<FText> PortraitText;
	TAttribute< TOptional<EHorizontalAlignment> > TextHAlign;
	TAttribute< TOptional<EVerticalAlignment> >	TextVAlign;
	TAttribute< TOptional<FMargin> > TextMargin;
	TAttribute< TOptional<FSlateFontInfo> >	TextFont;
	TAttribute< TOptional<float> > Opacity;
	TAttribute< TOptional<bool> > HideMouse;

	FCurveSequence WidgetAnimation;
	FCurveHandle OpacityCurve;

	bool bIsMouseButtonDown;
	bool bIsActionAllowed;
	bool bIsActiveAction;
	bool bIsUserActionRequired;
	bool bMouseCursorVisible;

	/** Pointer to our parent HUD */
	TWeakObjectPtr<class AHydrHUD> OwnerHUD;
};