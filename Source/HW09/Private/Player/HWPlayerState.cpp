// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HWPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HWPlayerController.h"
#include "Net/UnrealNetwork.h"

AHWPlayerState::AHWPlayerState() :
	PlayerNameString(TEXT("None")),
	PlayerIndex(0),
	CurrentGuessCount(0),
	MaxGuessCount(3)
{
	bReplicates = true;
}

void AHWPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, PlayerIndex);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

void AHWPlayerState::OnRep_PlayerIndex()
{
	OnPlayerIndexChanged.Broadcast(PlayerIndex);
}




