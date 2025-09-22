// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "../CoreGameBlueprintFunctionLibrary.h"
#include "CardDetailWidget.h"
#include "Components/Image.h"
#include "BattleWidget.generated.h"

/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UBattleWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void Init(UCardDetailWidget* inCardDetailWidget,
        UImage* inCountDownBar);

    UFUNCTION(BlueprintNativeEvent)
    void NotifyInit();

    void SetupCardDetail(FVector cardWorldPose, FString cardName);

    void HideCardDetail();

    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    void SetFinishCardSelectionText();

    void UpdateCountDownBarProgress(float progress);

    void UpdateEndRoundButtonState(EEndRoundButtonState state);

    UPROPERTY(EditDefaultsOnly)
    UMaterialInterface* actionCountDownBarMatParent;

    UPROPERTY()
    UCardDetailWidget* cardDetailWidget;

    UPROPERTY(BlueprintReadWrite)
    UButton* finishSelectCardButton;

    UPROPERTY(BlueprintReadWrite)
    UTextBlock* finishCardSelectionText;

    UPROPERTY(BlueprintReadWrite)
    UImage* actionCountDownBar;
};
