// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CoreGameBlueprintFunctionLibrary.h"
#include <Components/WidgetComponent.h>
#include <Components/StaticMeshComponent.h>
#include "UI/CardWidget3D.h"
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

    UFUNCTION(BlueprintNativeEvent)
    void NotifyInit();

    void InitCard(
        FString inCardName,
        int32 inCurHp,
        int32 inCurDefence,
        int32 inCurCd,
        int32 inCurAvailable);

    void UpdateCard(int32 inCurHp, int32 inCurDefence, int32 inCurCd, int32 inCurAvailable);

    FPLAY_CARD_INFO GetCardInfo();

    void Discard();

    void Highlight();

    void DeHighlight();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidgetComponent* cardWidgetComponent;

    CardAnimationStatus cardAnimationStatus = CardAnimationStatus::Default;
    BattleCardStatus cardStatus = BattleCardStatus::InHand;

    FString cardName;

    FString cardUid;

    int32 cardId;

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

    uint8 camp;

    int32 gridX;

    int32 gridY;





    ECardVerticalMotionStage verticalMotionStage = ECardVerticalMotionStage::Default;

    ECardRotationStage cardRotationStage = ECardRotationStage::Default;

    ECardHorizonMotionStage cardHorizonXMotionStage = ECardHorizonMotionStage::Default;

    ECardHorizonMotionStage cardHorizonYMotionStage = ECardHorizonMotionStage::Default;

    float testMotionCardRiseTop = 0.0;

    float testMotionCardXOffset = 0.0;

    float testMotionCardYOffset = 0.0;

    float testMotionCardXAccMotion = 0.0;

    float testMotionCardYAccMotion = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testDropAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testRebounceAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testRebounceSpeedLost = 0.5;

    float testMotionCardSpeed = 0.0;

    float testMotionCardHorizonXSpeed = 0.0;

    float testMotionCardHorizonYSpeed = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float maxMotionCardSpeed = 100.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardRotationAttenuation = 0.5;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardRotateSpeed = 0.4;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardMaxRotationPitch = 20.0;

    float testMotionCardCurRotationPitch = 0.0;

    float testMotionCardNextRotationPitch = 0.0;

    float testMotionCardHorizonXAcc = 0.0;

    float testMotionCardHorizonYAcc = 0.0;

    FVector2D testMotionCardHorizonTarget = FVector2D::Zero();

    UPROPERTY(EditDefaultsOnly)
    float testMotionHorizonT = 1.0;

    float testMotionCardCurXLoc = 0.0;

    float testMotionCardCurYLoc = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardLerp = 0.2;

    float testMotionCurCountT = 0.0;

    float testMotionRiseT = 0.0;

    float testMotionTotalT = 0.0;

    UFUNCTION(BlueprintCallable)
    void TriggerCardMotion(FVector2D targetHorizonTarget);

    UFUNCTION(BlueprintCallable)
    void TriggerCardRotation();

    UFUNCTION(BlueprintCallable)
    void CardMotion(float dT);

    UFUNCTION(BlueprintCallable)
    void CardRotation(float dT);
};
