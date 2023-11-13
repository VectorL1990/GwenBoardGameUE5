// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine/KBECommon.h"

#include "Engine/AccountBase.h"

namespace KBEngine
{

    class Account : public AccountBase
    {
    public:
        Account();
        virtual ~Account();

    public:
        virtual void __init__() override;
        virtual void onDestroy() override;

        void reqTest(int32 param);
        void ReqEnterRoom();
        void ReqMatch();

        virtual void onReqTest(int32 param) override;
        virtual void onSyncRoomCreated(uint64 roomKey) override;
    };

}
