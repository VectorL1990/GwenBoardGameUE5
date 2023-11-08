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
        void reqEnterRoom();

        virtual void onCreateAvatarResult(uint8 arg1) override;
        virtual void onRemoveAvatar(uint64 arg1) override;
        virtual void onReqAvatarList() override;
        virtual void onReqTest(int32 param) override;

    };

}
