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
    
    void Init(UMctsTreeNode* inParent, float inP);

    float GetValue();

    // Expand executes after selecting max U + Q leaf node
    void Expand(TMap<int32, float> actionProbs);

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

    int32 visit;

    float p;

    float evaluateQ;

    float q;

    float u;

    static float cPuct;
};
