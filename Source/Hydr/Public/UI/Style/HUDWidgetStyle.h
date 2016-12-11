#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "HUDWidgetStyle.generated.h"

/**
 * Represents the appearance of an SHUDMenuWidget
 */
USTRUCT()
struct FHUDStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FHUDStyle();
	virtual ~FHUDStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FHUDStyle& GetDefault();

	/**
	 * The brush used to draw the resources area background
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush ResourcesBackgroundBrush;
	FHUDStyle& SetResourcesBackgroundBrush(const FSlateBrush& InResourcesBackgroundBrush) { ResourcesBackgroundBrush = InResourcesBackgroundBrush; return *this; }

	/**
	 * The brush used to draw the resources image
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush ResourcesImage;
	FHUDStyle& SetResourcesImage(const FSlateBrush& InResourcesImage) { ResourcesImage = InResourcesImage; return *this; }

	/**
	 * The brush used to draw the frame around the mini-map
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush MinimapFrameBrush;
	FHUDStyle& SetMinimapFrameBrush(const FSlateBrush& InMinimapFrameBrush) { MinimapFrameBrush = InMinimapFrameBrush; return *this; }

	/**
	 * The brush used to draw the victory image
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush VictoryImage;
	FHUDStyle& SetVictoryImage(const FSlateBrush& InVictoryImage) { VictoryImage = InVictoryImage; return *this; }

	/**
	 * The brush used to draw the defeat image
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush DefeatImage;
	FHUDStyle& SetDefeatImage(const FSlateBrush& InDefeatImage) { DefeatImage = InDefeatImage; return *this; }

	/**
	 * The color used to draw the victory text
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateColor VictoryTextColor;
	FHUDStyle& SetVictoryTextColor(const FSlateColor& InVictoryTextColor) { VictoryTextColor = InVictoryTextColor; return *this; }

	/**
	 * The color used to draw the defeat text
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateColor DefeatTextColor;
	FHUDStyle& SetDefeatTextColor(const FSlateColor& InDefeatTextColor) { DefeatTextColor = InDefeatTextColor; return *this; }
};


/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UHUDWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the HUD appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FHUDStyle HUDStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &HUDStyle );
	}
};
