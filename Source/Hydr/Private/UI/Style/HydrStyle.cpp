#include "Hydr.h"
#include "SlateGameResources.h"

TSharedPtr< FSlateStyleSet > FHydrStyle::StyleInstance = NULL;

void FHydrStyle::Initialize()
{
	if ( !StyleInstance.IsValid() )
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle( *StyleInstance );
	}
}

void FHydrStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle( *StyleInstance );
	ensure( StyleInstance.IsUnique() );
	StyleInstance.Reset();
}

FName FHydrStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("Style"));
	return StyleSetName;
}


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FPaths::GameContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( FPaths::GameContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( FPaths::GameContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::GameContentDir() / "Slate"/ RelativePath + TEXT(".ttf"), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::GameContentDir() / "Slate"/ RelativePath + TEXT(".otf"), __VA_ARGS__ )

TSharedRef< FSlateStyleSet > FHydrStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FHydrStyle::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	FSlateStyleSet& Style = StyleRef.Get();

	const FLinearColor GoldColor(FColor(255,213,160));

	// Fonts still need to be specified in code for now
	Style.Set(".MenuFont", TTF_FONT("Fonts/Roboto-Black", 32));

	Style.Set(".ResourcesTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 40))
		.SetColorAndOpacity(GoldColor)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set(".ButtonFont", TTF_FONT("Fonts/Roboto-Black", 18));

	return StyleRef;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FHydrStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FHydrStyle::Get()
{
	return *StyleInstance;
}
