// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HWGameModeBase.h"
#include "Player/HWPlayerController.h"
#include "Player/HWPlayerState.h"
#include "Game/HWGameStateBase.h"
#include "EngineUtils.h"

void AHWGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHWPlayerController* HWPC = Cast<AHWPlayerController>(NewPlayer);

	if (IsValid(HWPC))
	{
		AllPlayerControllers.Add(HWPC);


		if (AHWPlayerState* HWPS = HWPC->GetPlayerState<AHWPlayerState>())
		{
			HWPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
			HWPS->PlayerIndex = AllPlayerControllers.Num();

			if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
			{
				FString JoinMsg = HWPS->PlayerNameString + TEXT(" Has Joined Game.");
				HWGS->MulticastRPCBroadcastMessage(JoinMsg);
			}
		}
	}
}

FString AHWGameModeBase::GenerateNumber()
{
	TArray<int32> NumArr;

	for (int32 i = 1; i <= 9; ++i)
	{
		NumArr.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	NumArr = NumArr.FilterByPredicate([](int32 Num) {return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, NumArr.Num() - 1);
		Result.Append(FString::FromInt(NumArr[Index]));
		NumArr.RemoveAt(Index);
	}

	return Result;
}

bool AHWGameModeBase::IsGuessNumber(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (!bIsUnique)
		{
			break;
		}

		bCanPlay = UniqueDigits.Num() == 3 ? true : false;

	} while (false);

	return bCanPlay;
}

FString AHWGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StCount = 0;
	int32 BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StCount++;
		}
		else
		{
			FString GuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(GuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StCount, BallCount);
}

void AHWGameModeBase::AddMsgBox(AHWPlayerController* InChattingPlayerController, const FString& Message)
{
	int Index = Message.Len() - 3;
	FString GuessNumString = Message.RightChop(Index);
	AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>();

	if (IsValid(HWGS))
	{
		if (HWGS->GetIsGameStart() && AllPlayerControllers[CurPlayerIndex] == InChattingPlayerController)
		{
			if (IsGuessNumber(GuessNumString))
			{
				FString ResultStr = JudgeResult(SecretNumberString, GuessNumString);
				IncreaseGuessCount(AllPlayerControllers[CurPlayerIndex]);
				// ex) Notification Msg => Player1 Guess : 321 / Result : 1S2B
				if (IsValid(HWGS))
				{
					if (AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>())
					{
						FString CombinedMsg = HWPS->PlayerNameString + TEXT(" Guess : ") + GuessNumString + TEXT(" / ") + ResultStr;
						HWGS->MulticastRPCBroadcastMessage(CombinedMsg);

						SendToClientMsg(InChattingPlayerController, Message);

						int32 StrikeCount = FCString::Atoi(*ResultStr.Left(1));
						JudgeGame(InChattingPlayerController, StrikeCount);
					}
				}
			}

		}
		else
		{
			SendToClientMsg(InChattingPlayerController, Message);
		}
	}

}

void AHWGameModeBase::SendToClientMsg(AHWPlayerController* InChattingPlayerController, const FString& Message)
{
	FString CombinedString;
	bool bIsSystemMsg = false;
	if (InChattingPlayerController == nullptr)
	{
		CombinedString = TEXT("System Message : ") + Message;
		bIsSystemMsg = true;
	}
	else if (AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>())
	{
		CombinedString = HWPS->PlayerNameString + Message;
	}

	for (TActorIterator<AHWPlayerController> It(GetWorld()); It; ++It)
	{
		AHWPlayerController* HWPC = *It;
		if (IsValid(HWPC) == true)
		{
			HWPC->ClientRPCPrintChatMessage(CombinedString, bIsSystemMsg ? FLinearColor::Yellow : FLinearColor::White);
		}
	}
}

void AHWGameModeBase::StartGame()
{
	AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>();
	if (IsValid(HWGS))
	{
		if (AllPlayerControllers.Num() < 2)
		{
			HWGS->MulticastRPCBroadcastMessage(TEXT("Not Enough Player"));
		}
		else
		{
			SecretNumberString = GenerateNumber();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *SecretNumberString);

			CurPlayerIndex = 0;
			HWGS->MulticastRPCBroadcastMessage(TEXT("GameStart!"));
			HWGS->SetIsGameStart(true);
			HWGS->StartTurnTimer();
			SendToClientMsg(nullptr, TEXT("GameStart!"));

			if (AHWPlayerState* HWPS = AllPlayerControllers[CurPlayerIndex]->GetPlayerState<AHWPlayerState>())
			{
				FString Msg = HWPS->PlayerNameString + " Turn!";
				SendToClientMsg(nullptr, Msg);
			}
		}
	}
}

void AHWGameModeBase::JudgeGame(AHWPlayerController* InChattingPlayerController, int32 StrikeCount)
{
	if (StrikeCount == 3)
	{
		AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>();
		for (const auto& HWPC : AllPlayerControllers)
		{
			if (IsValid(HWPS))
			{
				FString CombinedMessageString = HWPS->PlayerNameString + TEXT(" Has Won the Game!");
				if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
				{
					HWGS->MulticastRPCBroadcastMessage(CombinedMessageString);
				}

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& HWPlayerController : AllPlayerControllers)
		{
			if (AHWPlayerState* HWPS = HWPlayerController->GetPlayerState<AHWPlayerState>())
			{
				if (HWPS->CurrentGuessCount < HWPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (bIsDraw)
		{
			for (const auto& HWPlayerController : AllPlayerControllers)
			{
				if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
				{
					HWGS->MulticastRPCBroadcastMessage(TEXT("Draw.."));
				}
				ResetGame();
			}
		}
		else
		{
			if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
			{
				HWGS->StartTurnTimer();
				NextTurn();
			}
		}
	}
}

void AHWGameModeBase::IncreaseGuessCount(AHWPlayerController* InChattingPlayerController)
{
	if (AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>())
	{
		HWPS->CurrentGuessCount++;
	}
}

void AHWGameModeBase::ResetGame()
{
	for (int i = 0 ; i < AllPlayerControllers.Num(); ++i)
	{
		if (AHWPlayerState* HWPS = AllPlayerControllers[i]->GetPlayerState<AHWPlayerState>())
		{
			HWPS->CurrentGuessCount = 0;
			if (i == 0)
			{
				AllPlayerControllers[i]->ClientRPCSetStartButton(true);
			}
		}
	}

	if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
	{
		HWGS->SetIsGameStart(false);
		HWGS->ResetTimer();
	}
}

void AHWGameModeBase::NextTurn(bool bIsTimeOver)
{
	if (bIsTimeOver)
	{
		IncreaseGuessCount(AllPlayerControllers[CurPlayerIndex]);
		JudgeGame(AllPlayerControllers[CurPlayerIndex], 0);
	}

	CurPlayerIndex = (CurPlayerIndex + 1) % AllPlayerControllers.Num();
	if (AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>())
	{
		if (HWGS->GetIsGameStart())
		{
			HWGS->StartTurnTimer();

			if (AHWPlayerState* HWPS = AllPlayerControllers[CurPlayerIndex]->GetPlayerState<AHWPlayerState>())
			{
				FString Msg = HWPS->PlayerNameString + " Turn!";

				SendToClientMsg(nullptr, Msg);
			}
		}
	}
}
