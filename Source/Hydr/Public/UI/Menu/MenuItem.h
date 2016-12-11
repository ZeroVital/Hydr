#pragma once

class FMenuItem
{
public:

	/** menu item text */
	FText Text;

	/** sub menu if present */
	TSharedPtr<TArray<FMenuItem>> SubMenu;

	/** shared pointer to actual slate widget representing the menu item */
	TSharedPtr<class SMenuItem> Widget;

	/** confirm menu item delegate */
	DECLARE_DELEGATE(FOnConfirmMenuItem);

	FOnConfirmMenuItem OnConfirmMenuItem;

	/** constructor accepting menu item text */
	FMenuItem(FText _text)
	{
		Text = _text;
	}
};
