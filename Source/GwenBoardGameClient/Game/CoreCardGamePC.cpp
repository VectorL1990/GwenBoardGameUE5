// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGamePC.h"
#include "CoreCardGameModeBase.h"
#include "../CoreGameManager.h"
#include "Card.h"
#include "BoardGrid.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Base/GwenBoardGameInstance.h"

void ACoreCardGamePC::BeginPlay()
{
    SetShowMouseCursor(true);
    ShowBattleWidget();
}

void ACoreCardGamePC::Tick(float DeltaTime)
{

}

void ACoreCardGamePC::DealHover()
{
    FHitResult hitResult;
    bool hitSomething = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, hitResult);
    if (hitSomething && hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BoardGrid"))))
        {
            ABoardGrid* boardGrid = Cast<ABoardGrid>(hitResult.GetActor());
            if (boardGrid)
            {
                // if play has toggle card in hand, we should spawn a shadow card on board for demonstration
                
            }
        }
    }
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
                if (card->cardStatus == BattleCardStatus::Select)
                {
                    AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
                    ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
                }
                else if (card->cardStatus == BattleCardStatus::Standby)
                {
                    // which means player wants to look into the detail of this card
                    // show detail in battle widget
                }
            }
        }
        else if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BoardGrid"))))
        {
            ABoardGrid* boardGrid = Cast<ABoardGrid>(hitResult.GetActor());
            // send message to server to update battle
        }
    }
}

void ACoreCardGamePC::InitSelectCardCamera()
{
    AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
    ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
    ABattleCamera* selectCardCamera = coreCardGameMode->camerasMap[CameraType::SelectCardCamera];
    SetViewTarget(selectCardCamera);
}

void ACoreCardGamePC::ShowBattleWidget()
{
    UUserWidget* widget = CreateWidget(this, battleWidgetBPClass);
    widget->AddToPlayerScreen(-1);
    //battleWidget = Cast<UBattleWidget>(widget);
}

void ACoreCardGamePC::ReceiveFinishCardSelection()
{
    battleWidget->SetFinishCardSelectionText();
}

