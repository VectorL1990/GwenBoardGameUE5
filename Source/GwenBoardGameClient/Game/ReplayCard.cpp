// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ReplayCard.h"

// Sets default values
AReplayCard::AReplayCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReplayCard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AReplayCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AReplayCard::Init(FString cardName, int32 hp, int32 defence)
{
	cardWidgetComponent = GetComponentByClass<UWidgetComponent>();
	UCardWidget3D* widget = Cast<UCardWidget3D>(cardWidgetComponent->GetWidget());
	widget->NotifyInit();
	widget->SetupHpDefence(hp, defence);
}

