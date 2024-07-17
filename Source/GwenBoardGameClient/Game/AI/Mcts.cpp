// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"

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

void AMcts::DoSimulationMove(FCopyBoardInfo& copyBoardInfo)
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


}

