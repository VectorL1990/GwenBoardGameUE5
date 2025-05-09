// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MctNodeButton.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGameModeBase.h"
#include "../CoreCardGamePC.h"

void UMctNodeButton::Init(UMctsTreeNode* inMctsTreeNode)
{
	mctsTreeNode = inMctsTreeNode;
}

void UMctNodeButton::ClickButton()
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	ACoreCardGameModeBase* coreGameMode = Cast<ACoreCardGameModeBase>(gameMode);
	coreGameMode->DemonstrateMctsTreeNode(mctsTreeNode);
}

