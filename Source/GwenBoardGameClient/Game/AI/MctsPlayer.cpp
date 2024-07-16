// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/MctsPlayer.h"

// Sets default values
AMctsPlayer::AMctsPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMctsPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMctsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

