// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MctNodeButton.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGameModeBase.h"
#include "../CoreCardGamePC.h"

void UMctNodeButton::Init(UMctsTreeNode* inMctsTreeNode)
{
	mctsTreeNode = inMctsTreeNode;
	if (mctsTreeNode->actionType == ActionType::PlayCard)
	{
		actionTypeText->SetText(FText::FromString("PlayCard"));
	}
	else if (mctsTreeNode->actionType == ActionType::LaunchSkill)
	{
		actionTypeText->SetText(FText::FromString("LaunchSkill"));
	}
	else if (mctsTreeNode->actionType == ActionType::Move)
	{
		actionTypeText->SetText(FText::FromString("Move"));
	}
	else if (mctsTreeNode->actionType == ActionType::EndRound)
	{
		actionTypeText->SetText(FText::FromString("EndRound"));
	}

	double u = 0.0;
	if (mctsTreeNode->parent)
	{
		u = 5.0 * mctsTreeNode->p * FMath::Sqrt((float)mctsTreeNode->parent->visit) / (1.0 + (float)mctsTreeNode->visit);
	}
	//PText->SetText(FText::FromString(FString::SanitizeFloat(mctsTreeNode->p)));
	UText->SetText(FText::FromString(FString::SanitizeFloat(u)));
	//WText->SetText(FText::FromString(FString::SanitizeFloat(mctsTreeNode->w)));
	QText->SetText(FText::FromString(FString::SanitizeFloat(mctsTreeNode->q)));
	//actionIdText->SetText(FText::FromString(FString::FromInt(mctsTreeNode->actionId)));
	visitsText->SetText(FText::FromString(FString::FromInt(mctsTreeNode->visit)));
	//QUText->SetText(FText::FromString(FString::SanitizeFloat(mctsTreeNode->q + mctsTreeNode->u)));
	scoreZeroText->SetText(FText::FromString(FString::FromInt(mctsTreeNode->sectionZeroScore)));
	scoreOneText->SetText(FText::FromString(FString::FromInt(mctsTreeNode->sectionOneScore)));

	if (mctsTreeNode->winLoseResult == 1.0)
	{
		curNodeWinLoseText->SetText(FText::FromString("Win"));
	}
	else if (mctsTreeNode->winLoseResult == -1.0)
	{
		curNodeWinLoseText->SetText(FText::FromString("Lose"));
	}
	else
	{
		curNodeWinLoseText->SetText(FText::FromString("Draw"));
	}
}

void UMctNodeButton::ClickButton()
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	ACoreCardGameModeBase* coreGameMode = Cast<ACoreCardGameModeBase>(gameMode);
	coreGameMode->DemonstrateMctsTreeNode(mctsTreeNode);

	/*
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	ACoreCardGamePC* coreGamePC = Cast<ACoreCardGamePC>(pc);
	coreGamePC->mctReplayMenu->UpdateWHistories(mctsTreeNode->updateWActionIds, 
		mctsTreeNode->updateWLaunchSections, 
		mctsTreeNode->updateWHistories,
		mctsTreeNode->updateQUHistories);
	*/
}

