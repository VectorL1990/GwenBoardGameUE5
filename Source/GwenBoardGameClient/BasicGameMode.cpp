// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Engine/KBEngine.h"
#include "Engine/KBEMain.h"
#include "Base/GwenBoardGameInstance.h"

void ABasicGameMode::CallGameInstanceSendData(FString sendData)
{
    UGwenBoardGameInstance* gi = Cast<UGwenBoardGameInstance>(GetWorld()->GetGameInstance());
    TArray<uint8> bytes = gi->Conv_StringToBytes(sendData);
    gi->SendData(1, bytes);
}

/*
void ABasicGameMode::BeginPlay()
{
				Super::BeginPlay();

				InitEvents();
}*/

void ABasicGameMode::InitEvents()
{
    KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onKicked, onKicked);
				KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onCreateAccountResult, onCreateAccountResult);
}

void ABasicGameMode::onKicked(const UKBEventData* eventData)
{

}

void ABasicGameMode::onDisconnected(const UKBEventData* eventData)
{

}

void ABasicGameMode::ReqLogin()
{
				UActorComponent* actorComponent = kbeMainActor->GetComponentByClass(UKBEMain::StaticClass());
				UKBEMain* kbeMain = Cast<UKBEMain>(actorComponent);
				KBEngine::KBVar::KBVarBytes bytes;
				kbeMain->login("test111", "1234", bytes);
}

void ABasicGameMode::ReqCreateAccount()
{
				UActorComponent* actorComponent = kbeMainActor->GetComponentByClass(UKBEMain::StaticClass());
				UKBEMain* kbeMain = Cast<UKBEMain>(actorComponent);
				KBEngine::KBVar::KBVarBytes bytes;
				kbeMain->createAccount("test111", "1234", bytes);
}

void ABasicGameMode::InitKBEMain()
{
				FVector spawnLoc(0,0,0);
				FRotator spawnRot(0,0,0);
				kbeMainActor = GetWorld()->SpawnActor<AActor>(kbeMainClass, spawnLoc, spawnRot);
}

void ABasicGameMode::onCreateAccountResult(const UKBEventData* eventData)
{
				const UKBEventData_onCreateAccountResult* onCreateAccountEventData = Cast<UKBEventData_onCreateAccountResult>(eventData);
				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, onCreateAccountEventData->errorStr);
}
