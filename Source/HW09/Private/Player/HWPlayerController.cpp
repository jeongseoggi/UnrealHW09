// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HWPlayerController.h"
#include "UI/HWChatInput.h"
#include "UI/HWHUD.h"
#include "Components/ProgressBar.h"
#include "HW09/HW09.h"
#include "Net/UnrealNetwork.h"
#include "Game/HWGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HWPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

AHWPlayerController::AHWPlayerController()
{
	bReplicates = true;
}

void AHWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalPlayerController())
	{
		return;
	}

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	if (ChatInputWidgetClass)
	{
		ChatInputWidgetInstance = CreateWidget<UHWChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (NotificationWidgetClass)
	{
		NotificationWidgetInstance = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
		if (IsValid(NotificationWidgetInstance))
		{
			NotificationWidgetInstance->AddToViewport();
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UHWHUD>(this, HUDWidgetClass);
		if (IsValid(NotificationWidgetInstance))
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

void AHWPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController())
	{
		AHWPlayerState* HWPS = GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS))
		{
			FString CombinedMessageString = TEXT(": ") + InChatMessageString;
			ServerRPCPrintChatMessage(CombinedMessageString);
		}
	}
}

void AHWPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	NumberBaseBallFuctionLibrary::PrintString(this, InChatMessageString, 10.f);
}

void AHWPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, TurnTime);
}

void AHWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AHWPlayerState* HWPS = GetPlayerState<AHWPlayerState>())
	{
		HWPS->OnPlayerIndexChanged.AddDynamic(this, &ThisClass::HUDButtonHandle);

		if (HWPS->PlayerIndex != -1)
		{
			HUDButtonHandle(HWPS->PlayerIndex);
		}
	}

}

void AHWPlayerController::SetProgressBar(const float& RemainTime)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->GetTimerProgressBar()->SetPercent(RemainTime * 0.05f);
	}
}

void AHWPlayerController::SetMessageBox(const FString& Message, FLinearColor TextColor)
{
	if (ChatInputWidgetInstance)
	{
		ChatInputWidgetInstance->AddToChatBox(Message, TextColor);
	}
}

void AHWPlayerController::SetNotificationMsg(const FString& NotiMsg)
{
	if (IsValid(NotificationWidgetInstance))
	{
		UTextBlock* TextBlock = Cast<UTextBlock>(NotificationWidgetInstance->GetWidgetFromName(TEXT("NotificationText")));
		if (IsValid(TextBlock))
		{
			TextBlock->SetText(FText::FromString(NotiMsg));
		}
	}
}

void AHWPlayerController::HUDButtonHandle(int32 NewIndex)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetShowButton(NewIndex == 1);
	}
}

void AHWPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& Message, FLinearColor TextColor)
{
	SetMessageBox(Message, TextColor);
}

void AHWPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& Message)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);

	if (IsValid(GM))
	{
		if (AHWGameModeBase* HWGM = Cast<AHWGameModeBase>(GM))
		{
			HWGM->AddMsgBox(this, Message);
		}
	}
}

void AHWPlayerController::ServerRPCGameStart_Implementation()
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);

	if (IsValid(GM))
	{
		if (AHWGameModeBase* HWGM = Cast<AHWGameModeBase>(GM))
		{
			HWGM->StartGame();
		}
	}

	ClientRPCSetStartButton(false);
}

void AHWPlayerController::ClientRPCSetStartButton_Implementation(bool bIsShow)
{
	HUDWidgetInstance->SetShowButton(bIsShow);
}

void AHWPlayerController::SetTurnText(int32 NewGuessCount, int32 MaxGuessCount)
{
	if (IsValid(HUDWidgetInstance))
	{
		HUDWidgetInstance->SetTurnText(NewGuessCount, MaxGuessCount);
	}
}