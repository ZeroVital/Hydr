#include "Hydr.h"
#include "PortraitWidgetStyle.h"

FPortraitStyle::FPortraitStyle()
{
}

FPortraitStyle::~FPortraitStyle()
{
}

const FName FPortraitStyle::TypeName(TEXT("FPortraitStyle"));

const FPortraitStyle& FPortraitStyle::GetDefault()
{
	static FPortraitStyle Default;
	return Default;
}

void FPortraitStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&Portrait1);
	OutBrushes.Add(&Portrait2);
	OutBrushes.Add(&Portrait3);
	OutBrushes.Add(&Portrait4);
	OutBrushes.Add(&Portrait5);
}


UPortraitWidgetStyle::UPortraitWidgetStyle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
