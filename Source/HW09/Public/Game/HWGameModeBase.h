// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HWGameModeBase.generated.h"

class AHWPlayerController;
/**
 * 
 */
UCLASS()
class HW09_API AHWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateNumber();

	bool IsGuessNumber(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void AddMsgBox(AHWPlayerController* InChattingPlayerController, const FString& Message);

	void SendToClientMsg(AHWPlayerController* InChattingPlayerController, const FString& Message);
	
	void StartGame();

	void JudgeGame(AHWPlayerController* InChattingPlayerController, int32 StrikeCount);

	void IncreaseGuessCount(AHWPlayerController* InChattingPlayerController);

	void ResetGame();

	void NextTurn(bool bIsTimeOver = false);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AHWPlayerController>> AllPlayerControllers;

	UPROPERTY(VisibleAnywhere)
	int32 CurPlayerIndex;
};
