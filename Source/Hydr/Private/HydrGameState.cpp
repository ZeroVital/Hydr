#include "Hydr.h"
//#include "Building_Brewery.h"
#include "Types.h"

AHydrGameState::AHydrGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// team data for: unknown, player, enemy
	PlayersData.AddZeroed(ETeam::MAX);
	//MiniMapCamera = nullptr;
	WinningTeam = ETeam::Unknown;
	GameFinishedTime = 0;
}
/*
int32 AHydrGameState::GetNumberOfLivePawns(TEnumAsByte<ETeam::Type> InTeam) const
{
	return LivePawnCounter[InTeam];
}

void AHydrGameState::AddChar(AChar* InChar)
{
	if (InChar != nullptr)
	{
		LivePawnCounter[InChar->GetTeamNum()]++;
	}
}

void AHydrGameState::RemoveChar(AChar* InChar)
{
	if (InChar != nullptr)
	{
		LivePawnCounter[InChar->GetTeamNum()]--;
	}
}

void AHydrGameState::OnCharDied(AChar* InChar)
{
	if (InChar && (InChar->GetTeamNum() == ETeam::Enemy))
	{
		PlayersData[ETeam::Player].ResourcesAvailable += InChar->ResourcesToGather;
		RemoveChar(InChar);
	}
}

void AHydrGameState::OnActorDamaged(AActor* InActor, float Damage, AController* EventInstigator)
{
	ITeamInterface* const InstigatorTeam = Cast<ITeamInterface>(EventInstigator);

	// track damage done
	// @todo, this is def not the place for this
	if (InstigatorTeam)
	{
		FPlayerData& TeamData = PlayersData[InstigatorTeam->GetTeamNum()];
		TeamData.DamageDone += FMath::TruncToInt(Damage);
	}
}

void AHydrGameState::OnCharSpawned(AChar* InChar)
{
	if (InChar && !InChar->IsPendingKill())
	{
		AddChar(InChar);
	}
}
*/
FPlayerData* AHydrGameState::GetPlayerData(uint8 TeamNum) const
{
	if (TeamNum != ETeam::Unknown)
	{
		return &PlayersData[TeamNum];
	}

	return nullptr;
}

void AHydrGameState::SetGameplayState(EGameplayState::Type NewState)
{
	GameplayState = NewState;

	// notify the breweries of the state change
	/*for (int32 i = 0; i < PlayersData.Num(); i++)
	{
		if (PlayersData[i].Brewery.IsValid())
		{
			PlayersData[i].Brewery->OnGameplayStateChange(NewState);
		}
	}*/
}

bool AHydrGameState::IsGameActive() const
{
	return GameplayState == EGameplayState::Playing;
}

float AHydrGameState::GetRemainingWaitTime() const
{
	if (GameplayState == EGameplayState::Waiting)
	{
		return GetWorldTimerManager().GetTimerRemaining(TimerHandle_OnGameStart);
	}

	return 0.f;
}


ETeam::Type AHydrGameState::GetWinningTeam() const
{
	return WinningTeam;
}

void AHydrGameState::OnGameStart()
{
	SetGameplayState(EGameplayState::Playing);

	WinningTeam = ETeam::Unknown;
	GameFinishedTime = 0.0f;
}


void AHydrGameState::FinishGame(ETeam::Type InWinningTeam)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);

	SetGameplayState(EGameplayState::Finished);
	WinningTeam = InWinningTeam;
	GameFinishedTime = GetWorld()->GetRealTimeSeconds();
}



void AHydrGameState::StartGameplayStateMachine()
{
	if (WarmupTime > 0.f)
	{
		SetGameplayState(EGameplayState::Waiting);
		GetWorldTimerManager().SetTimer(TimerHandle_OnGameStart, this, &AHydrGameState::OnGameStart, WarmupTime, false);
	}
	else
	{
		OnGameStart();
	}
}

void AHydrGameState::SetTimersPause(bool bIsPaused)
{
	if (GameplayState == EGameplayState::Waiting)
	{
		bIsPaused ? GetWorldTimerManager().PauseTimer(TimerHandle_OnGameStart) : GetWorldTimerManager().UnPauseTimer(TimerHandle_OnGameStart);
	}
}
void AHydrGameState::SetGamePaused(bool bIsPaused)
{
	APlayerController1* const Player = Cast<APlayerController1>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (Player != nullptr)
	{
		Player->SetPause(bIsPaused);
		SetTimersPause(bIsPaused);
	}
}

void AHydrGameState::SetGameDifficulty(EGameDifficulty::Type NewDifficulty)
{
	GameDifficulty = NewDifficulty;
}

float AHydrGameState::GetGameFinishedTime() const
{
	return GameFinishedTime;
}
