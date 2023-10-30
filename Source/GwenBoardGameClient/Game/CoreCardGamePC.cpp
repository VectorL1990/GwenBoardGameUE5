// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGamePC.h"
#include "../CoreGameManager.h"
#include "Card.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "../Base/GwenBoardGameInstance.h"

void ACoreCardGamePC::BeginPlay()
{
    SetShowMouseCursor(true);

}

void ACoreCardGamePC::Tick(float DeltaTime)
{

}

void ACoreCardGamePC::DealLeftClick()
{
    FHitResult hitResult;
    bool hitSomething = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, hitResult);
    if (hitSomething && hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BattleCard"))))
        {
            ACard* card = Cast<ACard>(hitResult.GetActor());
            if (card)
            {
                if (card->cardStatus == BattleCardStatus::Standby)
                {
                    // which means 
                }
            }
        }
    }
}

