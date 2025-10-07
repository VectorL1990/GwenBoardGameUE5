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
	UMctsTreeNode* rootNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	FString rootUid = coreCardGameMode->aiRunnable->mcts->finishSelfPlayDemoTree.rootUid;
	rootNode->CopyFromMctNodeStruct(coreCardGameMode->aiRunnable->mcts->finishSelfPlayDemoTree.allNodes[rootUid]);
	rootNode->ConstructMctsTreeFromStruct(coreCardGameMode->aiRunnable->mcts->finishSelfPlayDemoTree, rootUid);
	treeView->AddItem(rootNode);
	
}

void UMctReplayMenuWidget::NextMctsNodeMenu()
{
	treeView->ClearListItems();

	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
	curShowMctsNodeNb += 1;
	if (curShowMctsNodeNb >= coreCardGameMode->aiRunnable->mcts->finishSelfPlayGameTrees.Num())
	{
		curShowMctsNodeNb = 0;
	}
	UMctsTreeNode* rootNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	FString rootUid = coreCardGameMode->aiRunnable->mcts->finishSelfPlayGameTrees[curShowMctsNodeNb].rootUid;
	rootNode->CopyFromMctNodeStruct(coreCardGameMode->aiRunnable->mcts->finishSelfPlayGameTrees[curShowMctsNodeNb].allNodes[rootUid]);
	rootNode->ConstructMctsTreeFromStruct(coreCardGameMode->aiRunnable->mcts->finishSelfPlayGameTrees[curShowMctsNodeNb], rootUid);
	treeView->AddItem(rootNode);
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

void UMctReplayMenuWidget::NotifyShowMenu_Implementation()
{

}

