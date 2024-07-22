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

void AMcts::GetMoveProbs(uint8* boardState)
{
				for (int32 i=0;)
}

