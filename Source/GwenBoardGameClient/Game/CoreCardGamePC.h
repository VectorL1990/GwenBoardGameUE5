// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicPlayerController.h"
#include "CoreCardGameManager.h"

#include "UI/BattleWidget.h"
#include "UI/SelectCardWidget.h"
#include "CoreCardGamePC.generated.h"

/**
 * 
 */


UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGamePC : public ABasicPlayerController
{
    GENERATED_BODY()
public:
    FString curMenuName;

    UFUNCTION(BlueprintCallable)
    void DealHover();

    UFUNCTION(BlueprintCallable)
    void DealLeftClick();

    void InitSelectCardCamera();

    void ReceiveFinishCardSelection();

    void InitMenu();

    void SwitchMenu(FString menuName);

    virtual void ShowCardDetail(UCardWidget* cardWidget) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    ACoreCardGameManager* coreCardGameManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> battleWidgetBPClass;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> selectCardWidgetBPClass;

    UPROPERTY()
    UBattleWidget* battleWidget;
    UPROPERTY()
    USelectCardWidget* selectCardWidget;
    

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    
};
