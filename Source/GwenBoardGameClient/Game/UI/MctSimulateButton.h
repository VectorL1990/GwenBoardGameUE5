// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../AI/MctsTreeNode.h"
#include "MctSimulateButton.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctSimulateButton : public UUserWidget
{
				GENERATED_BODY()
public:
    UMctsTreeNode* rootTreeNode;


    void RecordMctsTreeNodeInfo(UMctsTreeNode* mctsTreeNode);

    UFUNCTION(BlueprintCallable)
    void ClickButton();
};
