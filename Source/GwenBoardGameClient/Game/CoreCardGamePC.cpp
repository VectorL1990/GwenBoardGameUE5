// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGamePC.h"
#include "CoreCardGameModeBase.h"
#include "../CoreGameManager.h"
#include "Card.h"
#include "BoardGrid.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CanvasPanelSlot.h"
#include "../Base/GwenBoardGameInstance.h"


void ACoreCardGamePC::BeginPlay()
{
    SetShowMouseCursor(true);
    InitMenu();
}

void ACoreCardGamePC::Tick(float DeltaTime)
{
    DealHover();
}

void ACoreCardGamePC::DealHover()
{
    FHitResult hitResult;
    bool hitSomething = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, hitResult);
    if (hitSomething && hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("CardPlane"))))
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->RecoverCardLocations();
            for (int32 i = 0; i < coreCardGameMode->testCards.Num(); i++)
            {
                if (coreCardGameMode->testCards[i] == hitResult.GetComponent()->GetOwner())
                {
                    coreCardGameMode->RearrangeCardLocations(i);
                    break;
                }
            }
        }
        else
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->RecoverCardLocations();
        }
    }
    else
    {
        AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
        ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
        coreCardGameMode->RecoverCardLocations();
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


void ACoreCardGamePC::ReceiveFinishCardSelection()
{
    battleWidget->SetFinishCardSelectionText();
}

void ACoreCardGamePC::InitMenu()
{
    UUserWidget* initBattleWidget = CreateWidget(this, battleWidgetBPClass);
    battleWidget = Cast<UBattleWidget>(initBattleWidget);

    UUserWidget* initSelectCardWidget = CreateWidget(this, selectCardWidgetBPClass);
    selectCardWidget = Cast<USelectCardWidget>(initSelectCardWidget);
}

void ACoreCardGamePC::SwitchMenu(FString menuName)
{
    if (curMenuName == "BattleMenu")
    {
        battleWidget->RemoveFromParent();
    }
    else if (curMenuName == "SelectCardMenu")
    {
        selectCardWidget->TriggerClose();
    }

    if (menuName == "BattleMenu")
    {
        battleWidget->AddToViewport();
    }
    else if (menuName == "SelectCardMenu")
    {
        selectCardWidget->AddToViewport();
    }

    curMenuName = menuName;
}

void ACoreCardGamePC::ShowCardDetail(UCardWidget* cardWidget)
{
    if (cardWidget->battleCardWidgetType == BattleCardWidgetType::BattleSelectCard)
    {
        UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(cardWidget->Slot);
        FVector2D slotPosition = canvasPanelSlot->GetPosition();
        FVector2D detailPanelPosition = slotPosition + cardWidget->detailPanelOffset;
        
        UCanvasPanelSlot* detailWidgetSlot = Cast<UCanvasPanelSlot>(selectCardWidget->cardDetailWidget->Slot);
        detailWidgetSlot->SetPosition(detailPanelPosition);
        selectCardWidget->cardDetailWidget->TriggerShowWidget();
    }
}

