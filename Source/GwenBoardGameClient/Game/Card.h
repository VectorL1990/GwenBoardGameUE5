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

private:
    float cardAnimStateTick = 0.0;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void MoveCard();

    void InitCard(FString cardName);

    FPLAY_CARD_INFO GetCardInfo();

    void Discard();

    CardAnimationStatus cardAnimationStatus = CardAnimationStatus::Default;
    BattleCardStatus cardStatus = BattleCardStatus::Standby;

    FString cardName;

    FString cardUid;

    int32 hp;

    int32 agility;

    int32 defence;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* basicCardMat;

    UPROPERTY()
    TArray<FString> inherentTags;

    UPROPERTY()
    TArray<FSTATE_TAG_INFO> stateTags;

    UPROPERTY(BlueprintReadWrite)
    float curMoveCardTime = 0.0;
};
