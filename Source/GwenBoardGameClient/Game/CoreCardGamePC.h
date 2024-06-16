// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicPlayerController.h"
#include "CoreCardGameManager.h"
#include "UI/BattleWidget.h"
#include "CoreCardGamePC.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGamePC : public ABasicPlayerController
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void DealHover();

    UFUNCTION(BlueprintCallable)
    void DealLeftClick();

    void InitSelectCardCamera();

    void ShowBattleWidget();

    void ReceiveFinishCardSelection();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    ACoreCardGameManager* coreCardGameManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> battleWidgetBPClass;

    UPROPERTY()
    UBattleWidget* battleWidget;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    
};
