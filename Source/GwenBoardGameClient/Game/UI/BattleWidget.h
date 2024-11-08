// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "../CoreGameBlueprintFunctionLibrary.h"
#include "CardDetailWidget.h"
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
    void Init(UCardDetailWidget* inCardDetailWidget);

    void SetupCardDetail(FVector cardWorldPose);

    void HideCardDetail();

    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    void SetFinishCardSelectionText();

    UPROPERTY()
    UCardDetailWidget* cardDetailWidget;

    UPROPERTY(BlueprintReadWrite)
    UButton* finishSelectCardButton;

    UPROPERTY(BlueprintReadWrite)
    UTextBlock* finishCardSelectionText;
};
