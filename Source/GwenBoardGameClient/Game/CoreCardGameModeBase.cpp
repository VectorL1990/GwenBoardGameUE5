// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameModeBase.h"
#include "Scripts/BattleEvents.h"

void ACoreCardGameModeBase::InitEvents()
{
    KBENGINE_REGISTER_EVENT("syncInitializeAllCard", syncInitializeAllCard);
}

void ACoreCardGameModeBase::Tick(float deltaTime)
{
    if (curBattleStatus == BattleStatus::SwitchControllerAnimationCounting)
    {
        if (curCountingTick >= maxCoutingTicksMap["SwitchControllerAnimationCounting"])
        {
            // which means tick ends, send message to server 
            curCountingTick = 0.0;
            curBattleStatus = BattleStatus::Default;
        }
        else
        {
            curCountingTick += deltaTime;
        }
    }
}

void ACoreCardGameModeBase::syncPlayerPersistInfo(const UKBEventData* eventData)
{
    UKBEventData_syncPersistPlayerInfo persistPlayerInfo = Cast<UKBEventData_syncPersistPlayerInfo>(eventData);
    for (int32 i = 0; i < persistPlayerInfo.persistPlayerInfo.cardList.Num(); i++)
    {
        
    }
}

void ACoreCardGameModeBase::syncInitializeAllCard(const UKBEventData* eventData)
{

}

void ACoreCardGameModeBase::ReqEnterRoom()
{

}

void ACoreCardGameModeBase::ReqChangeHandCard(int32 changeCardId)
{

}

void ACoreCardGameModeBase::ReqPlayCard(int32 targetGridNb, int32 playCardUid)
{
    UKBEventData_reqSendAction* eventData = NewObject<UKBEventData_reqSendAction>();
    eventData->actionType = "playCard";
    FPLAY_CARD_KBE_INFO playCardInfo;
    playCardInfo.playCardId = playCardUid;
    playCardInfo.targetGridNb = targetGridNb;
    eventData->playCardInfo = playCardInfo;
    KBENGINE_EVENT_FIRE("ReqSendAction", eventData);
}

void ACoreCardGameModeBase::onUpdateGridInfoList(const UKBEventData* eventData)
{
    auto updateGridInfoListData = Cast<UKBEventData_onUpdateGridInfoList>(eventData);
    if (updateGridInfoListData->updateInfoList.updateId - curReceiveUpdateId == 1)
    {
        // which means receives latest data
        curReceiveUpdateId = updateGridInfoListData->updateInfoList.updateId;
        for (int32 i = 0; i < updateGridInfoListData->updateInfoList.updateGridList.Num(); i++)
        {
            if (updateGridInfoListData->updateInfoList.updateGridList[i].updateType == "createCard")
            {
                // TODO: should consider cases which cards missed or grid occupied 
                boardGrids[updateGridInfoListData->updateInfoList.updateGridList[i].gridNb]->card =
                    allCards[updateGridInfoListData->updateInfoList.updateGridList[i].cardUid];

                // and then play animation
            }
            else if (updateGridInfoListData->updateInfoList.updateGridList[i].updateType == "deleteCard")
            {

            }
        }
    }
}

void ACoreCardGameModeBase::onReceiveNewTurnMessage(const UKBEventData* eventData)
{
    curBattleStatus = BattleStatus::BattleCountDown;
}

void ACoreCardGameModeBase::onReceiveUpdateCoreGame(const UKBEventData* eventData)
{
    auto updateBattleEventData = Cast<UKBEventData_onUpdateBattle>(eventData);

    TArray<int32> latestOccupiedGrids;
    // update all grids
    for (int32 i = 0; i < updateBattleEventData->updateInfo.updateInfos.Num(); i++)
    {
        if (occupiedGridCardMap.Contains(updateBattleEventData->updateInfo.updateInfos[i].gridNb))
        {

        }
        else
        {
            // which means this is a createtion action
            occupiedGridCardMap.Add(updateBattleEventData->updateInfo.updateInfos[i].gridNb,
                allCards[updateBattleEventData->updateInfo.updateInfos[i].cardUid]);
        }
        latestOccupiedGrids.Add(updateBattleEventData->updateInfo.updateInfos[i].gridNb);
    }

    for (TPair<int32, ACard*>& element : occupiedGridCardMap)
    {
        if (!latestOccupiedGrids.Contains(element.Key))
        {
            // which means card in this grid is removed!
            // we should move this card to grave
        }
    }
}

