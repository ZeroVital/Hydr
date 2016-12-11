#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "PortraitWidgetStyle.generated.h"

/**
* Represents the the appearance of an SPortraitWidget
*/
USTRUCT()
struct FPortraitStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPortraitStyle();
	virtual ~FPortraitStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPortraitStyle& GetDefault();

	/**
	* The brush used to draw the Portrait background
	*/
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush Portrait1;
	FPortraitStyle& SetPortrait1(const FSlateBrush& InPortrait) { Portrait1 = InPortrait; return *this; }

	UPROPERTY(EditAnywhere, Category = Appearance)
		FSlateBrush Portrait2;
	FPortraitStyle& SetPortrait2(const FSlateBrush& InPortrait) { Portrait2 = InPortrait; return *this; }

	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush Portrait3;
	FPortraitStyle& SetPortrait3(const FSlateBrush& InPortrait) { Portrait3 = InPortrait; return *this; }

	UPROPERTY(EditAnywhere, Category = Appearance)
		FSlateBrush Portrait4;
	FPortraitStyle& SetPortrait4(const FSlateBrush& InPortrait) { Portrait4 = InPortrait; return *this; }

	UPROPERTY(EditAnywhere, Category = Appearance)
		FSlateBrush Portrait5;
	FPortraitStyle& SetPortrait5(const FSlateBrush& InPortrait) { Portrait5 = InPortrait; return *this; }

};


/**
*/
UCLASS(hidecategories = Object, MinimalAPI)
class UPortraitWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the Portrait's appearance. */
	UPROPERTY(Category = Appearance, EditAnywhere, meta = (ShowOnlyInnerProperties))
		FPortraitStyle PortraitStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >(&PortraitStyle);
	}
};
