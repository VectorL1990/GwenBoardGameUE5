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

        virtual void OnTeamData(uint8 arg1, const TArray<uint64>& arg2) {}
    public:
        virtual void onStopCardSelection() override;
        virtual void onSyncBattleResult(const STRING_LIST& losePlayerList) override;
        virtual void onSyncChangeHandCardSuccess(uint8 changeCardNb, const FString& changeHandCardKey, const FString& pileCardKey) override;
        virtual void onSyncExhaustCardReplacement() override;
        virtual void onSyncHeartBeat(int32 curBattleTick) override;
        virtual void onSyncLatestBattleState(const CORE_UPDATE_BATLLE_INFO& battleInfo) override;
        virtual void onSyncPlayerBattleInfo(const SYNC_PLAYER_BATTLE_INFO& playerInfo) override;
        virtual void onSyncResumeBattle(uint8 controllerNb) override;
        virtual void onSyncRoomStartBattle() override;
        virtual void onSyncSelectCardInterlude(const SYNC_PLAYER_BATTLE_INFO& playerInfo) override;
        virtual void onSyncSwitchController(uint8 controllerNb, uint64 avatarId) override;
        virtual void onSyncTimeInterval(const SYNC_BATTLE_TIME_INFO& syncTimeInfo) override;
        virtual void onSyncUpdateSelectedCards(uint8 changeNb, const SYNC_PLAYER_BATTLE_INFO& allCardInfos) override;

        void ReqChangeSelectCard(FString changeCardKey);
        void ReqFinishSelectCards();
        void ReqUpdateSelectedCard();
        void ReqSyncHeartBeat();
        void ReqLatestBattleInfo();
    };

}
