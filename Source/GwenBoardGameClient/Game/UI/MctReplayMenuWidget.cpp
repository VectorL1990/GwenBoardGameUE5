// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MctReplayMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MctSimulateButton.h"
#include "../CoreCardGameModeBase.h"

void UMctReplayMenuWidget::NotifyInit_Implementation()
{

}

void UMctReplayMenuWidget::Init(UTreeView* inTreeView, UScrollBox* inScrollBox)
{
	treeView = inTreeView;
	scrollBox = inScrollBox;
}

void UMctReplayMenuWidget::RefreshMctsMenu()
{
	treeView->ClearListItems();

	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
	rootNode = coreCardGameMode->aiRunnable->mcts->finishSelfPlayGameTreeRoots[0];
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

void UMctReplayMenuWidget::UpdateWHistories(const TArray<int32> wActionIds, 
	const TArray<uint8> wLaunchSections, 
	const TArray<float> wHistories,
	const TArray<float> quHistories)
{
	for (int32 i = 0; i < mctWElements.Num(); i++)
	{
		mctWElements[i]->ConditionalBeginDestroy();
	}
	scrollBox->ClearChildren();
	mctWElements.Empty();

	for (int32 i = 0; i < wHistories.Num(); i++)
	{
		UMctWValueElement* mctWValueElement = NewObject<UMctWValueElement>(GetWorld(), mctWElementBPClass);
		mctWElements.Add(mctWValueElement);
		scrollBox->AddChild(mctWValueElement);
		mctWValueElement->NotifySetText(wActionIds[i], wLaunchSections[i], wHistories[i], quHistories[i]);
	}
}

