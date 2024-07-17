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

float UMctsTreeNode::GetValue()
{
				u = UMctsTreeNode::cPuct * p * FMath::Sqrt((float)parent->visit) / (1.0 + (float)parent->visit);
				return u;
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
}

void UMctsTreeNode::UpdateEvaluateQValue(float inQ)
{
				evaluateQ = inQ;
				q = inQ;
				visit += 1;
				UpdateParentQValue(inQ);
}

void UMctsTreeNode::UpdateParentQValue(float leafQ)
{
				if (parent)
				{
								parent->UpdateParentQValue(leafQ);
				}
				UpdateCurNodeQValue(leafQ);
}

void UMctsTreeNode::UpdateCurNodeQValue(float leafQ)
{
				visit += 1;
				q = (q - leafQ) / visit;
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


