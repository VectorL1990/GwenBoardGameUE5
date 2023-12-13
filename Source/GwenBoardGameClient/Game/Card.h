// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CoreGameBlueprintFunctionLibrary.h"
#include "Card.generated.h"



UCLASS()
class GWENBOARDGAMECLIENT_API ACard : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACard();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void InitCard(FString cardName);

    FPLAY_CARD_INFO GetCardInfo();

    BattleCardStatus cardStatus = BattleCardStatus::Standby;

    FString cardName;

    FString cardUid;

    int32 hp;

    int32 agility;

    int32 defence;

    UPROPERTY()
    TArray<FString> tags;

    UPROPERTY()
    TArray<FString> stateTags;

};
