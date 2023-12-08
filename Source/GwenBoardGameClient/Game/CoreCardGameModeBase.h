// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "../BasicGameMode.h"
#include "BoardGrid.h"
#include "CoreCardGameModeBase.generated.h"

/**
 *
 */


UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGameModeBase : public ABasicGameMode
{
    GENERATED_BODY()
public:

    virtual void BeginPlay() override;

    virtual void InitEvents() override;

    virtual void Tick(float deltaTime) override;

    UFUNCTION(BlueprintNativeEvent)
    void InitDone();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACard> cardBPClass;

    UPROPERTY(EditDefaultsOnly)
    FVector initSpawnCardLoc;

    UPROPERTY(EditDefaultsOnly)
    FVector selectCardDemoLoc;

    UPROPERTY(EditDefaultsOnly)
    float selectCardInterval;

    UPROPERTY(EditDefaultsOnly)
    int32 maxSelectCardNb;

    UPROPERTY(EditDefaultsOnly)
    int32 availableSwitchCardNb;

    UPROPERTY(EditAnywhere)
    TArray<FVector> selectCardSpawnPts;


private:
    // --- Account req functions
    void ReqEnterRoom();

    void ReqPlayCard(int32 targetGridNb, int32 playCardUid);

    // --- Avatar req functions
    void ReqChangeSelectCard(FString changeCardKey);

    void ReqFinishSelectCards();

    void ReqUpdateSelectedCard();

    void ReqSyncHeartBeat();

    void ReqLatestBattleInfo();

    // --- Avatar sync functions
    void onReceiveNewTurnMessage(const UKBEventData* eventData);

    void onReceiveUpdateCoreGame(const UKBEventData* eventData);

    void onUpdateGridInfoList(const UKBEventData* eventData);

    void onStopCardSelection(const UKBEventData* eventData);

    void onSyncBattleResult(const UKBEventData* eventData);

    void onSyncChangeHandCardSuccess(const UKBEventData* eventData);

    void onSyncExhaustCardReplacement(const UKBEventData* eventData);

    void onSyncHeartBeat(const UKBEventData* eventData);

    void onSyncLatestBattleState(const UKBEventData* eventData);

    virtual void onSyncPlayerBattleInfo(const UKBEventData* eventData) override;

    void onSyncResumeBattle(const UKBEventData* eventData);

    void onSyncRoomStartBattle(const UKBEventData* eventData);

    void onSyncSelectCardInterlude(const UKBEventData* eventData);

    void onSyncSwitchController(const UKBEventData* eventData);

    void onSyncTimeInterval(const UKBEventData* eventData);

    void onSyncUpdateSelectedCards(const UKBEventData* eventData);


    virtual void InitPlayerBattleInfoDone(TArray<FString> cardList) override;

    BattleStatus curBattleStatus = BattleStatus::Default;

    TMap<FString, float> maxCoutingTicksMap;

    float curCountingTick = 0.0;

    int32 curReceiveUpdateId = 0;

    UPROPERTY()
        TArray<ABoardGrid*> boardGrids;

    UPROPERTY()
        TMap<int32, ACard*> occupiedGridCardMap;

    UPROPERTY()
    TMap<FString, FSYNC_CARD_INFO> allCardInfoMap;

    UPROPERTY()
    TArray<FString> handCardKeyList;

    UPROPERTY()
    TArray<FString> pileCardKeyList;

    UPROPERTY()
    TMap<FString, ACard*> handCardMap;

    UPROPERTY()
    TMap<FString, ACard*> allCardMap;

    uint8 maxChangeSelectCardNb = 3;
    uint8 curChangeSelectCardNb = 0;
};
