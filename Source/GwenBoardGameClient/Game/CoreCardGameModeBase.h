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

private:
    //void ReceivePlayCardResponse();
    void ReqEnterRoom();

    void ReqPlayCard(int32 targetGridNb, int32 playCardUid);

    void onReceiveNewTurnMessage(const UKBEventData* eventData);

    void onReceiveUpdateCoreGame(const UKBEventData* eventData);

    void onUpdateGridInfoList(const UKBEventData* eventData);

    void onSyncPlayerBattleInfo(const UKBEventData* eventData);

    BattleStatus curBattleStatus = BattleStatus::Default;

    TMap<FString, float> maxCoutingTicksMap;

    float curCountingTick = 0.0;

    int32 curReceiveUpdateId = 0;

    UPROPERTY()
        TArray<ABoardGrid*> boardGrids;

    UPROPERTY()
        TMap<int32, ACard*> occupiedGridCardMap;

    UPROPERTY()
        TMap<int32, ACard*> allCards;
};
