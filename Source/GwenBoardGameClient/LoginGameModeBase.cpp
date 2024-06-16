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
				InitKBEMain();
				ReqTest();
				BasicInitDone();
}

void ALoginGameModeBase::InitEvents()
{
				Super::InitEvents();

				
}

void ALoginGameModeBase::ReqTest()
{
				UKBEventData_ReqTest* eventData = NewObject<UKBEventData_ReqTest>();
				eventData->testParam = 200;
				KBENGINE_EVENT_FIRE("reqTest", eventData);
}

void ALoginGameModeBase::ReqModifyCardGroup()
{
				UKBEventData_ReqModifyCardGroup* eventData = NewObject<UKBEventData_ReqModifyCardGroup>();
				eventData->groupNb = 0;
				eventData->cardList.Add("aaa");
				eventData->cardList.Add("bbb");
				eventData->cardList.Add("ccc");
				eventData->cardList.Add("ddd");
				KBENGINE_EVENT_FIRE("ReqModifyCardGroup", eventData);
}

void ALoginGameModeBase::onReqTest(const UKBEventData* eventData)
{
				const UKBEventData_onReqTest* onReqTestEventData = Cast<UKBEventData_onReqTest>(eventData);
				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, FString::FromInt(onReqTestEventData->testParam));
}

void ALoginGameModeBase::BasicInitDone_Implementation()
{

}

void ALoginGameModeBase::SpawnSelectCard()
{

}
