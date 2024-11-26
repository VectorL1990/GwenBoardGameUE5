// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MctReplayMenuWidget.h"
#include "MctSimulateButton.h"

void UMctReplayMenuWidget::Init(UScrollBox* inMctNodesScrollBox)
{
				mctNodesScrollBox = inMctNodesScrollBox;
}

void UMctReplayMenuWidget::AddScrollBox(UScrollBox* scrollBox)
{
				scrollBoxes.Add(scrollBox);
}

void UMctReplayMenuWidget::RefreshSimulationButton(UMctsTreeNode* rootNode, int32 simulationNb)
{
				UUserWidget* newSimulateButton = CreateWidget(this, mctSimulationButtonBPClass);
				
}

void UMctReplayMenuWidget::GetInMctsNodesWidget(UMctsTreeNode* inParentNode)
{
				TArray<UWidget*> curTreeNodeWidgets = mctNodesScrollBox->GetAllChildren();
				for (int32 i = 0; i < curTreeNodeWidgets.Num(); i++)
				{
								curTreeNodeWidgets[i]->ConditionalBeginDestroy();
				}
				mctNodesScrollBox->ClearChildren();

				for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = inParentNode->children.CreateConstIterator(); iter; ++iter)
				{
								UUserWidget* newNodeButton = CreateWidget(this, mctNodeButtonBPClass);
								UMctNodeButton* mctNodeButton = Cast<UMctNodeButton>(newNodeButton);
								mctNodeButton->Init(iter->Value);
								mctNodesScrollBox->AddChild(mctNodeButton);
				}

				curParentNode = inParentNode;
}

void UMctReplayMenuWidget::GetOutMctsNodesWidget()
{
				TArray<UWidget*> curTreeNodeWidgets = mctNodesScrollBox->GetAllChildren();
				for (int32 i = 0; i < curTreeNodeWidgets.Num(); i++)
				{
								curTreeNodeWidgets[i]->ConditionalBeginDestroy();
				}
				mctNodesScrollBox->ClearChildren();


				if (curParentNode->parent)
				{
								for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = curParentNode->parent->children.CreateConstIterator(); iter; ++iter)
								{
												UUserWidget* newNodeButton = CreateWidget(this, mctNodeButtonBPClass);
												UMctNodeButton* mctNodeButton = Cast<UMctNodeButton>(newNodeButton);
												mctNodeButton->Init(iter->Value);
												mctNodesScrollBox->AddChild(mctNodeButton);
								}
								curParentNode = curParentNode->parent;
				}
				else
				{
								UUserWidget* newNodeButton = CreateWidget(this, mctNodeButtonBPClass);
								UMctNodeButton* mctNodeButton = Cast<UMctNodeButton>(newNodeButton);
								mctNodeButton->Init(curParentNode);
								mctNodesScrollBox->AddChild(mctNodeButton);
								curParentNode = NULL;
				}
}

