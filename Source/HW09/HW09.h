// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class NumberBaseBallFuctionLibrary
{
public:
	static void PrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDelay = 1.0f, FColor InColor = FColor::Cyan)
	{
		if (IsValid(GEngine) && IsValid(InWorldContextActor))
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDelay, InColor, InString);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InString : %s"), *InString);
		}
	}
};

