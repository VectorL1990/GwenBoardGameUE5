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
				if (mctsTreeNode->children.Num() > 0)
				{
								APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
								ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(pc);

								// Trigger mcts node tree reconstruction
								coreCardGamePC->mctReplayMenu->GetInMctsNodesWidget(mctsTreeNode);
				}

				AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
				ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
				coreCardGameMode->replayBoard->ClearAllCards();

				for (int32 i = 0; i < mctsTreeNode->stateStrings.Num(); i++)
				{
								int32 col = i % UGlobalConstFunctionLibrary::maxCol;
								int32 row = i / UGlobalConstFunctionLibrary::maxCol;
								
								if (mctsTreeNode->stateStrings[i] != "None")
								{
												TArray<FString> splitStateStrings;
												mctsTreeNode->stateStrings[i].ParseIntoArray(splitStateStrings, TEXT("/"), true);
												coreCardGameMode->replayBoard->AddCard(
																col,
																row,
																splitStateStrings[0],
																FCString::Atoi(*splitStateStrings[1]),
																FCString::Atoi(*splitStateStrings[2]),
																FCString::Atoi(*splitStateStrings[3]),
																FCString::Atoi(*splitStateStrings[4]));
								}

								
				}
}

