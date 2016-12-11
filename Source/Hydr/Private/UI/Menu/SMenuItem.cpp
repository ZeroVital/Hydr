#include "Hydr.h"
#include "SMenuItem.h"

void SMenuItem::Construct(const FArguments& InArgs)
{
	 SButtonWidget::Construct(
		 SButtonWidget::FArguments()
		.ButtonText(InArgs._Text)
		.TextFont(FHydrStyle::Get().GetFontStyle(TEXT(".MenuFont")))
		.TextVAlign(EVerticalAlignment::VAlign_Center)
		.TextMargin(FMargin(0))
		.Visibility(EVisibility::Visible)
		.OnClicked(InArgs._OnClicked)
		[
			InArgs._Content.Widget
		]
		);
}