// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/CardWidget.h"
#include "BasicPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ABasicPlayerController : public APlayerController
{
GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FSlateColor buttonNormalColor;

    UPROPERTY(EditDefaultsOnly)
    FSlateColor buttonHoverColor;

    UPROPERTY(EditDefaultsOnly)
    FSlateColor buttonPressColor;

    UPROPERTY(EditDefaultsOnly)
    float hoverAmplify = 1.1;

    virtual void ShowCardDetail(UCardWidget* cardWidget);
};
