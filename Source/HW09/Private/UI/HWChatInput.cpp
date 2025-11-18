// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HWChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/HWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ChatMsg.h"
#include "Components/ScrollBox.h"

void UHWChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	if (EditableTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHWChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		EditableTextBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHWChatInput::AddToChatBox(const FString& Message, FLinearColor TextColor)
{
	if (ChatMsgClass)
	{
		UChatMsg* ChatMsg = CreateWidget<UChatMsg>(this, ChatMsgClass);
		ChatMsg->SetChatMessage(Message, TextColor);
		ScrollBox->AddChild(ChatMsg);
		ScrollBox->ScrollToEnd();
		ScrollBox->bAnimateWheelScrolling = true;
	}
}

void UHWChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* PC = GetOwningPlayer();
		if (IsValid(PC))
		{
			AHWPlayerController* HWPC = Cast<AHWPlayerController>(PC);
			if (IsValid(HWPC))
			{
				HWPC->SetChatMessageString(Text.ToString());
				EditableTextBox->SetText(FText());
				EditableTextBox->SetIsEnabled(true);

				//Slate가 모든 커밋 처리를 끝낸 뒤인 다음 프레임에 포커스
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
				{
						//UMG 이벤트와 + 안정성을 위해 UMG의 포커스 Slate의 포커스 두 번 사용
						EditableTextBox->SetKeyboardFocus();
						FSlateApplication::Get().SetKeyboardFocus(EditableTextBox->TakeWidget(), EFocusCause::SetDirectly);
				});
			}
		}
	}
}
