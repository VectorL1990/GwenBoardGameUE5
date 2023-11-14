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

				
}

void ALoginGameModeBase::ReqTest()
{
				UKBEventData_ReqTest* eventData = NewObject<UKBEventData_ReqTest>();
				eventData->testParam = 200;
				KBENGINE_EVENT_FIRE("reqTest", eventData);
}

void ALoginGameModeBase::onReqTest(const UKBEventData* eventData)
{
				const UKBEventData_onReqTest* onReqTestEventData = Cast<UKBEventData_onReqTest>(eventData);
				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, FString::FromInt(onReqTestEventData->testParam));
}

void ALoginGameModeBase::BasicInitDone_Implementation()
{

}
