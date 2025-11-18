// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HWGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API AHWGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHWGameStateBase();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastMessage(const FString& NotiMessage);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetIsGameStart() const;

	void SetIsGameStart(bool bIsStart);

	void StartTurnTimer();

	void TickTurnTimer();

	void ResetTimer();

protected:
	UPROPERTY(Replicated)
	bool bIsGameStart;

	UPROPERTY(ReplicatedUsing = OnRep_TurnRemainingTime)
	float TurnTime;
	
	FTimerHandle TurnTimerHandle;

	UFUNCTION()
	void OnRep_TurnRemainingTime();

	float CurTime;
};
