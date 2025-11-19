// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HWGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HWPlayerController.h"
#include "Game/HWGameModeBase.h"
#include "Net/UnrealNetwork.h"

AHWGameStateBase::AHWGameStateBase() :
	bIsGameStart(false),
	TurnTime(20.0f)
{
	bReplicates = true;
}

void AHWGameStateBase::MulticastRPCBroadcastMessage_Implementation(const FString& NotiMessage)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			if (AHWPlayerController* HWPC = Cast<AHWPlayerController>(PC))
			{
				HWPC->SetNotificationMsg(NotiMessage);
			}
		}
	}
}



void AHWGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsGameStart);
	DOREPLIFETIME(ThisClass, TurnTime);
}

bool AHWGameStateBase::GetIsGameStart() const
{
	return bIsGameStart;
}

void AHWGameStateBase::SetIsGameStart(bool bIsStart)
{
	bIsGameStart = bIsStart;
}

void AHWGameStateBase::StartTurnTimer()
{
	ResetTimer();

	GetWorld()->GetTimerManager().SetTimer(
		TurnTimerHandle,
		this,
		&AHWGameStateBase::TickTurnTimer,
		0.01f,
		true
	);
}

void AHWGameStateBase::TickTurnTimer()
{
	TurnTime -= 0.01f;

	if (TurnTime <= 0.01f)
	{
		GetWorldTimerManager().ClearTimer(TurnTimerHandle);
		
		if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
		{
			if (AHWGameModeBase* HWGM = Cast<AHWGameModeBase>(GM))
			{
				HWGM->NextTurn(true);
			}
		}
	}
}

void AHWGameStateBase::ResetTimer()
{
	TurnTime = 20.0f;
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
}

void AHWGameStateBase::OnRep_TurnRemainingTime()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		if (AHWPlayerController* HWPC = Cast<AHWPlayerController>(PC))
		{
			HWPC->SetProgressBar(TurnTime);
		}
	}
}
