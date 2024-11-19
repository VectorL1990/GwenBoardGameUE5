// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"
#include "../CoreGameBlueprintFunctionLibrary.h"

// Sets default values
AMcts::AMcts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;

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
}

void AMcts::RecordSimulationTree(int32 actionId, UMctsTreeNode* node)
{
				int32 launchX, launchY, targetX, targetY;
				ActionType actionType;
				UCoreGameBlueprintFunctionLibrary::GetActionDetailFromId(actionId, launchX, launchY, targetX, targetY, actionType);
				if (node->children.Num() > 0)
				{
								for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = node->children.CreateConstIterator(); iter; ++iter)
								{
												RecordSimulationTree(iter->Key, iter->Value);
								}
				}
}

void AMcts::TriggerSimulation(ABattleBoard* board)
{
				UMctsTreeNode* curNode = treeRoot;

				// Record all nodes
				for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = curNode->children.CreateConstIterator(); iter; ++iter)
				{
								int32 actionId = iter->Key;
								int32 launchX, launchY, targetX, targetY;
								ActionType actionType;
								UCoreGameBlueprintFunctionLibrary::GetActionDetailFromId(actionId, launchX, launchY, targetX, targetY, actionType);
								// construct visulization tree
				}



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
								board->TriggerAction(action, true, renderEffectRoundList);

								curNode->stateStrings = board->StateStringCoding(board->simulationBoard);
				}

				uint8* boardCoding = board->StateCoding(board->simulationBoard);
				board->GetLatestSimulationBoard();

				TMap<int32, float> predictActionProbs;
				float simulationStateValue;
				board->GetLegalActionProbsBoardValue(boardCoding, predictActionProbs, simulationStateValue);

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
}

void AMcts::GetMoveProbs(ABattleBoard* board, TArray<int32>& outActs, TArray<float>& softmaxProbs)
{
				
				for (int32 i = 0; i < expandSimulationMoves; i++)
				{
								TriggerSimulation(board);
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

void AMcts::GetAction(ABattleBoard* board, int32& targetMove)
{
				TArray<int32> moves;
				TArray<float> softmaxProbs;
				GetMoveProbs(board, moves, softmaxProbs);
				targetMove = UCoreGameBlueprintFunctionLibrary::GetDirichletAction(moves, softmaxProbs);
				UpdateCurSearchNode(targetMove);
}

