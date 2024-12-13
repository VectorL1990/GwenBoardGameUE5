// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "CoreGameBlueprintFunctionLibrary.h"
#include "Card.h"
#include "BattleBoard.generated.h"




UCLASS()
class GWENBOARDGAMECLIENT_API ABattleBoard : public AActor
{
	GENERATED_BODY()
	
public:	
				// Sets default values for this actor's properties
				ABattleBoard();

protected:
				// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
				// Called every frame
				virtual void Tick(float DeltaTime) override;

				void InitBattleBoard();

				bool CheckGameEnd();

				UPROPERTY()
				uint8 curPlayerTurn;


				TMap<int32, ACard*> allCards;
};
