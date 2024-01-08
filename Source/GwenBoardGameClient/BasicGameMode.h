// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game/UI/LoginWidget.h"
#include "BasicGameMode.generated.h"

/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ABasicGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
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

    UFUNCTION()
    void onCreateAccountResult(const UKBEventData* eventData);

    void onSyncRoomCreated(const UKBEventData* eventData);

    virtual void onSyncPlayerBattleInfo(const UKBEventData* eventData);

    virtual void SpawnSelectCard(TArray<FString> cardList);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> kbeMainClass;

    UPROPERTY()
    AActor* kbeMainActor;

    UPROPERTY()
    FString sRoomKey;
};
