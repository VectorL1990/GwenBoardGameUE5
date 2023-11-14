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
        eventData->cardList = param.cardList;
        KBENGINE_EVENT_FIRE("onSyncPlayerBattleInfo", eventData);
    }

    void Avatar::onStopCardSelection()
    {

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

}