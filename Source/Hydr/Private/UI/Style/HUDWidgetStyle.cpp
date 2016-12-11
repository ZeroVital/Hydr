#include "Hydr.h"
#include "HUDWidgetStyle.h"

FHUDStyle::FHUDStyle()
{
}

FHUDStyle::~FHUDStyle()
{
}

const FName FHUDStyle::TypeName(TEXT("FHUDStyle"));

const FHUDStyle& FHUDStyle::GetDefault()
{
	static FHUDStyle Default;
	return Default;
}

void FHUDStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&ResourcesBackgroundBrush);
	OutBrushes.Add(&ResourcesImage);
	OutBrushes.Add(&MinimapFrameBrush);
	OutBrushes.Add(&VictoryImage);
	OutBrushes.Add(&DefeatImage);
}


UHUDWidgetStyle::UHUDWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
