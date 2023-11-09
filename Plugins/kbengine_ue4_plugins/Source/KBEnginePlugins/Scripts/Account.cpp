#include "Account.h"
#include "Engine/Entity.h"
#include "Engine/KBEngine.h"
#include "Engine/KBDebug.h"
#include "LogicEvents.h"
#include "BattleEvents.h"

namespace KBEngine
{

    Account::Account() : AccountBase()
    {
    }

    Account::~Account()
    {
    }

    void Account::__init__()
    {
        KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("reqTest", "reqTest", [this](const UKBEventData* pEventData)
            {
                const UKBEventData_ReqTest& data = static_cast<const UKBEventData_ReqTest&>(*pEventData);
                reqTest(data.testParam);
            });

        UKBEventData_onLoginSuccessfully* pEventData = NewObject<UKBEventData_onLoginSuccessfully>();
        pEventData->entity_uuid = KBEngineApp::getSingleton().entity_uuid();
        pEventData->entity_id = id();
        KBENGINE_EVENT_FIRE("onLoginSuccessfully", pEventData);
    }

    void Account::onDestroy()
    {
        // 注销注册的所有事件
        KBENGINE_DEREGISTER_ALL_EVENT();
    }

    void Account::reqTest(int32 param)
    {
        pBaseEntityCall->reqTest(param);
    }

    void Account::onReqTest(int32 param)
    {
        FString paramStr = FString::FromInt(param);
        GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, paramStr);
    }
}