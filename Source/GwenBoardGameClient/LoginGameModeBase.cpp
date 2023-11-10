// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "Base/GwenBoardGameInstance.h"
#include "Scripts/BattleEvents.h"
#include "Engine/KBEngine.h"
#include "Engine/KBEMain.h"
#include "UnLua.h"

void ALoginGameModeBase::BeginPlay()
{
				Super::InitEvents();
				InitEvents();
				ReqTest();
				InitKBEMain();
				BasicInitDone();
}

void ALoginGameModeBase::InitEvents()
{
				Super::InitEvents();

				KBENGINE_REGISTER_EVENT("onSyncPlayerBattleInfo", onSyncPlayerBattleInfo);
}

void ALoginGameModeBase::ReqTest()
{
				UKBEventData_ReqTest* eventData = NewObject<UKBEventData_ReqTest>();
				eventData->testParam = 200;
				KBENGINE_EVENT_FIRE("reqTest", eventData);
}

void ALoginGameModeBase::ReqEnterRoom()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqEnterRoom", eventData);
}

void ALoginGameModeBase::onReqTest(const UKBEventData* eventData)
{
				const UKBEventData_onReqTest* onReqTestEventData = Cast<UKBEventData_onReqTest>(eventData);
				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, FString::FromInt(onReqTestEventData->testParam));
}

void ALoginGameModeBase::onSyncPlayerBattleInfo(const UKBEventData* eventData)
{
				const UKBEventData_onSyncPlayerBattleInfo* onSyncPlayerBattleInfoData = Cast<UKBEventData_onSyncPlayerBattleInfo>(eventData);
				for (int i = 0; i < onSyncPlayerBattleInfoData->cardList.Num(); i++)
				{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, onSyncPlayerBattleInfoData->cardList[i]);
				}
}

void ALoginGameModeBase::BasicInitDone_Implementation()
{

}
