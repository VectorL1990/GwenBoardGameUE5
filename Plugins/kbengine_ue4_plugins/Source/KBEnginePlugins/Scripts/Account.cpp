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

        KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqEnterRoom", "ReqEnterRoom", [this](const UKBEventData* pEventData)
            {
                ReqEnterRoom();
            });

        KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqMatch", "ReqMatch", [this](const UKBEventData* pEventData)
            {
                ReqMatch();
            });

        KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqModifyCardGroup", "ReqModifyCardGroup", [this](const UKBEventData* pEventData)
            {
                const UKBEventData_ReqModifyCardGroup& data = static_cast<const UKBEventData_ReqModifyCardGroup&>(*pEventData);
                ReqModifyCardGroup(data.groupNb, data.cardList);
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
        GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Cyan, "Account reqTest message has already sent to server");
        pBaseEntityCall->reqTest(param);
    }

    void Account::ReqEnterRoom()
    {
        pBaseEntityCall->reqEnterRoom();
    }

    void Account::ReqMatch()
    {
        pBaseEntityCall->reqMatch();
    }

    void Account::ReqModifyCardGroup(int32 groupNb, TArray<FString> cardList)
    {
        CARD_GROUP cardGroup;
        cardGroup.stringList = cardList;
        pBaseEntityCall->reqModifyCardGroup(groupNb, cardGroup);
    }

    void Account::onAccountClientEnabled(const PLAYER_PERSIST_INFO& arg1)
    {
        UKBEventData_ReceivePlayerPersistInfo* eventData = NewObject<UKBEventData_ReceivePlayerPersistInfo>();
        for (int32 i = 0; i < arg1.persistCardList.Num(); i++)
        {
            FCardGroup cardGroup;
            cardGroup.cardList = arg1.persistCardList[i].stringList;
            eventData->cardGroups.Add(cardGroup);
        }
        
        KBENGINE_EVENT_FIRE("ReceivePlayerPersistInfo", eventData);
    }

    void Account::onReqTest(int32 param)
    {
        FString paramStr = FString::FromInt(param);
        GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, paramStr);
    }

    void Account::onSyncRoomCreated(uint64 roomKey)
    {
        UKBEventData_onSyncRoomCreated* pEventData = NewObject<UKBEventData_onSyncRoomCreated>();
        pEventData->sRoomKey = FString::Printf(TEXT("%lld"), roomKey);
        KBENGINE_EVENT_FIRE("onSyncRoomCreated", pEventData);
    }
}