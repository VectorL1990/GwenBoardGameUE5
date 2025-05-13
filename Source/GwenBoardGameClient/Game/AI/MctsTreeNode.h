// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GlobalConstFunctionLibrary.h"
#include "MctsTreeNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GWENBOARDGAMECLIENT_API UMctsTreeNode : public UObject
{
    GENERATED_BODY()
public:
    
    void Init(UMctsTreeNode* inParent, int32 inActionId, float inP, int32 inHirachy);

    float GetValue();

    // Expand executes after selecting max U + Q leaf node
    TArray<UMctsTreeNode*> Expand(int32 parentHirachy, TMap<int32, float> actionProbs);

    UMctsTreeNode* ExpandNode(int32 parentHirachy, 
        int32 actionId, 
        float prob, 
        const TArray<FBoardRow>& boardRows,
        const TMap<int32, FInstanceCardInfo> inAllInstanceCardInfos);

    UMctsTreeNode* Select(int32& outAction);

    void UpdateEvaluateQValue(float inQ);

    void UpdateParentQValue(float leafQ);

    void UpdateCurNodeQValue(float leafQ);
	
    bool IsLeaf();

    bool IsRoot();

    void ResetNode();

    UFUNCTION(BlueprintCallable)
    TArray<UMctsTreeNode*> ConvertChildrenToList();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMctsTreeNode> mctsTreeNodeBPClass;

    UPROPERTY()
    UMctsTreeNode* parent;

    UPROPERTY()
    TMap<int32, UMctsTreeNode*> children;

    UPROPERTY(BlueprintReadWrite)
    int32 actionId = -1;

    UPROPERTY()
    TArray<FBoardRow> replayBoardRows;

    UPROPERTY()
    TMap<int32, FInstanceCardInfo> allReplayInstanceCardInfo;

    int32 visit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float p;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float q;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float u;

    static float cPuct;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 hirachy;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 step;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 simulationNb;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> stateStrings;
};
