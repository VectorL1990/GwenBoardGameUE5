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
				KBENGINE_REGISTER_EVENT("ReceivePlayerPersistInfo", ReceivePlayerPersistInfo);
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

void ABasicGameMode::ReceivePlayerPersistInfo(const UKBEventData* eventData)
{
				const UKBEventData_ReceivePlayerPersistInfo* castEventData = Cast<UKBEventData_ReceivePlayerPersistInfo>(eventData);
				for (int32 i = 0; i < castEventData->cardGroups.Num(); i++)
				{

				}
}

void ABasicGameMode::SetCardInfo(FString cardName,
				FString aliasCN,
				FString aliasEN,
				FString cardFunctionCN,
				FString cardFunctionEN,
				FString despCN,
				FString despEN,
				int32 hp,
				int32 defence,
				int32 agility,
				TArray<FString> cardTags,
				FString launchType,
				int32 coolDown,
				int32 availableTimes,
				FString launchGeoType,
				FString autoSkillTargetGeoType,
				FString targetGeoType,
				FString aoeType,
				FString targetCamp,
				FString effectType,
				FString effectAffix,
				FString effectAffixCamp,
				FString prereqTagCondition,
				FString prereqTag,
				FString prereqCampType,
				FString prereqType,
				FString passivePrereqType,
				TArray<int32> values,
				UTexture* texture,
				FString moveType,
				FString cardCategory)
{
				FCardInfo cardInfo;
				cardInfo.cardName = cardName;
				cardInfo.aliasCN = aliasCN;
				cardInfo.aliasEN = aliasEN;
				cardInfo.cardFunctionCN = cardFunctionCN;
				cardInfo.cardFunctionEN = cardFunctionEN;
				cardInfo.despCN = despCN;
				cardInfo.despEN = despEN;
				cardInfo.hp = hp;
				cardInfo.defence = defence;
				cardInfo.agility = agility;
				cardInfo.cardTags = cardTags;
				cardInfo.launchType = launchType;
				cardInfo.coolDown = coolDown;
				cardInfo.availableTimes = availableTimes;
				cardInfo.launchGeoType = launchGeoType;
				cardInfo.autoSkillTargetGeoType = autoSkillTargetGeoType;
				cardInfo.targetGeoType = targetGeoType;
				cardInfo.aoeType = aoeType;
				cardInfo.targetCamp = targetCamp;
				cardInfo.effectType = effectType;
				cardInfo.effectAffix = effectAffix;
				cardInfo.effectAffixCamp = effectAffixCamp;
				cardInfo.prereqTagCondition = prereqTagCondition;
				cardInfo.prereqTag = prereqTag;
				cardInfo.prereqCampType = prereqCampType;
				cardInfo.prereqType = prereqType;
				cardInfo.passivePrereqType = passivePrereqType;
				cardInfo.values = values;
				cardInfo.texture = texture;
				cardInfo.moveType = moveType;
				cardInfo.cardCategory = cardCategory;
				allCardInfos.Add(cardName, cardInfo);
}

void ABasicGameMode::SpawnSelectCard()
{

}
