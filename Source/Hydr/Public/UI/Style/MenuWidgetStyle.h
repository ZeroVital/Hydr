#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "MenuWidgetStyle.generated.h"

/**
 * Represents the the appearance of an SMenuWidget
 */
USTRUCT()
struct FMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FMenuStyle();
	virtual ~FMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FMenuStyle& GetDefault();

	/**
	 * The brush used to draw the menu background
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush BackgroundBrush;
	FMenuStyle& SetBackgroundBrush(const FSlateBrush& InBackgroundBrush) { BackgroundBrush = InBackgroundBrush; return *this; }

	/**
	 * The brush used to draw the menu tile background
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush TileBackgroundBrush;
	FMenuStyle& SetTileBackgroundBrush(const FSlateBrush& InTileBackgroundBrush) { TileBackgroundBrush = InTileBackgroundBrush; return *this; }

	/**
	 * The brush used to draw the pop-up menu background
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush PopupBackgroundBrush;
	FMenuStyle& SetPopupBackgroundBrush(const FSlateBrush& InPopupBackgroundBrush) { PopupBackgroundBrush = InPopupBackgroundBrush; return *this; }

	/**
	 * The brush used to draw the top menu decoration
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush TopDecoration;
	FMenuStyle& SetTopDecoration(const FSlateBrush& InTopDecoration) { TopDecoration = InTopDecoration; return *this; }

	/**
	 * The brush used to draw the left menu decoration
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush LeftDecoration;
	FMenuStyle& SetLeftDecoration(const FSlateBrush& InLeftDecoration) { LeftDecoration = InLeftDecoration; return *this; }

	/**
	 * The brush used to draw the right menu decoration
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush RightDecoration;
	FMenuStyle& SetRightDecoration(const FSlateBrush& InRightDecoration) { RightDecoration = InRightDecoration; return *this; }

	/**
	 * The sound that should play when entering a sub-menu
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound MenuEnterSound;
	FMenuStyle& SetMenuEnterSound(const FSlateSound& InMenuEnterSound) { MenuEnterSound = InMenuEnterSound; return *this; }

	/**
	 * The sound that should play when leaving a sub- menu
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound MenuExitSound;
	FMenuStyle& SetMenuExitSound(const FSlateSound& InMenuExitSound) { MenuExitSound = InMenuExitSound; return *this; }
};


/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the menu's appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FMenuStyle MenuStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &MenuStyle );
	}
};
