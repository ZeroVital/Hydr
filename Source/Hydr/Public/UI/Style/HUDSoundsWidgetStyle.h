#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "HUDSoundsWidgetStyle.generated.h"

/**
 * Represents the common HUD sounds used in the  game
 */
USTRUCT()
struct FHUDSoundsStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FHUDSoundsStyle();
	virtual ~FHUDSoundsStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FHUDSoundsStyle& GetDefault();

	/**
	 * The sound that should play when starting the game
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound StartGameSound;
	FHUDSoundsStyle& SetStartGameSound(const FSlateSound& InStartGameSound) { StartGameSound = InStartGameSound; return *this; }

	/**
	 * The sound that should play when exiting the game
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound ExitGameSound;
	FHUDSoundsStyle& SetExitGameSound(const FSlateSound& InExitGameSound) { ExitGameSound = InExitGameSound; return *this; }

	/**
	 * The sound that should play when changing the selected menu item
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound MenuItemChangeSound;
	FHUDSoundsStyle& SetMenuItemChangeSound(const FSlateSound& InMenuItemChangeSound) { MenuItemChangeSound = InMenuItemChangeSound; return *this; }
};


/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UHUDSoundsWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the sounds */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FHUDSoundsStyle SoundsStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &SoundsStyle );
	}
};
