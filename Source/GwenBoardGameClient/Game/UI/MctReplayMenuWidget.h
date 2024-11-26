// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
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
    UMctsTreeNode* curParentNode;

    UPROPERTY()
    TArray<UScrollBox*> scrollBoxes;

    UScrollBox* mctNodesScrollBox;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> mctNodeButtonBPClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> mctSimulationButtonBPClass;

    UFUNCTION(BlueprintCallable)
    void Init(UScrollBox* inMctNodesScrollBox);

    UFUNCTION(BlueprintCallable)
    void AddScrollBox(UScrollBox* scrollBox);

    void RefreshSimulationButton(UMctsTreeNode* rootNode, int32 simulationNb);

    void GetInMctsNodesWidget(UMctsTreeNode* inParentNode);

    void GetOutMctsNodesWidget();
};
