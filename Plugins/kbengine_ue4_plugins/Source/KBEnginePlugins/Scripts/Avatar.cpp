#include "Avatar.h"
#include "Engine/Entity.h"
#include "LogicEvents.h"

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
            KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("relive", "relive", [this](const UKBEventData* pEventData)
                {
                    const UKBEventData_reqRelive& data = static_cast<const UKBEventData_reqRelive&>(*pEventData);
                    reqRelive(data.reliveType);
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

    void Avatar::onStopCardSelection()
    {}

    void Avatar::resumeBattle()
    {}

    void Avatar::startBattle()
    {}

    void Avatar::switchController(uint8 controllerNb)
    {}

    void Avatar::syncTimeInterval(const SYNC_BATTLE_TIME_INFO& param)
    {}
}