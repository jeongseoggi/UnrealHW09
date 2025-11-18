// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HWPlayerController.generated.h"

class UHWChatInput;
class UUserWidget;
class UHWHUD;
/**
 * 
 */
UCLASS()
class HW09_API AHWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHWPlayerController();

	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);
	
	void PrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_PlayerState() override;

	void SetProgressBar(const float& RemainTime);

	void SetMessageBox(const FString& Message, FLinearColor TextColor = FLinearColor::White);

	void SetNotificationMsg(const FString& NotiMsg);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& Message, FLinearColor TextColor = FLinearColor::White);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& Message);

	UFUNCTION(Server, Reliable)
	void ServerRPCGameStart();

	UFUNCTION(Client, Reliable)
	void ClientRPCSetStartButton(bool bIsShow);

	UFUNCTION()
	void SetTurnText(int32 NewGuessCount, int32 MaxGuessCount);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHWChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UHWChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHWHUD> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHWHUD> HUDWidgetInstance;

	FString ChatMessageString;

	UFUNCTION()
	void HUDButtonHandle(int32 NewIndex);

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

	UPROPERTY(Replicated)
	float TurnTime;
};
