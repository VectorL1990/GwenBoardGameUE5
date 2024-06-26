// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "CoreCardGameModeBase.h"


// Sets default values
ACard::ACard()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ACard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (cardAnimationStatus == CardAnimationStatus::Generating)
    {
        UMaterialInstanceDynamic* matInstance = UMaterialInstanceDynamic::Create(basicCardMat, this);
    }
    else if (cardAnimationStatus == CardAnimationStatus::Discarding)
    {

    }

    
}

void ACard::InitCard(FString cardName)
{
    cardAnimationStatus = CardAnimationStatus::Generating;
}

FPLAY_CARD_INFO ACard::GetCardInfo()
{
    FPLAY_CARD_INFO info;
    info.hp = hp;
    info.defence = defence;
    info.agility = agility;
    return info;
}

void ACard::Discard()
{
    cardAnimationStatus = CardAnimationStatus::Discarding;
}

void ACard::MoveCard()
{
    //float test = ACoreCardGameModeBase::cardMoveTime;
}

