// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine/KBECommon.h"

#include "Engine/AvatarBase.h"

namespace KBEngine
{

    class Avatar : public AvatarBase
    {
    public:
        Avatar();
        virtual ~Avatar();

        virtual void __init__() override;

    public:
        virtual void onDestroy() override;
        virtual void onEnterWorld() override;
        virtual void onLeaveWorld() override;
        virtual void onEnterSpace() override;
        virtual void onLeaveSpace() override;

    public:
        virtual void onStopCardSelection() override;

        virtual void resumeBattle() override;

        virtual void startBattle() override;

        virtual void switchController(uint8 controllerNb) override;

        virtual void syncTimeInterval(const SYNC_BATTLE_TIME_INFO& param) override;
    };

}
