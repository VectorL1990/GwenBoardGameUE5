// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine/KBECommon.h"

#include "Engine/PlayerBase.h"

namespace KBEngine
{

    class Player : public PlayerBase
    {
    public:
        Player();
        virtual ~Player();

        virtual void onUpdateBattle(const CORE_UPDATE_BATLLE_INFO& arg1) override;
        virtual void onUpdateGridInfoList(const UPDATE_GRID_INFO_LIST& arg1) override;
    };

}
