#include "Hydr.h"
#include "SButtonWidget.h"
#include "SHUDMenuWidget.h"
#include "HydrGameHelpers.h"
//#include "AIController.h"
//#include "Building.h"
//#include "Building_Brewery.h"

AHydrHUD::AHydrHUD(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> BarFillObj(TEXT("/Game/UI/HUD/BarFill"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> PlayerTeamHPObj(TEXT("/Game/UI/HUD/PlayerTeamHealthBar"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> EnemyTeamHPObj(TEXT("/Game/UI/HUD/EnemyTeamHealthBar"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultActionObj(TEXT("/Game/UI/HUD/Actions/DefaultAction"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultCenterActionObj(TEXT("/Game/UI/HUD/Actions/DefaultActionBig"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ActionPauseObj(TEXT("/Game/UI/HUD/Actions/ActionPause"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> MenuButtonObj(TEXT("/Game/UI/MainMenu/MenuButton"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ResourceObj(TEXT("/Game/UI/HUD/Coin"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> LivesObj(TEXT("/Game/UI/HUD/Actions/Barrel"));


	static ConstructorHelpers::FObjectFinder<UMaterial> HUDMousePointerNeutralObj(TEXT("/Game/UI/Pointers/Neutral"));
	static ConstructorHelpers::FObjectFinder<UMaterial> HUDMousePointerAttackObj(TEXT("/Game/UI/Pointers/Enemy"));

	BarFillTexture = BarFillObj.Object;
	PlayerTeamHPTexture = PlayerTeamHPObj.Object;
	EnemyTeamHPTexture = EnemyTeamHPObj.Object;
	LivesTexture = LivesObj.Object;
	
	DefaultActionTexture = DefaultActionObj.Object;
	DefaultCenterActionTexture = DefaultCenterActionObj.Object;
	ActionPauseTexture = ActionPauseObj.Object;
	MenuButtonTexture = MenuButtonObj.Object;
	ResourceTexture = ResourceObj.Object;
	
	MousePointerNeutral = HUDMousePointerNeutralObj.Object;
	MousePointerAttack = HUDMousePointerAttackObj.Object;
	
	MiniMapMargin = 40;
	bBlackScreenActive = false;
}


/**
* This is the main drawing pump.  It will determine which hud we need to draw (Game or PostGame).  Any drawing that should occur
* regardless of the game state should go here.
*/
void AHydrHUD::DrawHUD()
{
	if (bBlackScreenActive)
	{
		FCanvasTileItem TileItem(FVector2D(0.0f, 0.0f), FVector2D(Canvas->ClipX, Canvas->ClipY), FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
		return;
	}

	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		UIScale = ViewportSize.X / 2048.0f;
	}

	Super::DrawHUD();
	bool bConsoleOpen = false;

	AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
	if (State)
	{
		//Builds the widgets if they are not yet built
		BuildMenuWidgets();


		if (IsPauseMenuUp())
		{
			/*FCanvasTileItem TileItem( FVector2D( 0.0f, 0.0f ), FVector2D( Canvas->ClipX,Canvas->ClipY ), FLinearColor( 0.0f, 0.0f, 0.0f, 0.25f ) );
			TileItem.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem( TileItem );*/
		}

		if (SelectedActor.IsValid())
		{
			ActionGridPos = FVector2D(Canvas->Project(SelectedActor->GetActorLocation())) / UIScale - (HUDMenuWidget->ActionButtonsWidget->GetDesiredSize()) / 2;
		}
	}
}

void AHydrHUD::ShowBlackScreen()
{
	HideAllActionButtons(true);
	bBlackScreenActive = true;
	GEngine->GameViewport->RemoveAllViewportWidgets();
}


void AHydrHUD::BuildMenuWidgets()
{
	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	if (!HUDMenuWidget.IsValid())
	{
		const APlayerController1* PCOwner = Cast<APlayerController1>(PlayerOwner);

		if (PCOwner)
		{
			SAssignNew(HUDMenuWidget, SHUDMenuWidget)
				.OwnerHUD(this);

			if (HUDMenuWidget.IsValid())
			{

				GEngine->GameViewport->AddViewportWidgetContent(
					SNew(SWeakWidget)
					.PossiblyNullContent(HUDMenuWidget.ToSharedRef())
				);

				HUDMenuWidget->ActionButtonsWidget->SetVisibility(EVisibility::Visible);
				HUDMenuWidget->ActionWidgetPosition.BindUObject(this, &AHydrHUD::GetActionsWidgetPos);

				if (ActionPauseTexture != NULL)
				{
					HUDMenuWidget->PauseButton->SetImage(ActionPauseTexture);
					HUDMenuWidget->PauseButton->DeferredShow();
				}
				if (MenuButtonTexture != NULL)
				{
					for (uint8 i = 0; i < HUDMenuWidget->PauseMenuButtons.Num(); i++)
					{
						HUDMenuWidget->PauseMenuButtons[i]->SetImage(MenuButtonTexture);
					}
				}
			}
		}
	}
}


FVector2D AHydrHUD::GetActionsWidgetPos() const
{
	return ActionGridPos;
}

TSharedPtr<FActionButtonInfo> AHydrHUD::GetActionButton(int32 Index) const
{
	if (HUDMenuWidget.IsValid() &&
		HUDMenuWidget->ActionButtonsWidget.IsValid() &&
		HUDMenuWidget->ActionButtonsWidget->ActionButtons.IsValidIndex(Index))
	{
		return HUDMenuWidget->ActionButtonsWidget->ActionButtons[Index];
	}

	return TSharedPtr<FActionButtonInfo>();
}

TSharedPtr<SHUDMenuWidget> AHydrHUD::GetHUDWidget() const
{
	return HUDMenuWidget;
}


void AHydrHUD::ClearActionRequiredStates()
{
	if (HUDMenuWidget.IsValid() &&
		HUDMenuWidget->ActionButtonsWidget.IsValid())
	{
		for (int32 i = HUDMenuWidget->ActionButtonsWidget->ActionButtons.Num() - 1; i >= 0; i--)
		{
			HUDMenuWidget->ActionButtonsWidget->ActionButtons[i]->Widget->SetUserActionRequired(false);
		}
	}
}

void AHydrHUD::HideAllActionButtons(bool bInstantHide)
{
	if (HUDMenuWidget.IsValid() &&
		HUDMenuWidget->ActionButtonsWidget.IsValid())
	{
		for (int32 i = HUDMenuWidget->ActionButtonsWidget->ActionButtons.Num() - 1; i >= 0; i--)
		{
			HUDMenuWidget->ActionButtonsWidget->ActionButtons[i]->Widget->DeferredHide(bInstantHide);
			HUDMenuWidget->ActionButtonsWidget->ActionButtons[i]->Data.Visibility = EVisibility::Hidden;
		}
	}
}

/*void AHydrHUD::DrawLives() const
{
	AHydrGameState const* const State = GetWorld()->GetGameState<AHydrGameState>();
	ABuilding_Brewery const* const Brewery = State ? State->GetPlayerData(ETeam::Player)->Brewery.Get() : NULL;

	uint8 const Lives = Brewery ? Brewery->GetNumberOfLives() : 0;

	float const TextureDrawWidth = LivesTexture->GetSurfaceWidth() * UIScale;
	float const TextureDrawHeight = LivesTexture->GetSurfaceHeight() * UIScale;

	FCanvasTileItem TileItem(FVector2D(0, 0), LivesTexture->Resource,
		FVector2D(TextureDrawWidth, TextureDrawHeight), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	for (int32 i = 0; i < Lives; i++)
	{
		TileItem.Position = FVector2D(Canvas->ClipX - Lives*TextureDrawWidth + i*TextureDrawWidth, 0);
		Canvas->DrawItem(TileItem);
	}
}

void AHydrHUD::DrawHealthBar(AActor* ForActor, float HealthPercentage, int32 BarHeight, int32 OffsetY) const
{
	FBox BB = ForActor->GetComponentsBoundingBox();
	FVector Center = BB.GetCenter();
	FVector Extent = BB.GetExtent();
	FVector2D Center2D = FVector2D(Canvas->Project(FVector(Center.X, Center.Y, Center.Z + Extent.Z)));
	float ActorExtent = 40;
	if (Cast<APawn>(ForActor) != NULL)
	{
		AChar* Char = Cast<AChar>(ForActor);
		if ((Char != NULL) && (Char->GetCapsuleComponent() != NULL))
		{
			ActorExtent = Char->GetCapsuleComponent()->GetScaledCapsuleRadius();
		}
	}
	else if (Cast<ABuilding>(ForActor) != NULL)
	{
		Center2D = FVector2D(Canvas->Project(ForActor->GetActorLocation()));
		ActorExtent = 60;
	}

	FVector Pos1 = Canvas->Project(FVector(Center.X, Center.Y - ActorExtent * 2, Center.Z + Extent.Z));
	FVector Pos2 = Canvas->Project(FVector(Center.X, Center.Y + ActorExtent * 2, Center.Z + Extent.Z));
	float HealthBarLength = (Pos2 - Pos1).Size2D();

	APlayerController1* PC = GetPlayerController();
	ITeamInterface* ActorTeam = Cast<ITeamInterface>(ForActor);
	UTexture2D* HealthBarTexture = EnemyTeamHPTexture;

	if (ActorTeam != NULL && PC != NULL && ActorTeam->GetTeamNum() == PC->GetTeamNum())
	{
		HealthBarTexture = PlayerTeamHPTexture;
	}
	float X = Center2D.X - HealthBarLength / 2;
	float Y = Center2D.Y + OffsetY;
	FCanvasTileItem TileItem(FVector2D(X, Y), HealthBarTexture->Resource, FVector2D(HealthBarLength * HealthPercentage, BarHeight), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	TileItem.UV1 = FVector2D(HealthPercentage, 1.0f);

	Canvas->DrawItem(TileItem);
	//Fill the rest of health with gray gradient texture
	X = Center2D.X - HealthBarLength / 2 + HealthBarLength * HealthPercentage;
	Y = Center2D.Y + OffsetY;
	TileItem.Position = FVector2D(X, Y);
	TileItem.Texture = BarFillTexture->Resource;
	TileItem.UV1 = FVector2D(1.0f, 1.0f);
	TileItem.Size = FVector2D(HealthBarLength * (1.0f - HealthPercentage), BarHeight);
	TileItem.SetColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	Canvas->DrawItem(TileItem);
}

*/

void AHydrHUD::DrawMousePointer()
{
#if PLATFORM_DESKTOP
	if (PlayerOwner)
	{
		FVector2D MousePosition;
		PlayerOwner->GetMousePosition(MousePosition.X, MousePosition.Y);

		const FVector2D PointerSize(60.0f, 30.0f);
		const FVector2D DrawPos(MousePosition.X - PointerSize.X * 0.5f, MousePosition.Y - PointerSize.Y * 0.5f);

		DrawMaterialSimple(MousePointerNeutral, DrawPos.X, DrawPos.Y, PointerSize.X, PointerSize.Y);
	}
#endif
}

bool AHydrHUD::IsPauseMenuUp() const
{
	if (HUDMenuWidget.IsValid())
	{
		return HUDMenuWidget->IsPauseMenuUp();
	}
	else
	{
		return false;
	}
}

void AHydrHUD::TogglePauseMenu()
{
	if (HUDMenuWidget.IsValid())
	{
		HUDMenuWidget->TogglePauseMenu();
	}
}

APlayerController1* AHydrHUD::GetPlayerController() const
{
	return Cast<APlayerController1>(PlayerOwner);
}

void AHydrHUD::SetActionGridActor(AActor* InSelectedActor)
{
	SelectedActor.Reset();
	SelectedActor = TWeakObjectPtr<AActor>(InSelectedActor);
}
