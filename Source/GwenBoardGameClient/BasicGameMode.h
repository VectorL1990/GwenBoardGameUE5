// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game/UI/LoginWidget.h"
#include "Game/GlobalConstFunctionLibrary.h"
#include "BasicGameMode.generated.h"

/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ABasicGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    UPROPERTY()
    TMap<FString, FCardInfo> allCardInfos;

    //virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void CallGameInstanceSendData(FString sendData);

    virtual void InitEvents();

    UFUNCTION()
    void onKicked(const UKBEventData* eventData);

    void onDisconnected(const UKBEventData* eventData);

    UFUNCTION(BlueprintCallable)
    void InitKBEMain();

    UFUNCTION()
    void ReqCreateAccount(FString playerName, FString pwd);

    UFUNCTION()
    void ReqLogin(FString playerName, FString pwd);

    UFUNCTION()
    void ReqMatch();

    UFUNCTION()
    void ReqEnterRoom();

    void ReceivePlayerPersistInfo(const UKBEventData* eventData);

    UFUNCTION()
    void onCreateAccountResult(const UKBEventData* eventData);

    void onSyncRoomCreated(const UKBEventData* eventData);

    virtual void onSyncPlayerBattleInfo(const UKBEventData* eventData);

    UFUNCTION(BlueprintCallable)
    void SetCardInfo(FString cardName,
            FString aliasCN,
            FString aliasEN,
            FString cardFunctionCN,
            FString cardFunctionEN,
            FString despCN,
            FString despEN,
            int32 hp,
            int32 defence,
            int32 agility,
            TArray<FString> cardTags,
            FString launchType,
            int32 coolDown,
            int32 availableTimes,
            FString launchGeoType,
            FString autoSkillTargetGeoType,
            FString targetGeoType,
            FString aoeType,
            FString targetCamp,
            FString effectType,
            FString effectAffix,
            FString effectAffixCamp,
            FString prereqTagCondition,
            FString prereqTag,
            FString prereqCampType,
            FString prereqType,
            FString passivePrereqType,
            TArray<int32> values,
            UTexture* texture,
            FString moveType,
            FString cardCategory);

    virtual void SpawnSelectCard();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> kbeMainClass;

    UPROPERTY()
    AActor* kbeMainActor;

    UPROPERTY()
    FString sRoomKey;
};
