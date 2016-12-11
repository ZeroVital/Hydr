#include "Hydr.h"
#include "MenuWidgetStyle.h"

FMenuStyle::FMenuStyle()
{
}

FMenuStyle::~FMenuStyle()
{
}

const FName FMenuStyle::TypeName(TEXT("FMenuStyle"));

const FMenuStyle& FMenuStyle::GetDefault()
{
	static FMenuStyle Default;
	return Default;
}

void FMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&BackgroundBrush);
	OutBrushes.Add(&TileBackgroundBrush);
	OutBrushes.Add(&PopupBackgroundBrush);
	OutBrushes.Add(&TopDecoration);
	OutBrushes.Add(&LeftDecoration);
	OutBrushes.Add(&RightDecoration);
}


UMenuWidgetStyle::UMenuWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
