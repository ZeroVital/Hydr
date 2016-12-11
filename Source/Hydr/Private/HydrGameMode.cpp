#include "Hydr.h"
//#include "Hexagone.h"
//#include "PlayerController1.h"
#include "HydrSpectatorPawn.h"
//#include <vector>
using namespace std;


AHydrGameMode::AHydrGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = APlayerController1::StaticClass();
	SpectatorClass = AHydrSpectatorPawn::StaticClass();
	DefaultPawnClass = AHydrSpectatorPawn::StaticClass();
	GameStateClass = AHydrGameState::StaticClass();
	HUDClass = AHydrHUD::StaticClass();

	//static ConstructorHelpers::FClassFinder<ABuilding> EmptyWallSlotHelper(TEXT("/Game/Buildings/Wall/Wall_EmptySlot"));
	//EmptyWallSlotClass = EmptyWallSlotHelper.Class;
	if ((GEngine != nullptr) && (GEngine->GameViewport != nullptr))
	{
		GEngine->GameViewport->SetSuppressTransitionMessage(true);
	}
}

// internal
EGameplayState::Type AHydrGameMode::GetGameplayState() const
{
	return GetGameState<AHydrGameState>()->GameplayState;
}

const FString AHydrGameMode::DifficultyOptionName(TEXT("Difficulty"));


void AHydrGameMode::InitGameState()
{
	Super::InitGameState();

	AHydrGameState* const GameState = GetGameState<AHydrGameState>();
	if (GameState)
	{
		EGameDifficulty::Type const NewDifficulty = (EGameDifficulty::Type) UGameplayStatics::GetIntOption(OptionsString, DifficultyOptionName, 0);
		GameState->SetGameDifficulty(NewDifficulty);

		// start the game!
		GameState->StartGameplayStateMachine();
	}
}

void AHydrGameMode::RestartPlayer(AController* NewPlayer)
{
	AActor* const StartSpot = FindPlayerStart(NewPlayer);
	if (StartSpot != nullptr)
	{
		// initialize and start it up
		InitStartSpot(StartSpot, NewPlayer);

		APlayerController1* const NewPC = Cast<APlayerController1>(NewPlayer);
		if (NewPC != nullptr)
		{
			NewPC->SetInitialLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
		}
	}
	else
	{
		//UE_LOG(LogGame, Warning, TEXT("Player start not found, failed to restart player"));
	}
}

float AHydrGameMode::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	// no health changes after game is finished
	if (GetGameplayState() == EGameplayState::Finished)
	{
		return 0.0f;
	}

	if (Damage > 0.f)
	{
		/*const ITeamInterface* VictimTeam = Cast<ITeamInterface>(DamagedActor);
		ITeamInterface* InstigatorTeam = Cast<ITeamInterface>(EventInstigator);
		if (InstigatorTeam == nullptr)
		{
			InstigatorTeam = Cast<ITeamInterface>(DamageCauser);
		}

		// skip friendly fire
		if (InstigatorTeam && VictimTeam && InstigatorTeam->GetTeamNum() == VictimTeam->GetTeamNum())
		{
			return 0.0f;
		}

		// pawn's damage reduction
		const AChar* DamagedChar = Cast<AChar>(DamagedActor);
		if (DamagedChar)
		{
			Damage -= DamagedChar->GetPawnData()->DamageReduction;
		}*/
	}

	return Damage;
}

void AHydrGameMode::FinishGame(ETeam::Type InWinningTeam)
{
	AHydrGameState* CurrentGameState = GetGameState<AHydrGameState>();
	// If the game state is valid and we are still playing
	if ((CurrentGameState != nullptr) && (CurrentGameState->GameplayState != EGameplayState::Finished))
	{
		// tell the gamestate to wrap it up
		CurrentGameState->FinishGame(InWinningTeam);

	}
	// Add a timer to return to main if one does not already exist.
	if (GetWorldTimerManager().GetTimerRate(TimerHandle_ReturnToMenu) == -1.0f)
	{
		// set timer to return to the main menu
		GetWorldTimerManager().SetTimer(TimerHandle_ReturnToMenu, this, &AHydrGameMode::ReturnToMenu, TimeBeforeReturnToMenu, false);
	}
}

void AHydrGameMode::ReturnToMenu()
{
	GetWorld()->ServerTravel(FString("/Game/Maps/MenuMap"));
}


void AHydrGameMode::ExitGame()
{
	APlayerController1* PlayerController1 = nullptr;
	if (GEngine)
	{
		PlayerController1 = Cast<APlayerController1>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController1->ConsoleCommand(TEXT("quit"));
	}
}

/*void AHydrGameMode::BuildField()
{
	float apotheme = 0.866;
	int i, j = 0;
	float x, y;
	UWorld* World = GetWorld();
	vector< vector<AHexagone*> > vHexagoneField(9, vector<AHexagone*>(10));
	for (auto row : vHexagoneField)
	{
		i = 0;
		y = (float)(j) * 0.866 * 210;
		for (auto &element : row)
		{
			x = (float)(i) * 3 / 4 * 210;
			if ((i % 2) == 0)
			{
				element = World->SpawnActor<AHexagone>(FVector(x, y, 0.0f), FRotator(0.0f, 120.0f, 0.0f));
			}
			else
			{
				element = World->SpawnActor<AHexagone>(FVector(x, y + 86.6f, 0.0f), FRotator(0.0f, 120.0f, 0.0f));
			}
			i++;
		}
		j++;
	}
}*/




