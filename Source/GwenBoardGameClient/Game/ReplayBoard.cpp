// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayBoard.h"

// Sets default values
AReplayBoard::AReplayBoard()
{
				// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReplayBoard::BeginPlay()
{
				Super::BeginPlay();
	
}

// Called every frame
void AReplayBoard::Tick(float DeltaTime)
{
				Super::Tick(DeltaTime);

}

FVector AReplayBoard::GetGridLocation(int32 col, int32 row)
{
				return FVector::Zero();
}

void AReplayBoard::ClearAllCards()
{
				for (int32 i = 0; i < allCards.Num(); i++)
				{
								if (allCards[i]->IsValidLowLevel())
								{
												allCards[i]->ConditionalBeginDestroy();
								}
				}
				allCards.Empty();
}

void AReplayBoard::AddCard(int32 col,
				int32 row,
				FString cardName,
				int32 hp,
				int32 defence,
				int32 coolDown,
				int32 availableTimes)
{
				FVector cardLocation = GetGridLocation(col, row);
				ACard* card = GetWorld()->SpawnActor<ACard>(cardBPClass, cardLocation, FRotator::ZeroRotator);
				card->InitCard(cardName);
				allCards.Add(card);
}

