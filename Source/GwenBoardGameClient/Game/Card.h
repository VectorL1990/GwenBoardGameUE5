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


    ECardVerticalMotionStage cardHurtMotionStage = ECardVerticalMotionStage::Default;

    ECardRotationStage cardHurtRotationStage = ECardRotationStage::Default;

    UPROPERTY(EditDefaultsOnly)
    float maxHurtCardRiseSpeed = 100.0;

    UPROPERTY(EditDefaultsOnly)
    float hurtCardRiseAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float hurtCardDropAcc = 100;

    float hurtCardMotionTotalT = 0.0;

    float hurtCardMotionCurCountT = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float deadCountDown = 0.8;

    float curDeadCountDown = 0.0;

    UFUNCTION(BlueprintCallable)
    void TriggerCardEffectMotion();

    UFUNCTION(BlueprintCallable)
    void TriggerCardHurtRotation();

    UFUNCTION(BlueprintCallable)
    void CardHurtMotion(float dT);

    UFUNCTION(BlueprintCallable)
    void CardHurtRotation(float dT);

    void ResetCardHurtMotion();

    void TriggerCardDeadAnim();

    void KillCard();







    ECardVerticalMotionStage cardVerticalMotionStage = ECardVerticalMotionStage::Default;

    ECardRotationStage cardRotationStage = ECardRotationStage::Default;

    ECardHorizonMotionStage cardHorizonXMotionStage = ECardHorizonMotionStage::Default;

    ECardHorizonMotionStage cardHorizonYMotionStage = ECardHorizonMotionStage::Default;

    float cardMotionRiseTop = 0.0;
    
    float cardMotionBottom = 120.0;

    float cardMotionXOffset = 0.0;

    float cardMotionYOffset = 0.0;

    float cardXMotion = 0.0;

    float cardYMotion = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardVerticalRiseAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float cardVerticalDropAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float cardRebounceVerticalAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float cardRebounceSpeedLost = 0.5;

    float cardVerticalSpeed = 0.0;

    float cardHorizonXSpeed = 0.0;

    float cardHorizonYSpeed = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float maxMotionCardSpeed = 100.0;


    UPROPERTY(EditDefaultsOnly)
    float cardRotateSpeed = 0.4;

    UPROPERTY(EditDefaultsOnly)
    float cardMaxRotationPitch = 20.0;

    UPROPERTY(EditDefaultsOnly)
    float cardMotionPitchAdjustCoe = 1.0;

    UPROPERTY(EditDefaultsOnly)
    float cardMotionYawAdjustCoe = 0.15;

    UPROPERTY(EditDefaultsOnly)
    float cardMotionRollAdjustCoe = 0.15;

    float cardCurRotationPitch = 0.0;

    float cardNextRotationPitch = 0.0;

    float cardMotionHorizonXAcc = 0.0;

    float cardMotionHorizonYAcc = 0.0;

    FVector2D cardHorizonTarget = FVector2D::Zero();


    float cardCurXLoc = 0.0;

    float cardCurYLoc = 0.0;


    float cardMotionCurCountT = 0.0;

    float cardMotionRiseT = 0.0;

    float cardMotionTotalT = 0.0;

    UFUNCTION(BlueprintCallable)
    void TriggerCardMotion(FVector2D targetHorizonTarget);

    UFUNCTION(BlueprintCallable)
    void TriggerCardRotation();

    UFUNCTION(BlueprintCallable)
    void CardMotion(float dT);

    UFUNCTION(BlueprintCallable)
    void CardRotation(float dT);

    ECardFloatMotionStage cardFloatMotionStage = ECardFloatMotionStage::Default;

    ECardFloatRotateStage cardFloatRotateStage = ECardFloatRotateStage::Default;

    float cardFloatOriginHeight = 0.0;

    float cardFloatCurSpeed = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatMaxSpeed = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatRiseAcc = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatRotateMaxProgress = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatRotateProgressSpeed = 0.0;

    float cardFloatRotateCurProgress = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatMaxRotate_1 = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float cardFloatMaxRotate_2 = 0.0;

    void TriggerSelectFloat();

    void RecoverSelectOrigin();

    void TriggerHoverRotate();

    void SelectFloatingRotate(float dT);

    void SelectFloatingRise(float dT);

    void ResetCardMotion();
};
