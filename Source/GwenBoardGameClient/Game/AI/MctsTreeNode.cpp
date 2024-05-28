// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/MctsTreeNode.h"

void UMctsTreeNode::Init(UMctsTreeNode* inParent, float inP)
{
				parent = inParent;
				visit = 0;
				p = inP;
				q = 0.0;
				u = 0.0;
}

void UMctsTreeNode::GetValue()
{
				u = UMctsTreeNode::cPuct * 
}

void UMctsTreeNode::Expand(TMap<int, float> actionProbs)
{
				for (TMap<int, float>::TConstIterator iter = actionProbs.CreateConstIterator(); iter; ++iter)
				{
								if (!children.Contains(iter->Key))
								{
												UMctsTreeNode* child = NewObject<UMctsTreeNode>();
												child->Init(this, iter->Value);
												children.Add(iter->Key, child);
								}
				}
}

void UMctsTreeNode::Select(int& outAction, UMctsTreeNode* outNode)
{
				float maxQU = 0.0;
				int maxQUAction = 0;
				UMctsTreeNode* maxQUNode;
				for (TMap<int, UMctsTreeNode*>::TConstIterator iter = children.CreateConstIterator(); iter; ++iter)
				{
								float nodeQU = iter->Value->GetValue();
								if (nodeQU >= maxQU)
								{
												maxQU = nodeQU;
												maxQUAction = iter->Key;
												maxQUNode = iter->Value;
								}
				}
				outAction = maxQUAction;
				outNode = maxQUNode;
}

void UMctsTreeNode::UpdateCurNode(float inQ)
{
				UpdateRecursive(inQ);
				visit += 1;
				q = inQ;
}

void UMctsTreeNode::UpdateRecursive(float inQ)
{
				if (parent)
				{
								parent->UpdateRecursive(inQ);
				}
				Update(inQ);
}

void UMctsTreeNode::Update(float leafQ)
{
				visit += 1;
				q += (q - leafQ)/(float)visit;
}

