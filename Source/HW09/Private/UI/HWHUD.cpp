// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HWHUD.h"
#include "Components/ProgressBar.h"
#include "Player/HWPlayerController.h"
#include "Components/Button.h"
#include "UI/HWChatInput.h"
#include "Components/TextBlock.h"

void UHWHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (AHWPlayerController* HWPC = Cast<AHWPlayerController>(GetOwningPlayer()))
	{
		StartButton->OnClicked.AddDynamic(HWPC, &AHWPlayerController::ServerRPCGameStart);
	}
}

UProgressBar* UHWHUD::GetTimerProgressBar() const
{
	if (IsValid(TimerProgressBar))
	{
		return TimerProgressBar;
	}
	return nullptr;
}

void UHWHUD::ChatInputAdd()
{
	AHWPlayerController* HWPC = Cast<AHWPlayerController>(GetWorld()->GetFirstLocalPlayerFromController());

	if (IsValid(HWPC) && ChatInputClass)
	{
		ChatInputInstance = CreateWidget<UHWChatInput>(this, ChatInputClass);
		ChatInputInstance->AddToViewport();
	}
}

void UHWHUD::SetShowButton(bool bIsShow)
{
	StartButton->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UHWHUD::SetTurnText(int32 CurTurnCount, int32 MaxCount)
{
	FString TurnString =  TEXT("Turn : ") + FString::FromInt(CurTurnCount) + TEXT(" / ") + FString::FromInt(MaxCount);
	TurnText->SetText(FText::FromString(TurnString));
}
