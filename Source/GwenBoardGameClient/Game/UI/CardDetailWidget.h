// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCardDetailWidget : public UUserWidget
{
GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent)
    void TriggerShowWidget();
};
