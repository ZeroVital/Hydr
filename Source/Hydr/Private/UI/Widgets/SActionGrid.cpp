#include "Hydr.h"
#include "SActionGrid.h"
#include "SButtonWidget.h"


void SActionGrid::Construct(const FArguments& InArgs)
{
	const int32 PaddingValue = 2;
	const int32 GridRows = 3;
	const int32 GridCols = 3;

	OwnerHUD = InArgs._OwnerHUD;
	ActionButtons.Reset();
	for (uint8 i=0; i < GridRows*GridCols; i++)
	{
		ActionButtons.Add(MakeShareable(new FActionButtonInfo()));
	}
 
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SAssignNew(ActionGrid, SGridPanel)
		]
	];

	for (int32 i = 0; i < GridRows; i++)
	{
		for (int32 j=0; j < GridCols; j++)
		{
			ActionGrid->AddSlot(j,i)
			.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SActionGrid::GetActionPadding, i*GridRows+j)))
			[
				SAssignNew(ActionButtons[i*GridRows+j]->Widget,SButtonWidget)
				.OwnerHUD(OwnerHUD)
				.ToolTip(SNew(SToolTip).Text(this, &SActionGrid::GetTooltip,i*GridRows+j))
				.IsEnabled(this, &SActionGrid::GetEnabled, i*GridRows+j)
				.Visibility(this, &SActionGrid::GetActionVisibility, i*GridRows+j)
				.OnClicked(this, &SActionGrid::TriggerAction, i*GridRows+j)
				.ButtonText(this, &SActionGrid::GetActionCostText, i*GridRows+j)
				.CenterText(this, &SActionGrid::GetActionText, i*GridRows+j)
				.CornerText(this,&SActionGrid::GetActionQueueText, i*GridRows+j)
				.CoinIconVisible(this, &SActionGrid::GetCoinIconVisibility, i*GridRows+j)
			];
		}
	}
}

FMargin SActionGrid::GetActionPadding(int32 idx) const
{
	const float SmallMargin = 6;
	const float LargeMargin = 60;
	//this is difference in size from big action icon(which is used only in the center) to small action icon divided by 2
	//example (86 - 64)/2 = 11
	const float CenterMarginOffset = 0;//11;
	//Margins are set up so buttons form a circle
	const FMargin ActionButtonsMargins[9] = { 
		FMargin(LargeMargin, LargeMargin, SmallMargin, SmallMargin), 
		FMargin(SmallMargin, SmallMargin, SmallMargin, LargeMargin - CenterMarginOffset),
		FMargin(SmallMargin, LargeMargin, LargeMargin, SmallMargin),

		FMargin(SmallMargin, SmallMargin, LargeMargin - CenterMarginOffset, SmallMargin),
		FMargin(SmallMargin),
		FMargin(LargeMargin - CenterMarginOffset, SmallMargin, SmallMargin, SmallMargin),

		FMargin(LargeMargin, SmallMargin, SmallMargin, LargeMargin),
		FMargin(SmallMargin, LargeMargin - CenterMarginOffset, SmallMargin, SmallMargin),
		FMargin(SmallMargin, SmallMargin, LargeMargin, LargeMargin)
	};

	return (idx >= 0 && idx <= 8) ? ActionButtonsMargins[idx] : FMargin(SmallMargin);
}

FReply SActionGrid::TriggerAction(int32 idx) const
{
	if (ActionButtons[idx]->Widget.IsValid() && 
		ActionButtons[idx]->Data.TriggerDelegate.IsBound() && 
		ActionButtons[idx]->Data.TriggerDelegate.Execute())
	{
		ActionButtons[idx]->Data.bIsEnabled = false;
		ActionButtons[idx]->Widget->DeferredHide();
	}
	return FReply::Handled();
}

FText SActionGrid::GetActionCostText(int32 idx) const
{
	return ActionButtons[idx]->Data.ActionCost != 0 ? FText::AsNumber(ActionButtons[idx]->Data.ActionCost) : FText::GetEmpty();
}

FText SActionGrid::GetActionText(int32 idx) const
{
	return ActionButtons[idx]->Data.StrButtonText;
}

FText SActionGrid::GetActionQueueText(int32 idx) const
{
	FText Result = FText::GetEmpty();
	if (ActionButtons[idx]->Data.GetQueueLengthDelegate.IsBound())
	{
		Result = ActionButtons[idx]->Data.GetQueueLengthDelegate.Execute();
	}
	return Result;
}

TOptional<EVisibility> SActionGrid::GetCoinIconVisibility(int32 idx) const
{
	return ActionButtons[idx]->Data.ActionCost == 0 ? EVisibility::Collapsed : EVisibility::Visible;
}

FText SActionGrid::GetTooltip(int32 idx) const
{
	return ActionButtons[idx]->Data.StrTooltip;
}

bool SActionGrid::GetEnabled(int32 idx) const
{
	const APlayerController1* const PC = Cast<APlayerController1>(OwnerHUD->PlayerOwner);
	AHydrGameState* const State = OwnerHUD->GetWorld()->GetGameState<AHydrGameState>();
	if (State && PC)
	{
		/*FPlayerData* const PlayerData = State->GetPlayerData(PC->GetTeamNum());
		if (PlayerData)
		{
			if (PlayerData->ResourcesAvailable >= (uint32)FMath::Max(0, ActionButtons[idx]->Data.ActionCost))
			{
				ActionButtons[idx]->Widget->SetActionAllowed(ActionButtons[idx]->Data.bIsEnabled);
			} 
			else
			{
				ActionButtons[idx]->Widget->SetActionAllowed(false);
			}
		}*/
	}

	return true;
}

EVisibility SActionGrid::GetActionVisibility(int32 idx) const
{
	return ActionButtons[idx]->Widget->IsAnimating() ? EVisibility::Visible : ActionButtons[idx]->Data.Visibility;
}
