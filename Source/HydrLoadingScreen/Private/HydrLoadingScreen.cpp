


#include "HydrLoadingScreen.h"

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SThrobber.h"
#include "RunTime/MoviePlayer/Public/MoviePlayer.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

struct FHydrLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FHydrLoadingScreenBrush( const FName InTextureName, const FVector2D& InImageSize )
		: FSlateDynamicImageBrush( InTextureName, InImageSize )
	{
		ResourceObject = LoadObject<UObject>( NULL, *InTextureName.ToString() );
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if( ResourceObject )
		{
			Collector.AddReferencedObject(ResourceObject);
		}
	}
};

class SHydrLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHydrLoadingScreen) {}
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/UI/MainMenu/Hydr_LoadingScreen.Hydr_LoadingScreen"));

		LoadingScreenBrush = MakeShareable( new FHydrLoadingScreenBrush( LoadingScreenName, FVector2D(1920,1080) ) );

		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SHydrLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis =  GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	
	/** loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FHydrLoadingScreenModule : public IHydrLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		//force load for cooker reference
		LoadObject<UObject>(NULL, TEXT("/Game/UI/MainMenu/Hydr_LoadingScreen.Hydr_LoadingScreen") );

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
		CreateScreen();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SHydrLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

};

IMPLEMENT_GAME_MODULE(FHydrLoadingScreenModule, HydrLoadingScreen);
