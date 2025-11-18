// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HWHUD.generated.h"

class UProgressBar;
class UHWChatInput;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class HW09_API UHWHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> TimerProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TurnText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHWChatInput> ChatInputClass;

	UPROPERTY()
	TObjectPtr<UHWChatInput> ChatInputInstance;

	UFUNCTION()
	UProgressBar* GetTimerProgressBar() const;

	UFUNCTION()
	void ChatInputAdd();

	UFUNCTION(BlueprintCallable)
	void SetShowButton(bool bIsShow);

	UFUNCTION(BlueprintCallable)
	void SetTurnText(int32 CurTurnCount, int32 MaxCount);
};
