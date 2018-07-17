// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnBasedShooterGameInstance.h"


void UTurnBasedShooterGameInstance::PassOnPlayerData
(
	int ArrayReference,
	APlayerController* PlayerControllerIDReference,
	int PlayerIDReference,
	int TeamDelta,
	int GadgetDelta,
	int CharacterDelta,
	bool bIsActiveDelta
)
{
	if ( ! PlayerDataArray.IsValidIndex(ArrayReference))
	{
		UE_LOG(LogTemp, Warning, TEXT("PassOnPlayerData() does not have a valid index reference"));
	}
	else
	{
		PlayerDataArray[ArrayReference].PlayerControllerID = PlayerControllerIDReference;
		PlayerDataArray[ArrayReference].PlayerID = PlayerIDReference;
		PlayerDataArray[ArrayReference].Team = TeamDelta;
		PlayerDataArray[ArrayReference].Gadget = GadgetDelta;
		PlayerDataArray[ArrayReference].Character = CharacterDelta;
		PlayerDataArray[ArrayReference].bIsActive = bIsActiveDelta;
	}
}


void UTurnBasedShooterGameInstance::PassOnRoundData(int ArrayReference, int TeamToChange, int ScoreDelta)
{
	RoundDataArray[ArrayReference].Team = TeamToChange;
	RoundDataArray[ArrayReference].Score = ScoreDelta;
}


void UTurnBasedShooterGameInstance::ClearRoundDataArray()
{
	RoundDataArray.Empty();
}


void UTurnBasedShooterGameInstance::ClearPlayerDataArray()
{
	PlayerDataArray.Empty();
}


//EXTRA
//The following are commands set for the Exhibition only

void UTurnBasedShooterGameInstance::vc(float Delta)
{
	if (Delta != NULL)
	{
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("BPcv Delta"), ar, NULL, true);
	}
}

void UTurnBasedShooterGameInstance::vcm(float Delta)
{
	if (Delta != NULL)
	{
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("BPvcm %Delta"), ar, NULL, true);
	}
}

void UTurnBasedShooterGameInstance::vm(float Delta)
{
	if (Delta != NULL)
	{
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("BPvm"), ar, NULL, true);
	}
}

void UTurnBasedShooterGameInstance::vmm(float Delta)
{
	if (Delta != NULL)
	{
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("BPvmm"), ar, NULL, true);
	}
}

void UTurnBasedShooterGameInstance::vsfx(float Delta)
{
	if (Delta != NULL)
	{
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("BPvsfx"), ar, NULL, true);
	}
}
