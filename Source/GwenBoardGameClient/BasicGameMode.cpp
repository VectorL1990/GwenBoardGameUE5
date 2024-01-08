// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/KBEngine.h"
#include "Engine/KBEMain.h"
#include "Scripts/BattleEvents.h"
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
				KBENGINE_REGISTER_EVENT("onSyncRoomCreated", onSyncRoomCreated);
				KBENGINE_REGISTER_EVENT("onSyncPlayerBattleInfo", onSyncPlayerBattleInfo);
}

void ABasicGameMode::onKicked(const UKBEventData* eventData)
{

}

void ABasicGameMode::onDisconnected(const UKBEventData* eventData)
{

}

void ABasicGameMode::ReqLogin(FString playerName, FString pwd)
{
				UActorComponent* actorComponent = kbeMainActor->GetComponentByClass(UKBEMain::StaticClass());
				UKBEMain* kbeMain = Cast<UKBEMain>(actorComponent);
				KBEngine::KBVar::KBVarBytes bytes;
				kbeMain->login(playerName, pwd, bytes);
}

void ABasicGameMode::ReqMatch()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqMatch", eventData);
}

void ABasicGameMode::ReqEnterRoom()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqEnterRoom", eventData);
}

void ABasicGameMode::ReqCreateAccount(FString playerName, FString pwd)
{
				UActorComponent* actorComponent = kbeMainActor->GetComponentByClass(UKBEMain::StaticClass());
				UKBEMain* kbeMain = Cast<UKBEMain>(actorComponent);
				KBEngine::KBVar::KBVarBytes bytes;
				kbeMain->createAccount(playerName, pwd, bytes);
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

void ABasicGameMode::onSyncRoomCreated(const UKBEventData* eventData)
{
				const UKBEventData_onSyncRoomCreated* onSyncRoomCreatedData = Cast<UKBEventData_onSyncRoomCreated>(eventData);
				sRoomKey = onSyncRoomCreatedData->sRoomKey;
				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, "onSyncRoomCreated room key is: " + sRoomKey);
				UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
				UGwenBoardGameInstance* gwenGameInstance = Cast<UGwenBoardGameInstance>(gameInstance);
				gwenGameInstance->curRoomKey = sRoomKey;
				// at this point we should switch to battle level
				// and then we send message to server about entering room
				UGameplayStatics::OpenLevel(this, "DesertBoardMap");
}

void ABasicGameMode::onSyncPlayerBattleInfo(const UKBEventData* eventData)
{
				const UKBEventData_onSyncPlayerBattleInfo* onSyncPlayerBattleInfoData = Cast<UKBEventData_onSyncPlayerBattleInfo>(eventData);
				for (int i = 0; i < onSyncPlayerBattleInfoData->cardList.Num(); i++)
				{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, onSyncPlayerBattleInfoData->cardList[i].cardKey);
				}
				//InitPlayerBattleInfoDone(onSyncPlayerBattleInfoData->cardList);
}

void ABasicGameMode::SpawnSelectCard(TArray<FString> cardList)
{

}
