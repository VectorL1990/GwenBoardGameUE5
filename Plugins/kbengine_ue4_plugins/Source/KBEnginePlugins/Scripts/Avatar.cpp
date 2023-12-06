#include "Avatar.h"
#include "Engine/Entity.h"
#include "LogicEvents.h"
#include "BattleEvents.h"

namespace KBEngine
{

    Avatar::Avatar() :
        AvatarBase()
    {
    }

    Avatar::~Avatar()
    {
    }

    void Avatar::__init__()
    {
        // 注册事件
        if (isPlayer())
        {
            
            KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqChangeSelectCard", "ReqChangeSelectCard", [this](const UKBEventData* pEventData)
            {
                const UKBEventData_reqChangeSelectCard* reqChangeSelectCardData = Cast<UKBEventData_reqChangeSelectCard>(pEventData);
                ReqChangeSelectCard(reqChangeSelectCardData->changeCardKey);
            });
            KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqUpdateSelectedCard", "ReqUpdateSelectedCard", [this](const UKBEventData* pEventData)
            {
                ReqUpdateSelectedCard();
            });
        }
    }

    void Avatar::onDestroy()
    {
        AvatarBase::onDestroy();

        // 注销注册的所有事件
        KBENGINE_DEREGISTER_ALL_EVENT();
    }

    void Avatar::onEnterWorld()
    {
        AvatarBase::onEnterWorld();
    }

    void Avatar::onLeaveWorld()
    {
        AvatarBase::onLeaveWorld();
    }

    void Avatar::onEnterSpace()
    {
        AvatarBase::onEnterSpace();
    }

    void Avatar::onLeaveSpace()
    {
        AvatarBase::onLeaveSpace();
    }

    void Avatar::onSyncPlayerBattleInfo(const SYNC_PLAYER_BATTLE_INFO& param)
    {
        UKBEventData_onSyncPlayerBattleInfo* eventData = NewObject<UKBEventData_onSyncPlayerBattleInfo>();
        for (int32 i = 0; i < param.cardList.Num(); i++)
        {
            FSYNC_CARD_INFO syncCardInfo;
            syncCardInfo.cardKey = param.cardList[i].cardKey;
            syncCardInfo.cardName = param.cardList[i].cardName;
            syncCardInfo.hp = param.cardList[i].hp;
            syncCardInfo.defence = param.cardList[i].defence;
            syncCardInfo.agility = param.cardList[i].agility;
            syncCardInfo.tags = param.cardList[i].tags;
            eventData->cardList.Add(syncCardInfo);
        }
        eventData->handCardList = param.handCardList;
        
        KBENGINE_EVENT_FIRE("onSyncPlayerBattleInfo", eventData);
    }

    void Avatar::onStopCardSelection()
    {

    }

    void Avatar::onSyncChangeHandCardSuccess(uint8 changeCardNb, const FString& changeHandCardKey, const FString& pileCardKey)
    {
        UKBEventData_onSyncChangeHandCardSuccess* eventData = NewObject<UKBEventData_onSyncChangeHandCardSuccess>();
        eventData->changeSelectCardNb = changeCardNb;
        eventData->changeHandCardKey = changeHandCardKey;
        eventData->changePileCardKey = pileCardKey;
        KBENGINE_EVENT_FIRE("onSyncChangeHandCardSuccess", eventData);
    }

    void Avatar::onSyncExhaustCardReplacement()
    {
        UKBEventData* eventData = NewObject<UKBEventData>();
        KBENGINE_EVENT_FIRE("onSyncExhaustCardReplacement", eventData);
    }

    void Avatar::onSyncUpdateSelectedCards(uint8 changeNb, const SYNC_PLAYER_BATTLE_INFO& allCardInfos)
    {
        UKBEventData_onSyncUpdateSelectedCards* eventData = NewObject<UKBEventData_onSyncUpdateSelectedCards>();
        eventData->changeNb = changeNb;
        for (int32 i = 0; i < allCardInfos.cardList.Num(); i++)
        {
            FSYNC_CARD_INFO syncCardInfo;
            syncCardInfo.cardKey = allCardInfos.cardList[i].cardKey;
            syncCardInfo.cardName = allCardInfos.cardList[i].cardName;
            syncCardInfo.hp = allCardInfos.cardList[i].hp;
            syncCardInfo.defence = allCardInfos.cardList[i].defence;
            syncCardInfo.agility = allCardInfos.cardList[i].agility;
            syncCardInfo.tags = allCardInfos.cardList[i].tags;
            eventData->cardList.Add(syncCardInfo);
        }
        eventData->handCardList = allCardInfos.handCardList;
        KBENGINE_EVENT_FIRE("onSyncUpdateSelectedCards", eventData);
    }

    void Avatar::onSyncRoomStartBattle()
    {
        UKBEventData* eventData = NewObject<UKBEventData>();
        KBENGINE_EVENT_FIRE("onSyncRoomStartBattle", eventData);
    }

    void Avatar::resumeBattle()
    {

    }

    void Avatar::startBattle()
    {

    }

    void Avatar::switchController(uint8 playerNb)
    {

    }

    void Avatar::syncTimeInterval(const SYNC_BATTLE_TIME_INFO& param)
    {

    }

    void Avatar::ReqChangeSelectCard(FString changeCardKey)
    {
        pBaseEntityCall->reqChangeSelectCard(changeCardKey);
    }

    void Avatar::ReqUpdateSelectedCard()
    {
        pBaseEntityCall->reqUpdateSelectedCard();
    }

}