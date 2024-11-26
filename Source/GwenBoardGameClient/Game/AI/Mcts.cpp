// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGamePC.h"
#include "../CoreGameBlueprintFunctionLibrary.h"

// Sets default values
AMcts::AMcts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;
				InitMcts(10);
}

// Called when the game starts or when spawned
void AMcts::BeginPlay()
{
				Super::BeginPlay();
	
}

// Called every frame
void AMcts::Tick(float DeltaTime)
{
				Super::Tick(DeltaTime);

}

void AMcts::InitMcts(int32 simulationMoves)
{
				expandSimulationMoves = simulationMoves;

				treeRoot = NewObject<UMctsTreeNode>();
}


void AMcts::TriggerSimulation(uint8 sectionNb, int32 simulationNb, ABattleBoard* board)
{
				UMctsTreeNode* curNode = treeRoot;



				while (true)
				{
								if (curNode->IsLeaf())
								{
												break;
								}
								int32 action = 0;
								UMctsTreeNode* selectNode = NULL;
								curNode->Select(action, selectNode);
								curNode = selectNode;
								// we should do move here! So that we can predict next action probs
								TArray<FRenderEffectRound> renderEffectRoundList;
								board->TriggerAction(sectionNb, action, true, renderEffectRoundList);

								curNode->stateStrings = board->StateStringCoding(board->simulationBoard);
				}

				uint8* boardCoding = board->StateCoding(board->simulationBoard);
				board->GetLatestSimulationBoard();

				TMap<int32, float> predictActionProbs;
				float simulationStateValue;
				board->GetLegalActionProbsBoardValue(sectionNb, boardCoding, predictActionProbs, simulationStateValue);

				// Tell whether game is end
				bool isGameEnd = false;

				// expand the tree and update P, U for each node
				if (!isGameEnd)
				{
								curNode->Expand(curNode->hirachy, predictActionProbs);
				}
				else
				{

				}

				curNode->UpdateEvaluateQValue(simulationStateValue);

				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(playerController);
				coreCardGamePC->RefreshMctReplayMenu(curSearchNode, simulationNb);
}

void AMcts::GetMoveProbs(uint8 sectionNb, ABattleBoard* board, TArray<int32>& outActs, TArray<float>& softmaxProbs)
{
				
				for (int32 i = 0; i < expandSimulationMoves; i++)
				{
								TriggerSimulation(sectionNb, i, board);
				}

				TArray<float> logVisits;
				for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = curSearchNode->children.CreateConstIterator(); iter; ++iter)
				{
								float logVisit = FMath::Loge(iter->Value->visit + 1e-10);
								outActs.Add(iter->Key);
								logVisits.Add(logVisit);
				}

				UCoreGameBlueprintFunctionLibrary::Softmax(logVisits, 0.001, softmaxProbs);
}

void AMcts::UpdateCurSearchNode(int32 targetMove)
{
				if (curSearchNode->children.Contains(targetMove))
				{
								curSearchNode = curSearchNode->children[targetMove];
								curSearchNode->parent = NULL;
				}
				else
				{
								curSearchNode = treeRoot;
				}
}

void AMcts::GetAction(uint8 sectionNb, ABattleBoard* board, int32& targetMove)
{
				TArray<int32> moves;
				TArray<float> softmaxProbs;
				GetMoveProbs(sectionNb, board, moves, softmaxProbs);
				targetMove = UCoreGameBlueprintFunctionLibrary::GetDirichletAction(moves, softmaxProbs);

				if (isTraining)
				{
								UpdateCurSearchNode(targetMove);
				}
				else
				{
								// reset search tree in real battle case
								UpdateCurSearchNode(-1);
				}
}


