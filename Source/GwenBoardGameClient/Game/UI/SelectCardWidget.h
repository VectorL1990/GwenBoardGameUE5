// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "BattleCardWidget.h"
#include "CardDetailWidget.h"
#include "CardWidget.h"
#include "SelectCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API USelectCardWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    
    TArray<UCardWidget*> selectedCards;

    UFUNCTION(BlueprintCallable)
    void Init(UCardDetailWidget* inCardDetailWidget);

    void RegisterSelectCard(UCardWidget* inCardWidget);


    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    UFUNCTION(BlueprintNativeEvent)
    void TriggerClose();

    UFUNCTION(BlueprintCallable)
    void GenerateSelectCards();

    UPROPERTY(EditDefaultsOnly)
    TArray<UBattleCardWidget*> initSeletableCardWidgetList;

    UPROPERTY(EditAnywhere)
    UCardDetailWidget* cardDetailWidget;
};
