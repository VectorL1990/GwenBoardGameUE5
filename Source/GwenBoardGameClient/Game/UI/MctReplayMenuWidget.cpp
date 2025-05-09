// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MctReplayMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MctSimulateButton.h"
#include "../CoreCardGameModeBase.h"

void UMctReplayMenuWidget::NotifyInit_Implementation()
{

}

void UMctReplayMenuWidget::Init(UTreeView* inTreeView)
{
	treeView = inTreeView;
}

void UMctReplayMenuWidget::RefreshMctsMenu()
{
	treeView->ClearListItems();

	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
	rootNode = coreCardGameMode->aiRunnable->mcts->veryFirstNode;
	treeView->AddItem(rootNode);
	/*
	UMctsTreeNode* testParent = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	testParent->hirachy = 0;

	
	UMctsTreeNode* childNode_1 = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	testParent->children.Add(1, childNode_1);
	childNode_1->hirachy = 1;

	UMctsTreeNode* childNode_2 = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	childNode_1->children.Add(2, childNode_2);
	childNode_2->hirachy = 2;
	
	treeView->AddItem(testParent);
	*/
}


