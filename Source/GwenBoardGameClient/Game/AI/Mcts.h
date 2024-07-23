// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GlobalConstFunctionLibrary.h"
#include "MctsTreeNode.h"
#include "Mcts.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API AMcts : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
				AMcts();

protected:
	// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
	// Called every frame
				virtual void Tick(float DeltaTime) override;

				void InitMcts(int32 simulationMoves);

				void DoSimulationMove(uint8* boardState);

				void GetMoveProbs(uint8* boardState, TArray<int32>& outActs, TArray<float>& softmaxProbs);

				void UpdateCurSearchNode(int32 targetMove);

				void GetAction(uint8* boardState, int32& targetMove, TArray<float> softmaxProbs);

				UPROPERTY()
				int32 expandSimulationMoves = 200;

				UPROPERTY()
				UMctsTreeNode* treeRoot;

				UPROPERTY()
				UMctsTreeNode* curSearchNode;
};
