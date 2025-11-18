// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HWChatInput.generated.h"

class UEditableTextBox;
class UScrollBox;
class UChatMsg;

/**
 * 
 */
UCLASS()
class HW09_API UHWChatInput : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void AddToChatBox(const FString& Message, FLinearColor TextColor);

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatMsg> ChatMsgClass;
};
