#include "Hydr.h"
#include "HUDSoundsWidgetStyle.h"

FHUDSoundsStyle::FHUDSoundsStyle()
{
}

FHUDSoundsStyle::~FHUDSoundsStyle()
{
}

const FName FHUDSoundsStyle::TypeName(TEXT("FHUDSoundsStyle"));

const FHUDSoundsStyle& FHUDSoundsStyle::GetDefault()
{
	static FHUDSoundsStyle Default;
	return Default;
}

void FHUDSoundsStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
}


UHUDSoundsWidgetStyle::UHUDSoundsWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
