// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "BattleCardWidget.h"
#include "SelectCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API USelectCardWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    UPROPERTY(EditDefaultsOnly)
    TArray<UBattleCardWidget*> initSeletableCardWidgetList;
};
