// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameModeBase.h"
#include "Scripts/BattleEvents.h"

void ACoreCardGameModeBase::BeginPlay()
{
    InitEvents();
    InitKBEMain();
    InitDone();
}

void ACoreCardGameModeBase::InitEvents()
{
    Super::InitEvents();
    KBENGINE_REGISTER_EVENT("onSyncPlayerBattleInfo", onSyncPlayerBattleInfo);
    KBENGINE_REGISTER_EVENT("onSyncChangeHandCardSuccess", onSyncChangeHandCardSuccess);
    KBENGINE_REGISTER_EVENT("onSyncExhaustCardReplacement", onSyncExhaustCardReplacement);
    KBENGINE_REGISTER_EVENT("onSyncUpdateSelectedCards", onSyncUpdateSelectedCards);
    KBENGINE_REGISTER_EVENT("onSyncRoomStartBattle", onSyncRoomStartBattle);
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

void ACoreCardGameModeBase::ReqEnterRoom()
{
    UKBEventData* eventData = NewObject<UKBEventData>();
    KBENGINE_EVENT_FIRE("ReqEnterRoom", eventData);
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

void ACoreCardGameModeBase::ReqChangeSelectCard(FString changeCardKey)
{
    UKBEventData_reqChangeSelectCard* eventData = NewObject<UKBEventData_reqChangeSelectCard>();
    eventData->changeCardKey = changeCardKey;
    KBENGINE_EVENT_FIRE("ReqChangeSelectCard", eventData);
}

void ACoreCardGameModeBase::ReqUpdateSelectedCard()
{
    UKBEventData* eventData = NewObject<UKBEventData>();
    KBENGINE_EVENT_FIRE("ReqUpdateSelectedCard", eventData);
}

void ACoreCardGameModeBase::onUpdateGridInfoList(const UKBEventData* eventData)
{
    /*auto updateGridInfoListData = Cast<UKBEventData_onUpdateGridInfoList>(eventData);
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
    }*/
}

void ACoreCardGameModeBase::onReceiveNewTurnMessage(const UKBEventData* eventData)
{
    curBattleStatus = BattleStatus::BattleCountDown;
}

void ACoreCardGameModeBase::onReceiveUpdateCoreGame(const UKBEventData* eventData)
{
    auto updateBattleEventData = Cast<UKBEventData_onUpdateBattle>(eventData);

    /*TArray<int32> latestOccupiedGrids;
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
    }*/
}

void ACoreCardGameModeBase::InitDone_Implementation()
{

}

void ACoreCardGameModeBase::onSyncChangeHandCardSuccess(const UKBEventData* eventData)
{
    const UKBEventData_onSyncChangeHandCardSuccess* onSyncChangeHandCardSuccessData = Cast<UKBEventData_onSyncChangeHandCardSuccess>(eventData);
    if (onSyncChangeHandCardSuccessData->changeSelectCardNb - curChangeSelectCardNb == 1)
    {
        if (handCardKeyList.Contains(onSyncChangeHandCardSuccessData->changeHandCardKey) &&
            pileCardKeyList.Contains(onSyncChangeHandCardSuccessData->changePileCardKey))
        {
            // which means information is complete, nothing missed
            curChangeSelectCardNb = onSyncChangeHandCardSuccessData->changeSelectCardNb;

            // delete choosen hand card and replace it with card in card pile
            int32 changeHandCardKeyLoc = handCardKeyList.Find(onSyncChangeHandCardSuccessData->changeHandCardKey);
            handCardKeyList[changeHandCardKeyLoc] = onSyncChangeHandCardSuccessData->changePileCardKey;
            // remove target pile card key from pileCardKeyList
            pileCardKeyList.RemoveAt(0);
            pileCardKeyList.Add(onSyncChangeHandCardSuccessData->changeHandCardKey);
            if (!handCardMap.Contains(onSyncChangeHandCardSuccessData->changeHandCardKey))
            {
                // which means 3d card error, regenerate all 3d cards!
            }
            else
            {
                FVector changeHandCardLoc = handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->GetActorLocation();
                FRotator changeHandCardRot = handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->GetActorRotation();
                handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->Destroy();
                handCardMap.Remove(onSyncChangeHandCardSuccessData->changeHandCardKey);
                ACard* changeHandCard = GetWorld()->SpawnActor<ACard>(cardBPClass, changeHandCardLoc, changeHandCardRot);
                changeHandCard->cardStatus = BattleCardStatus::Select;
                changeHandCard->InitCard(allCardInfoMap[onSyncChangeHandCardSuccessData->changeHandCardKey].cardName);
                handCardMap.Add(onSyncChangeHandCardSuccessData->changeHandCardKey, changeHandCard);
            }
            if (maxChangeSelectCardNb - curChangeSelectCardNb <= 1)
            {
                // which means all opportunity exhausted
                // tell server 
            }
        }
        else
        {
            // which means information loss, require latest select hand cards!
        }
    }
    else
    {
        // which means information loss, require latest select hand cards!
    }
}

void ACoreCardGameModeBase::onSyncPlayerBattleInfo(const UKBEventData* eventData)
{
    const UKBEventData_onSyncPlayerBattleInfo* onSyncPlayerBattleInfoData = Cast<UKBEventData_onSyncPlayerBattleInfo>(eventData);

    for (int32 i = 0; i < onSyncPlayerBattleInfoData->cardList.Num(); i++)
    {
        if (!onSyncPlayerBattleInfoData->handCardList.Contains(onSyncPlayerBattleInfoData->cardList[i].cardKey))
        {
            pileCardKeyList.Add(onSyncPlayerBattleInfoData->cardList[i].cardKey);
        }
        allCardInfoMap.Add(onSyncPlayerBattleInfoData->cardList[i].cardKey, onSyncPlayerBattleInfoData->cardList[i]);
    }
    handCardKeyList = onSyncPlayerBattleInfoData->handCardList;
}

void ACoreCardGameModeBase::onSyncExhaustCardReplacement(const UKBEventData* eventData)
{
    // which means this player lost some information
    // player should ask server to supplement all informations
    UKBEventData* eventData = NewObject<UKBEventData>();
    KBENGINE_EVENT_FIRE("ReqUpdateSelectedCard", eventData);
}

void ACoreCardGameModeBase::onSyncUpdateSelectedCards(const UKBEventData* eventData)
{
    const UKBEventData_onSyncUpdateSelectedCards* onSyncUpdateSelectedCardsData = Cast<UKBEventData_onSyncUpdateSelectedCards>(eventData);
    // delete all existing hand cards and replace them with data from server
    for (TMap<FString, ACard*>::TConstIterator iter = handCardMap.CreateConstIterator(); iter; ++iter)
    {
        if (iter->Value->IsValidLowLevel())
        {
            iter->Value->Destroy();
        }
    }
    handCardMap.Empty();

    allCardInfoMap.Empty();
    pileCardKeyList.Empty();
    for (int32 i = 0; i < onSyncUpdateSelectedCardsData->cardList.Num(); i++)
    {
        if (!onSyncUpdateSelectedCardsData->handCardList.Contains(onSyncUpdateSelectedCardsData->cardList[i].cardKey))
        {
            pileCardKeyList.Add(onSyncUpdateSelectedCardsData->cardList[i].cardKey, onSyncUpdateSelectedCardsData->cardList[i]);
        }
        allCardInfoMap.Add(onSyncUpdateSelectedCardsData->cardList[i].cardKey, onSyncUpdateSelectedCardsData->cardList[i]);
    }
    handCardKeyList = onSyncUpdateSelectedCardsData->handCardList;

    FRotator spawnRot = FRotator::ZeroRotator;
    for (int32 i = 0; i < handCardKeyList.Num(); i++)
    {
        if (i >= selectCardSpawnPts.Num())
        {
            break;
        }
        FVector spawnLoc = selectCardSpawnPts[i];
        ACard* handCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
        handCard->cardStatus = BattleCardStatus::Select;
        handCard->InitCard(allCardInfoMap[handCardKeyList[i]].cardName);
        handCardMap.Add(handCardKeyList[i], handCard);
    }
}

void ACoreCardGameModeBase::onSyncRoomStartBattle(const UKBEventData* eventData)
{

}

void ACoreCardGameModeBase::InitPlayerBattleInfoDone(TArray<FString> cardList)
{
    /*FRotator spawnRot = FRotator::ZeroRotator;
    for (int32 i = 0; i < cardList.Num(); i++)
    {
        FVector spawnLoc = selectCardDemoLoc + i * cardIntervalVector;
        ACard* newCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
        newCard->cardStatus = BattleCardStatus::Select;
        newCard->InitCard(cardList[i]);
        selectCards.Add(newCard);
    }*/

    FRotator spawnRot = FRotator::ZeroRotator;
    for (int32 i = 0; i < handCardKeyList.Num(); i++)
    {
        if (i >= selectCardSpawnPts.Num())
        {
            break;
        }
        FVector spawnLoc = selectCardSpawnPts[i];
        ACard* handCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
        handCard->cardStatus = BattleCardStatus::Select;
        handCard->InitCard(allCardInfoMap[handCardKeyList[i]].cardName);
        handCardMap.Add(handCardKeyList[i], handCard);
    }
}

