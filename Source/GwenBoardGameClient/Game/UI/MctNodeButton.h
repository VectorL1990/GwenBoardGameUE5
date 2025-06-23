// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../AI/MctsTreeNode.h"
#include "Components/TextBlock.h"
#include "MctNodeButton.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctNodeButton : public UUserWidget
{
GENERATED_BODY()
public:
    UPROPERTY()
    UMctsTreeNode* mctsTreeNode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* QText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* PText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* WText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* UText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* actionTypeText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* actionIdText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* visitsText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* scoreZeroText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* scoreOneText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* curNodeWinLoseText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextBlock* QUText;

    UFUNCTION(BlueprintCallable)
    void Init(UMctsTreeNode* inMctsTreeNode);

    UFUNCTION(BlueprintCallable)
    void ClickButton();
};
