// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Engine/KBEngine.h"
#include "Base/GwenBoardGameInstance.h"

void ABasicGameMode::CallGameInstanceSendData(FString sendData)
{
				UGwenBoardGameInstance* gi = Cast<UGwenBoardGameInstance>(GetWorld()->GetGameInstance());
				TArray<uint8> bytes = gi->Conv_StringToBytes(sendData);
				gi->SendData(1, bytes);
}

void ABasicGameMode::InitEvents()
{
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onKicked, onKicked);
}

void ABasicGameMode::onKicked(const UKBEventData* eventData)
{

}

void ABasicGameMode::onDisconnected(const UKBEventData* eventData)
{

}
