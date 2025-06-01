// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/MctsTreeNode.h"

void UMctsTreeNode::Init(UMctsTreeNode* inParent, int32 inActionId, float inP, int32 inHirachy)
{
	parent = inParent;
	actionId = inActionId;
	visit = 0;
	p = inP;
	q = 0.0;
	w = 0.0;
	u = 0.0;
	hirachy = inHirachy;
}

float UMctsTreeNode::GetValue()
{
	u = 5.0 * p * FMath::Sqrt((float)parent->visit) / (1.0 + (float)visit);
	return u;
	//return 0;
}

TArray<UMctsTreeNode*> UMctsTreeNode::Expand(int32 parentHirachy, TMap<int32, float> actionProbs)
{
	TArray<UMctsTreeNode*> newNodes;
	for (TMap<int32, float>::TConstIterator iter = actionProbs.CreateConstIterator(); iter; ++iter)
	{
		if (!children.Contains(iter->Key))
		{
			UMctsTreeNode* child = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
			child->Init(this, iter->Key, iter->Value, hirachy + 1);
			children.Add(iter->Key, child);
			newNodes.Add(child);
		}
	}
	return newNodes;
}

UMctsTreeNode* UMctsTreeNode::ExpandNode(int32 parentHirachy, 
	int32 actionId, 
	float prob, 
	const TArray<FBoardRow>& inBoardRows, 
	const TMap<int32, FInstanceCardInfo> inAllInstanceCardInfos)
{
	if (!children.Contains(actionId))
	{
		UMctsTreeNode* child = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
		child->Init(this, actionId, prob, hirachy + 1);
		child->replayBoardRows = inBoardRows;
		child->allReplayInstanceCardInfo = inAllInstanceCardInfos;
		children.Add(actionId, child);
		return child;
	}
	return NULL;
}

UMctsTreeNode* UMctsTreeNode::Select(int32& outAction)
{
	float maxQU = -std::numeric_limits<float>::max();
	int32 maxQUAction = 0;
	UMctsTreeNode* outNode = NULL;
	for (TMap<int, UMctsTreeNode*>::TConstIterator iter = children.CreateConstIterator(); iter; ++iter)
	{
		float nodeQU = iter->Value->GetValue();
		if (nodeQU >= maxQU)
		{
			maxQU = nodeQU;
			maxQUAction = iter->Key;
			outNode = iter->Value;
		}
	}
	outAction = maxQUAction;
	return outNode;
}

void UMctsTreeNode::UpdateQValueRecursive(float leafW)
{
	if (parent)
	{
		if (parent->curSectionNb != curSectionNb)
		{
			parent->UpdateQValueRecursive(-leafW);
		}
		else
		{
			parent->UpdateQValueRecursive(leafW);
		}
	}
	visit += 1;
	
	w = w + leafW;
	q = w / (float)visit;
}

bool UMctsTreeNode::IsLeaf()
{
	if (children.Num() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UMctsTreeNode::IsRoot()
{
	if (!parent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UMctsTreeNode::ResetNode()
{
	parent = NULL;
	actionId = -1;
	visit = 0;
	p = 1.0;
	q = 0.0;
	u = 0.0;
	hirachy = 0;
}

TArray<UMctsTreeNode*> UMctsTreeNode::ConvertChildrenToList()
{
	TArray<UMctsTreeNode*> childrenList;
	for (TMap<int, UMctsTreeNode*>::TConstIterator iter = children.CreateConstIterator(); iter; ++iter)
	{
		if (iter->Value)
		{
			childrenList.Add(iter->Value);
		}
	}
	return childrenList;
}


