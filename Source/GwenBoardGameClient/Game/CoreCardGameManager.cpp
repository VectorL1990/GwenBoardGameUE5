// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameManager.h"

ACoreCardGameManager* ACoreCardGameManager::instance = nullptr;

// Sets default values
ACoreCardGameManager::ACoreCardGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;
				
}

ACoreCardGameManager* ACoreCardGameManager::GetInstance()
{
				return instance;
}

// Called when the game starts or when spawned
void ACoreCardGameManager::BeginPlay()
{
				Super::BeginPlay();
				
}

// Called every frame
void ACoreCardGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACoreCardGameManager::Init(AActor* actor)
{
				if (!actor)
				{
								return false;
				}
				return true;
}

