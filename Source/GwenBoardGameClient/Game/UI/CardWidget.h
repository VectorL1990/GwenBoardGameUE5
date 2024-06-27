// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CoreGameBlueprintFunctionLibrary.h"
#include "CardWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCardWidget : public UUserWidget
{
				GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    UFUNCTION(BlueprintCallable)
    void Hover();

    BattleCardWidgetType battleCardWidgetType;
};
