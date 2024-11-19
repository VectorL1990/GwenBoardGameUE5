// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MctsTreeNode.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctsTreeNode : public UObject
{
    GENERATED_BODY()
public:
    
    void Init(UMctsTreeNode* inParent, int32 inActionId, float inP, int32 inHirachy);

    float GetValue();

    // Expand executes after selecting max U + Q leaf node
    void Expand(int32 parentHirachy, TMap<int32, float> actionProbs);

    void Select(int32& outAction, UMctsTreeNode* outNode);

    void UpdateEvaluateQValue(float inQ);

    void UpdateParentQValue(float leafQ);

    void UpdateCurNodeQValue(float leafQ);
	
    bool IsLeaf();

    bool IsRoot();

    UPROPERTY()
    UMctsTreeNode* parent;

    UPROPERTY()
    TMap<int32, UMctsTreeNode*> children;

    int32 actionId = -1;

    int32 visit;

    float p;

    float evaluateQ;

    float q;

    float u;

    static float cPuct;

    int32 hirachy;

    int32 step;

    int32 simulationNb;

    TArray<FString> stateStrings;
};
