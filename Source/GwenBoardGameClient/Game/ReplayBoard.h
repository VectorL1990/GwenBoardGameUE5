// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.h"
#include "ReplayBoard.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API AReplayBoard : public AActor
{
	GENERATED_BODY()
	
public:	
				// Sets default values for this actor's properties
				AReplayBoard();

protected:
				// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
				UPROPERTY(EditDefaultsOnly)
				TSubclassOf<ACard> cardBPClass;

				TArray<ACard*> allCards;

				// Called every frame
				virtual void Tick(float DeltaTime) override;

				FVector GetGridLocation(int32 col, int32 row);

				void ClearAllCards();

				void AddCard(
								int32 col, 
								int32 row, 
								FString cardName,
								int32 hp,
								int32 defence,
								int32 coolDown,
								int32 availableTimes);

};
