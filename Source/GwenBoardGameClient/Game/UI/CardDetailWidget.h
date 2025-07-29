// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CardDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCardDetailWidget : public UUserWidget
{
GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    UTextBlock* descriptionTextBlock;

    UFUNCTION(BlueprintNativeEvent)
    void NotifyInit();

    UFUNCTION(BlueprintNativeEvent)
    void TriggerShowWidget();

    void SetupDetailDescription(FString cardName);
};
