// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BattleBoard.h"

// Sets default values
ABattleBoard::ABattleBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

