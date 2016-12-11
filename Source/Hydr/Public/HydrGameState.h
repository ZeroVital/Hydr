#pragma once

#include "Types.h"
#include "GameFramework/GameState.h"
#include "HydrGameState.generated.h"

UCLASS(config = Game)
class AHydrGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

public:

	/** Game state. */
	EGameplayState::Type GameplayState;

	/** World bounds for mini map & camera movement. */
	FBox WorldBounds;

	/** Warm up time before game starts */
	UPROPERTY(config)
		int32 WarmupTime;

	/** Current difficulty level of the game. */
	EGameDifficulty::Type GameDifficulty;

	/*
	* Return number of living pawns from a team.
	*
	* @param InTeam 	The team to get living pawn count for.
	* @returns 		The living pawn count for the given team.
	*/
	//UFUNCTION(BlueprintCallable, Category = Game)
	//	int32 GetNumberOfLivePawns(TEnumAsByte<ETeam::Type> InTeam) const;

	/**
	* Set the pause state of the game.
	*
	* @param	bIsPaused	The required pause state
	*/
	UFUNCTION(BlueprintCallable, Category = Game)
		void SetGamePaused(bool bIsPaused);

	/**
	* Notification that a character has died.
	*
	* @param	InChar	The character that has died.
	*/
	//void OnCharDied(AChar* InChar);

	/**
	* Notification that a character has spawned.
	*
	* @param	InChar	The character that has died.
	*/
	//void OnCharSpawned(AChar* InChar);

	/**
	* Notification that an actor was damaged.
	*
	* @param	InChar	The character that has died.
	* @param	Damage	The amount of damage inflicted.
	* @param	InChar	The controller that inflicted the damage.
	*/
	//void OnActorDamaged(AActor* InActor, float Damage, AController* EventInstigator);

	/**
	* Get a team's data.
	*
	* @param	TeamNum	The team to get the data for
	* @returns FPlayerData pointer to the data for requested team.
	*/
	FPlayerData* GetPlayerData(uint8 TeamNum) const;

	/**
	* Initialize the game-play state machine.
	*/
	void StartGameplayStateMachine();

	/**
	* Change game state and notify observers.
	* @param	NewState	The required game state.
	*/
	void SetGameplayState(EGameplayState::Type NewState);

	/** Is game currently in state: playing ? */
	bool IsGameActive() const;

	/** Notification that game has started. */
	void OnGameStart();

	/**
	* Finish the game.
	* @param	InWinningTeam	The team that won.
	*/
	void FinishGame(ETeam::Type InWinningTeam);

	/** Get time until game starts, in seconds. */
	float GetRemainingWaitTime() const;

	/** Get winning team */
	ETeam::Type GetWinningTeam() const;

	/** Get time when game finished */
	float GetGameFinishedTime() const;

	/**
	* Set current difficulty level of the game.
	*
	* @param	NewDifficulty	The required game difficulty.
	*/
	void SetGameDifficulty(EGameDifficulty::Type NewDifficulty);

protected:

	// @todo, get rid of mutable?
	/** Gameplay information about each player. */
	mutable TArray<FPlayerData> PlayersData;

	/** Count of live pawns for each team */
	//uint32 LivePawnCounter[ETeam::MAX];

	/** Team that won.  Set at end of game. */
	ETeam::Type WinningTeam;

	/** Time in seconds when the game finished. Set at the end of game. */
	float GameFinishedTime;

	/** Handle for efficient management of UpdateHealth timer */
	FTimerHandle TimerHandle_OnGameStart;

	/**
	* Register new char to get information from it.
	*
	* @param	InChar		The character to register.
	*/
	//void AddChar(AChar* InChar);

	/**
	* Unregister char after death.
	*
	* @param	NewState	The character to remove/unregister.
	*/
	//void RemoveChar(AChar* InChar);

	/**
	* Pauses/Unpauses current game timer.
	*
	* @param	bIsPaused The required pause state.
	*/
	void SetTimersPause(bool bIsPaused);
};


