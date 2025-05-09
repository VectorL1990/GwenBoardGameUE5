// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
#include "TreeView.h"
#include "MctNodeButton.h"
#include "../AI/MctsTreeNode.h"
#include "MctReplayMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctReplayMenuWidget : public UUserWidget
{
GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMctsTreeNode> mctsTreeNodeBPClass;

    UMctsTreeNode* rootNode;

    UTreeView* treeView;

    UFUNCTION(BlueprintNativeEvent)
    void NotifyInit();

    UFUNCTION(BlueprintCallable)
    void Init(UTreeView* inTreeView);

    UFUNCTION(BlueprintCallable)
    void RefreshMctsMenu();
};
