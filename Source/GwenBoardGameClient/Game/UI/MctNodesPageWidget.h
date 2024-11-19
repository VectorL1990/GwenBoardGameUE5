// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
#include "MctNodesPageWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctNodesPageWidget : public UUserWidget
{
GENERATED_BODY()
public:

    UPROPERTY()
    UScrollBox* scrollBox;

    UFUNCTION(BlueprintCallable)
    void Init(UScrollBox* inScrollBox);
	
};
