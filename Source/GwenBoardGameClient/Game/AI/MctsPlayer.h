// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mcts.h"
#include "MctsPlayer.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API AMctsPlayer : public AActor
{
				GENERATED_BODY()
	
public:	
				// Sets default values for this actor's properties
				AMctsPlayer();

protected:
				// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
				// Called every frame
				virtual void Tick(float DeltaTime) override;

				AMcts* mcts;
};
