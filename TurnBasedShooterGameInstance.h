// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/GameInstance.h"
#include "TurnBasedShooterGameInstance.generated.h"


/**
* All player data. Player information passed on here from menus and used by
* TurnBasedShooterGameModeBase for spawning and setting up player.
*/
USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

public:
	//Player Controller reference
	UPROPERTY(BlueprintReadWrite)
	APlayerController* PlayerControllerID;

	//ID is the player controller number order
	UPROPERTY(BlueprintReadWrite)
	int PlayerID;

	//0 for Corp, 1 for Junk
	UPROPERTY(BlueprintReadWrite)
	int Team;

	//Gadget int, TODO - replace with ENUM later
	UPROPERTY(BlueprintReadWrite)
	int Gadget;

	//Character int, 0 is Monowheel, 1 is Kite
	UPROPERTY(BlueprintReadWrite)
	int Character;

	//To check if player has joined game in player select lobby
	UPROPERTY(BlueprintReadWrite)
	bool bIsActive;
};


/**
* Round data. set as a Struct so an Array can be made for data manipulation.
*/
USTRUCT(BlueprintType)
struct FRoundData
{
	GENERATED_USTRUCT_BODY()

public:
	//0 for Corp, 1 for Junk
	UPROPERTY(BlueprintReadWrite)
	int Team;

	//checked for best of 3 rounds in game mode
	UPROPERTY(BlueprintReadWrite)
	int Score;
};


/**
* The Game Instance is used as a management class whos data is persistent across maps
* This class has very little function outside of data handling
*
* Responsibilities:
* Passing on Struct Data and clearing it
*/
UCLASS()
class TURNBASEDSHOOTER_API UTurnBasedShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/**
	* Called from Game Mode to pass on player data to this call
	* 
	* All parameters references @see TurnBasedShooterGameModeBase.h
	*/
	UFUNCTION(BlueprintCallable)
	void PassOnPlayerData
	(
		int ArrayReference,
		APlayerController* PlayerControllerIDReference,
		int PlayerIDReference,
		int TeamDelta,
		int GadgetDelta,
		int CharacterDelta,
		bool bIsActiveDelta
	);

	//Called from Game Mode to pass on round data to this call
	UFUNCTION(BlueprintCallable)
	void PassOnRoundData(int ArrayReference, int TeamToChange, int ScoreDelta);

	//Clear all RoundDataArray
	UFUNCTION(BlueprintCallable)
	void ClearRoundDataArray();

	//Clear all PlayerDataArray
	UFUNCTION(BlueprintCallable)
	void ClearPlayerDataArray();

	//An array to store data for each player
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerData> PlayerDataArray;
	
	//An array for each team to work out who has one.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FRoundData> RoundDataArray;

	//EXTRA
	//these command callable functions are purely for backup in the exhibition, remove later
	UFUNCTION(exec)
	void vc(float Delta);

	UFUNCTION(exec)
	void vcm(float Delta);

	UFUNCTION(exec)
	void vm(float Delta);

	UFUNCTION(exec)
	void vmm(float Delta);

	UFUNCTION(exec)
	void vsfx(float Delta);
};
