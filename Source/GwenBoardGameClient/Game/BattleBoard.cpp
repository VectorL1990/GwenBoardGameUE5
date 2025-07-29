// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BattleBoard.h"
#include "Kismet/GameplayStatics.h"
#include "Base/GwenBoardGameInstance.h"
#include "CheckTargetGeoRuleLibrary.h"
#include "CheckPrereqTagFunctionLibrary.h"
#include "PassiveEffectFunctionLibrary.h"
#include "CheckPrereqFunctionLibrary.h"

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

void ABattleBoard::InitBattleBoard()
{
}

bool ABattleBoard::CheckGameEnd()
{
				return false;
}


