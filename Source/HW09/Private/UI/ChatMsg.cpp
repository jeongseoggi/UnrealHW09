// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatMsg.h"
#include "Components/TextBlock.h"

void UChatMsg::SetChatMessage(const FString& Message, FLinearColor TextColor)
{
	ChatMsgBlock->SetText(FText::FromString(Message));
	ChatMsgBlock->SetColorAndOpacity(TextColor);
	ChatMsgBlock->Font.Size = 20.0f;
}
