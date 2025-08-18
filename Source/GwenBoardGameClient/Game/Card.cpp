// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "CoreCardGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Base/GwenBoardGameInstance.h"


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

	CardMotion(DeltaTime);
	CardRotation(DeltaTime);
}

void ACard::NotifyInit_Implementation()
{

}

void ACard::InitCard(
    FString inCardName,
    int32 inCurHp,
    int32 inCurDefence,
    int32 inCurCd,
    int32 inCurAvailable)
{
    NotifyInit();
    cardName = inCardName;
    UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
    UGwenBoardGameInstance* gameGI = Cast<UGwenBoardGameInstance>(gi);

    UStaticMeshComponent* staticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
    UMaterialInstanceDynamic* mi = Cast<UMaterialInstanceDynamic>(staticMeshComponent->GetMaterial(0));
    mi->SetTextureParameterValue("BaseTexture", gameGI->allCardInfos[cardName].texture);

    cardWidgetComponent = GetComponentByClass<UWidgetComponent>();
    UCardWidget3D* widget = Cast<UCardWidget3D>(cardWidgetComponent->GetWidget());
    widget->NotifyInit();
    widget->SetupHpDefence(inCurHp, inCurDefence, inCurCd, inCurAvailable);
}

void ACard::UpdateCard(int32 inCurHp, int32 inCurDefence, int32 inCurCd, int32 inCurAvailable)
{
    cardWidgetComponent = GetComponentByClass<UWidgetComponent>();
    UCardWidget3D* widget = Cast<UCardWidget3D>(cardWidgetComponent->GetWidget());
    widget->SetupHpDefence(inCurHp, inCurDefence, inCurCd, inCurAvailable);
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

void ACard::Highlight()
{

}

void ACard::DeHighlight()
{

}

void ACard::TriggerCardMotion(FVector2D targetHorizonTarget)
{
    cardVerticalMotionStage = ECardVerticalMotionStage::RiseAcc;
    cardHorizonTarget = targetHorizonTarget;

    FVector2D curHorizonLoc = FVector2D(GetActorLocation().X, GetActorLocation().Y);
    float xOffset = targetHorizonTarget.X - curHorizonLoc.X;
    float yOffset = targetHorizonTarget.Y - curHorizonLoc.Y;

    cardMotionXOffset = FMath::Abs(xOffset);
    cardMotionYOffset = FMath::Abs(yOffset);

    cardHorizonXMotionStage = ECardHorizonMotionStage::Acc;
    cardHorizonYMotionStage = ECardHorizonMotionStage::Acc;

    float riseT0 = maxMotionCardSpeed / cardVerticalRiseAcc * 2.0;
    cardMotionRiseTop = 0.5 * cardVerticalRiseAcc * riseT0 * riseT0 * 2.0 + GetActorLocation().Z;

    float dropT0 = FMath::Sqrt(2.0 * (cardMotionRiseTop - cardMotionBottom) / cardVerticalDropAcc);
    float rebounceSpeed = cardVerticalDropAcc * dropT0 * cardRebounceSpeedLost;
    float riseT1 = rebounceSpeed / cardRebounceVerticalAcc;
    float dropT1 = riseT1;
    cardMotionTotalT = riseT0 + dropT0 + riseT1 + dropT1;
    cardMotionRiseT = riseT0;
    cardMotionCurCountT = 0.0;

    cardMotionHorizonXAcc = xOffset / (cardMotionRiseT * cardMotionRiseT);
    cardMotionHorizonYAcc = yOffset / (cardMotionRiseT * cardMotionRiseT);
}

void ACard::TriggerCardRotation()
{
    cardRotationStage = ECardRotationStage::PosRotate;
    cardNextRotationPitch = cardMaxRotationPitch;
}

void ACard::CardMotion(float dT)
{
	if (cardMotionCurCountT >= cardMotionTotalT)
	{
		return;
	}

	if (cardHorizonXMotionStage == ECardHorizonMotionStage::Acc)
	{
		float xMotion = cardHorizonXSpeed * dT + 0.5 * cardMotionHorizonXAcc * dT * dT;
		cardXMotion += FMath::Abs(xMotion);
		float xPos = GetActorLocation().X + xMotion;
		//float xPos = testMotionCard->GetActorLocation().X;
		cardHorizonXSpeed += cardMotionHorizonXAcc * dT;
		if (cardXMotion >= cardMotionXOffset / 2.0)
		{
			cardHorizonXMotionStage = ECardHorizonMotionStage::Dec;
		}
		cardCurXLoc = xPos;
	}
	else if (cardHorizonXMotionStage == ECardHorizonMotionStage::Dec)
	{
		float xMotion = cardHorizonXSpeed * dT + 0.5 * cardMotionHorizonXAcc * dT * dT;
		cardXMotion += FMath::Abs(xMotion);
		float xPos = GetActorLocation().X + xMotion;
		//float xPos = testMotionCard->GetActorLocation().X;
		cardHorizonXSpeed -= cardMotionHorizonXAcc * dT;
		if (cardXMotion >= cardMotionXOffset)
		{
			cardHorizonXMotionStage = ECardHorizonMotionStage::Default;
			xPos = cardHorizonTarget.X;
		}
		cardCurXLoc = xPos;
	}


	if (cardHorizonYMotionStage == ECardHorizonMotionStage::Acc)
	{
		float yMotion = cardHorizonYSpeed * dT + 0.5 * cardMotionHorizonYAcc * dT * dT;
		cardYMotion += FMath::Abs(yMotion);
		float yPos = GetActorLocation().Y + yMotion;
		//float yPos = testMotionCard->GetActorLocation().Y;
		cardHorizonYSpeed += cardMotionHorizonYAcc * dT;
		if (cardYMotion >= cardMotionYOffset / 2.0)
		{
			cardHorizonYMotionStage = ECardHorizonMotionStage::Dec;
		}
		cardCurYLoc = yPos;
	}
	else if (cardHorizonYMotionStage == ECardHorizonMotionStage::Dec)
	{
		float yMotion = cardHorizonYSpeed * dT + 0.5 * cardMotionHorizonYAcc * dT * dT;
		cardYMotion += FMath::Abs(yMotion);
		float yPos = GetActorLocation().Y + yMotion;
		//float yPos = testMotionCard->GetActorLocation().Y;
		cardHorizonYSpeed -= cardMotionHorizonYAcc * dT;
		if (cardYMotion >= cardMotionYOffset)
		{
			cardHorizonYMotionStage = ECardHorizonMotionStage::Default;
			yPos = cardHorizonTarget.Y;
		}
		cardCurYLoc = yPos;
	}



	if (cardVerticalMotionStage == ECardVerticalMotionStage::RiseAcc)
	{
		float rising = GetActorLocation().Z + cardVerticalSpeed * dT + 0.5 * cardVerticalRiseAcc * dT * dT;
		cardVerticalSpeed += cardVerticalRiseAcc * dT;
		if (cardVerticalSpeed >= maxMotionCardSpeed)
		{
			cardVerticalSpeed = maxMotionCardSpeed;
			cardVerticalMotionStage = ECardVerticalMotionStage::RiseDec;
		}
		FVector des = FVector(cardCurXLoc, cardCurYLoc, rising);
		SetActorLocation(des);
	}
	else if (cardVerticalMotionStage == ECardVerticalMotionStage::RiseDec)
	{
		float rising = GetActorLocation().Z + cardVerticalSpeed * dT + 0.5 * cardVerticalRiseAcc * dT * dT;
		cardVerticalSpeed -= cardVerticalRiseAcc * dT;
		if (cardVerticalSpeed <= 0.0)
		{
			cardVerticalSpeed = 0.0;
			cardVerticalMotionStage = ECardVerticalMotionStage::DropAcc;
		}
		FVector des = FVector(cardCurXLoc, cardCurYLoc, rising);
		SetActorLocation(des);
	}
	else if (cardVerticalMotionStage == ECardVerticalMotionStage::DropAcc)
	{
		float drop = GetActorLocation().Z - cardVerticalSpeed * dT - 0.5 * cardVerticalDropAcc * dT * dT;
		cardVerticalSpeed += cardVerticalDropAcc * dT;
		FVector des = FVector(cardCurXLoc, cardCurYLoc, drop);
		SetActorLocation(des);
		if (des.Z <= 120.0)
		{
			cardVerticalMotionStage = ECardVerticalMotionStage::RebounceUp;
			des.Z = 120.0;
			SetActorLocation(des);
			cardVerticalSpeed = cardVerticalSpeed * cardRebounceSpeedLost;

			TriggerCardRotation();
		}
	}
	else if (cardVerticalMotionStage == ECardVerticalMotionStage::RebounceUp)
	{
		float rebounce = GetActorLocation().Z + cardVerticalSpeed * dT + 0.5 * cardRebounceVerticalAcc * dT * dT;
		cardVerticalSpeed -= cardRebounceVerticalAcc * dT;
		FVector des = FVector(cardCurXLoc, cardCurYLoc, rebounce);
		SetActorLocation(des);
		if (cardVerticalSpeed <= 0.0)
		{
			cardVerticalSpeed = 0.0;
			cardVerticalMotionStage = ECardVerticalMotionStage::RebounceDown;
		}
	}
	else if (cardVerticalMotionStage == ECardVerticalMotionStage::RebounceDown)
	{
		float drop = GetActorLocation().Z - cardVerticalSpeed * dT - 0.5 * cardRebounceVerticalAcc * dT * dT;
		cardVerticalSpeed += cardRebounceVerticalAcc * dT;
		FVector des = FVector(cardCurXLoc, cardCurYLoc, drop);
		SetActorLocation(des);
		if (des.Z <= 120.0)
		{
			cardVerticalMotionStage = ECardVerticalMotionStage::Default;
			des.Z = 120.0;
			SetActorLocation(des);
			cardVerticalSpeed = 0.0;
		}
	}

	cardMotionCurCountT += dT;
}

void ACard::CardRotation(float dT)
{
	if (FMath::Abs(cardNextRotationPitch) <= 1.0)
	{
		cardRotationStage = ECardRotationStage::Default;
		FRotator rot = FRotator(0.0, 0.0, 0.0);
		SetActorRotation(rot);
	}
	else
	{
		if (cardRotationStage == ECardRotationStage::PosRotate)
		{
			if (cardCurRotationPitch >= cardNextRotationPitch)
			{
				cardCurRotationPitch = cardNextRotationPitch;
				cardNextRotationPitch = -0.5 * cardNextRotationPitch;
				cardRotationStage = ECardRotationStage::NegRotate;
			}
			else
			{
				cardCurRotationPitch += cardRotateSpeed;
			}
		}
		else if (cardRotationStage == ECardRotationStage::NegRotate)
		{
			if (cardCurRotationPitch <= cardNextRotationPitch)
			{
				cardCurRotationPitch = cardNextRotationPitch;
				cardNextRotationPitch = -0.5 * cardNextRotationPitch;
				cardRotationStage = ECardRotationStage::PosRotate;
			}
			else
			{
				cardCurRotationPitch -= cardRotateSpeed;
			}
		}
		FRotator rot = FRotator(cardCurRotationPitch, cardCurRotationPitch * 0.15, cardCurRotationPitch * 0.15);
		SetActorRotation(rot);
	}
}

