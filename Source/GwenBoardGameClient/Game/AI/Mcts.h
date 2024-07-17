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

				void DoSimulationMove(FCopyBoardInfo& copyBoardInfo);

				UPROPERTY()
				UMctsTreeNode* treeRoot;
};
