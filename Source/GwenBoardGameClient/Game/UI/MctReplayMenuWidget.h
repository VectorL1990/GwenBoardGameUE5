// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
#include "TreeView.h"
#include "MctNodeButton.h"
#include "../AI/MctsTreeNode.h"
#include "MctWValueElement.h"
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

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMctWValueElement> mctWElementBPClass;

    UMctsTreeNode* rootNode;

    UTreeView* treeView;

    UScrollBox* scrollBox;

    UPROPERTY()
    TArray<UMctWValueElement*> mctWElements;

    UFUNCTION(BlueprintNativeEvent)
    void NotifyInit();

    UFUNCTION(BlueprintCallable)
    void Init(UTreeView* inTreeView, UScrollBox* inScrollBox);

    UFUNCTION(BlueprintCallable)
    void RefreshMctsMenu();

    void UpdateWHistories(const TArray<int32> wActionIds, 
        const TArray<uint8> wLaunchSections, 
        const TArray<float> wHistories,
        const TArray<float> quHistories);
};
