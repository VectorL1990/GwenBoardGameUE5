// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Base/GwenBoardGameInstance.h"

void ABasicGameMode::CallGameInstanceSendData(FString sendData)
{
				UGwenBoardGameInstance* gi = Cast<UGwenBoardGameInstance>(GetWorld()->GetGameInstance());
				TArray<uint8> bytes = gi->Conv_StringToBytes(sendData);
				gi->SendData(1, bytes);
}