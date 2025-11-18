// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HWPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIndexChanged, int32, NewPlayerIndex);

/**
 * 
 */
UCLASS()
class HW09_API AHWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AHWPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex)
	int32 PlayerIndex;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerIndexChanged OnPlayerIndexChanged;

protected:

	UFUNCTION()
	void OnRep_PlayerIndex();

};
