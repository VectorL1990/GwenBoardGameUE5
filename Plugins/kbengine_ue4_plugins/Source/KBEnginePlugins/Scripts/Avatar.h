﻿// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

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

        virtual void OnTeamData(uint8 arg1, const TArray<uint64>& arg2) {}
    public:
        virtual void onSyncBattleResult(const STRING_LIST& losePlayerList) override;
        virtual void onSyncChangeHandCardSuccess(uint8 changeCardNb, const FString& changeHandCardKey, const FString& pileCardKey) override;
        virtual void onSyncExhaustCardReplacement() override;
        virtual void onSyncHeartBeat(int32 curBattleTick) override;
        virtual void onSyncLatestBattleState(const CORE_UPDATE_BATLLE_INFO& battleInfo) override;
        virtual void onSyncLaunchSkillFailed(int32 curActionSequence, int32 clientActionSequence) override;
        virtual void onSyncPlayerBattleInfo(const SYNC_PLAYER_BATTLE_INFO& playerInfo) override;
        virtual void onSyncReceiveEnterRoom(uint8 result) override;
        virtual void onSyncReceiveFinishCardSelection() override;
        virtual void onSyncResumeBattle(int32 controllerNb) override;
        virtual void onSyncRoomStartBattle() override;
        virtual void onSyncSelectCardInterlude(const SYNC_PLAYER_BATTLE_INFO& playerInfo) override;
        virtual void onSyncSwitchController(int32 controllerNb, uint64 avatarId) override;
        virtual void onSyncTimeInterval(const SYNC_BATTLE_TIME_INFO& syncTimeInfo) override;
        virtual void onSyncUpdateActionInfo(const SYNC_MODIFICATION_INFO& modificationInfo) override;
        virtual void onSyncUpdateSelectedCards(uint8 changeNb, const SYNC_PLAYER_BATTLE_INFO& allCardInfos) override;

        void ReqChangeSelectCard(FString changeCardKey);
        void ReqFinishSelectCards();
        void ReqUpdateSelectedCard();
        void ReqSyncHeartBeat();
        void ReqLatestBattleInfo();
        void ReqEnterRoom();
    };

}
