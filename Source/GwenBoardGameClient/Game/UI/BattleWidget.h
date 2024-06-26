// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "../CoreGameBlueprintFunctionLibrary.h"
#include "BattleWidget.generated.h"

/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UBattleWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetupCardDetail();

    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    void SetFinishCardSelectionText();

    UPROPERTY(BlueprintReadWrite)
    UButton* finishSelectCardButton;

    UPROPERTY(BlueprintReadWrite)
    UTextBlock* finishCardSelectionText;
};
