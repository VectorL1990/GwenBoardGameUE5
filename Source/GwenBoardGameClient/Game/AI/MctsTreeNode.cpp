// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/MctsTreeNode.h"

void UMctsTreeNode::Init(UMctsTreeNode* inParent, int32 inActionId, ActionType inActionType, float inP, int32 inHirachy)
{
	parent = inParent;
	actionId = inActionId;
	actionType = inActionType;
	visit = 0;
	p = inP;
	q = 0.0;
	w = 0.0;
	u = 0.0;
	hirachy = inHirachy;
	sectionZeroScore = 0;
	sectionOneScore = 0;
}

float UMctsTreeNode::GetValue(bool isTraining)
{
	/*
	if (isTraining)
	{
		if (visit == 0)
		{
			return FLT_MAX;
		}
		else
		{
			u = 5.0 * p * FMath::Sqrt((float)parent->visit) / (1.0 + (float)visit);
			return u + q;
		}
	}
	else
	{
		u = 5.0 * p * FMath::Sqrt((float)parent->visit) / (1.0 + (float)visit);
		return u + q;
	}
	*/
	
	u = 5.0 * p * FMath::Sqrt((float)parent->visit) / (1.0 + (float)visit);
	return u + q;
	//return 0;
}

UMctsTreeNode* UMctsTreeNode::ExpandNode(int32 parentHirachy, 
	int32 actionId, 
	ActionType actionType,
	uint8 curPlayingSectionNb,
	float prob, 
	const TArray<FBoardRow>& inBoardRows, 
	const TMap<int32, FInstanceCardInfo> inAllInstanceCardInfos)
{
	if (!children.Contains(actionId))
	{
		UMctsTreeNode* child = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
		child->Init(this, actionId, actionType, prob, hirachy + 1);
		child->curPlayingSectionNb = curPlayingSectionNb;
		child->replayBoardRows = inBoardRows;
		child->allReplayInstanceCardInfo = inAllInstanceCardInfos;
		children.Add(actionId, child);
		return child;
	}
	return NULL;
}

UMctsTreeNode* UMctsTreeNode::Select(int32& outAction, bool isTraining)
{
	float maxQU = -std::numeric_limits<float>::max();
	int32 maxQUAction = 0;
	UMctsTreeNode* outNode = NULL;
	for (TMap<int, UMctsTreeNode*>::TConstIterator iter = children.CreateConstIterator(); iter; ++iter)
	{
		float nodeQU = iter->Value->GetValue(isTraining);
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

void UMctsTreeNode::UpdateQValueRecursive(int32 originActionId, int32 originLaunchSection, int32 originHirachy, float leafW, bool isTraining)
{
	if (parent)
	{
		if (parent->curPlayingSectionNb != curPlayingSectionNb)
		{
			parent->UpdateQValueRecursive(originActionId, originLaunchSection, originHirachy, -leafW, isTraining);
		}
		else
		{
			parent->UpdateQValueRecursive(originActionId, originLaunchSection, originHirachy, leafW, isTraining);
		}
	}

	visit += 1;
	if (originHirachy <= hirachy)
	{
		truncatedVisit += 1;
	}
	
	updateWActionIds.Add(originActionId);
	updateWLaunchSections.Add(originLaunchSection);
	updateWHistories.Add(leafW);
	//w = w + leafW;
	//q = w / (float)visit;
	q += (leafW - q) / (float)visit;
	if (!parent)
	{
		updateQUHistories.Add(0.0);
	}
	else
	{
		updateQUHistories.Add(GetValue(isTraining));
	}
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

void UMctsTreeNode::DeleteChildren()
{

}

void UMctsTreeNode::ResetNode()
{
	parent = NULL;
	actionId = -1;
	visit = 0;
	truncatedVisit = 0;
	p = 1.0;
	q = 0.0;
	u = 0.0;
	hirachy = 0;
	sectionZeroScore = 0;
	sectionOneScore = 0;
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

void UMctsTreeNode::UpdateWinLoseResult(int32 winSection)
{
	if (winSection == -1)
	{
		// which means draw
		winLoseResult = 0.0;
	}
	else
	{
		if (curPlayingSectionNb == winSection)
		{
			winLoseResult = 1.0;
		}
		else
		{
			winLoseResult = -1.0;
		}
	}

	if (children.Num() > 0)
	{
		for (TMap<int, UMctsTreeNode*>::TConstIterator iter = children.CreateConstIterator(); iter; ++iter)
		{
			iter->Value->UpdateWinLoseResult(winSection);
		}
	}
}

void UMctsTreeNode::ConstructMctsTreeFromStruct(const FMctsNodeTree& treeStruct, FString curTreeNodeUid)
{
	if (treeStruct.allNodes.Contains(curTreeNodeUid))
	{
		for (int32 i = 0; i < treeStruct.allNodes[curTreeNodeUid].childrenUids.Num(); i++)
		{
			UMctsTreeNode* child = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
			child->CopyFromMctNodeStruct(treeStruct.allNodes[treeStruct.allNodes[curTreeNodeUid].childrenUids[i]]);
			child->parent = this;
			children.Add(child->actionId, child);
			child->ConstructMctsTreeFromStruct(treeStruct, treeStruct.allNodes[curTreeNodeUid].childrenUids[i]);
		}
	}
}

void UMctsTreeNode::CopyFromMctNodeStruct(const FMctsNode& nodeStruct)
{
	actionId = nodeStruct.actionId;
	actionType = nodeStruct.actionType;
	visit = nodeStruct.visit;
	truncatedVisit = nodeStruct.truncatedVisit;
	p = nodeStruct.p;
	q = nodeStruct.q;
	u = nodeStruct.u;
	hirachy = nodeStruct.hirachy;
	sectionZeroScore = nodeStruct.sectionZeroScore;
	sectionOneScore = nodeStruct.sectionOneScore;
	replayBoardRows = nodeStruct.replayBoardRows;
	allReplayInstanceCardInfo = nodeStruct.allReplayInstanceCardInfo;
}


