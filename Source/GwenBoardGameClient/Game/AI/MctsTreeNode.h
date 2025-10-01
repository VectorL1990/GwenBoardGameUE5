// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GlobalConstFunctionLibrary.h"
#include <Game\CoreGameBlueprintFunctionLibrary.h>
#include "MctsTreeNode.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class GWENBOARDGAMECLIENT_API UMctsTreeNode : public UObject
{
    GENERATED_BODY()
public:
    
    void Init(UMctsTreeNode* inParent, int32 inActionId, ActionType actionType, float inP, int32 inHirachy);

    float GetValue(bool isTraining);

    // Expand executes after selecting max U + Q leaf node
    UMctsTreeNode* ExpandNode(int32 parentHirachy, 
        int32 actionId, 
        ActionType actionType,
        uint8 curPlayingSectionNb,
        float prob, 
        const TArray<FBoardRow>& boardRows,
        const TMap<int32, FInstanceCardInfo> inAllInstanceCardInfos);

    UMctsTreeNode* Select(int32& outAction, bool isTraining);

    void UpdateQValueRecursive(int32 originActionId, int32 originLaunchSection, int32 originHirachy, float leafW, bool isTraining);
	
    bool IsLeaf();

    bool IsRoot();

    void DeleteChildren();

    void ResetNode();

    UFUNCTION(BlueprintCallable)
    TArray<UMctsTreeNode*> ConvertChildrenToList();

    void UpdateWinLoseResult(int32 winSection);

    void ConstructMctsTreeFromStruct(const FMctsNodeTree& treeStruct, FString curTreeNodeUid);

    void CopyFromMctNodeStruct(const FMctsNode& nodeStruct);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMctsTreeNode> mctsTreeNodeBPClass;

    UPROPERTY()
    UMctsTreeNode* parent;

    UPROPERTY()
    TMap<int32, UMctsTreeNode*> children;

    UPROPERTY(BlueprintReadWrite)
    int32 actionId = -1;

    ActionType actionType = ActionType::EndRound;

    UPROPERTY()
    TArray<FBoardRow> replayBoardRows;

    UPROPERTY()
    TMap<int32, FInstanceCardInfo> allReplayInstanceCardInfo;

    UPROPERTY()
    int32 stateCoding[StateCodingTotalCHW] = { 0 };

    int32 visit;

    int32 truncatedVisit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float p;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float q;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float w;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float u;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 sectionZeroScore;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 sectionOneScore;

    int32 winLoseResult;

    static float cPuct;

    UPROPERTY()
    TArray<float> updateWHistories;

    UPROPERTY()
    TArray<float> updateQUHistories;

    UPROPERTY()
    TArray<int32> updateWActionIds;

    UPROPERTY()
    TArray<uint8> updateWLaunchSections;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 hirachy;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    uint8 curPlayingSectionNb;
};
