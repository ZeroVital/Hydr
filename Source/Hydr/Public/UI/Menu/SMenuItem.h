#pragma once

#include "SButtonWidget.h"

//class declare
class SMenuItem : public SButtonWidget
{
public:
	SLATE_BEGIN_ARGS(SMenuItem)
	{}

	SLATE_DEFAULT_SLOT(FArguments, Content)

	/** Called when the button is clicked */
	SLATE_EVENT(FOnClicked, OnClicked)

	SLATE_ATTRIBUTE(FText, Text)

	SLATE_END_ARGS()

	/** Needed for every widget */
	void Construct(const FArguments& InArgs);

	/** Says that we can support keyboard focus */
	virtual bool SupportsKeyboardFocus() const override { return true; }
};


