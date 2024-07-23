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

void AMcts::DoSimulationMove(uint8* boardState)
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
				}

				TMap<int32, float> predictActionProbs;
				float simulationStateValue;
				UCoreGameBlueprintFunctionLibrary::QueryRemotePolicyValue(boardState, predictActionProbs, simulationStateValue);

				// Tell whether game is end
				bool isGameEnd = false;

				// expand the tree and update P, U for each node
				if (!isGameEnd)
				{
								curNode->Expand(predictActionProbs);
				}
				else
				{

				}

				curNode->UpdateEvaluateQValue(simulationStateValue);
}

void AMcts::GetMoveProbs(uint8* boardState, TArray<int32>& outActs, TArray<float>& softmaxProbs)
{
				for (int32 i = 0; i < expandSimulationMoves; i++)
				{
								uint8 copyBoard[UGlobalConstFunctionLibrary::boardStateLen];
								memcpy(copyBoard, boardState, UGlobalConstFunctionLibrary::boardStateLen*sizeof(uint8));
								DoSimulationMove(copyBoard);
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

void AMcts::GetAction(uint8* boardState, int32& targetMove, TArray<float> softmaxProbs)
{
				TArray<int32> moves;
				GetMoveProbs(boardState, moves, softmaxProbs);
				targetMove = UCoreGameBlueprintFunctionLibrary::GetDirichletAction(moves, softmaxProbs);
				UpdateCurSearchNode(targetMove);
}

