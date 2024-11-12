// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicPlayerController.h"
#include "CoreCardGameManager.h"
#include "Card.h"
#include "UI/BattleWidget.h"
#include "UI/CardWidget.h"
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

    void GetRaycastObj(FHitResult& outHitResult);

    UFUNCTION(BlueprintCallable)
    void DealHover();

    UFUNCTION(BlueprintCallable)
    void DealLeftClick();

    void InitSelectCardCamera();

    void ReceiveFinishCardSelection();

    void GetCursorScreenPose();

    void InitMenu();

    void RegisterSelectCardWidget(UCardWidget* cardWidget);

    void SwitchMenu(FString menuName);

    virtual void ShowCardDetail(UCardWidget* cardWidget) override;

    virtual void HideCardDetail(UCardWidget* cardWidget) override;

    void ShowCardDetailInBattle(ACard* card);

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
