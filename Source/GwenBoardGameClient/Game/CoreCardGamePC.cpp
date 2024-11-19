// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGamePC.h"
#include "CoreCardGameModeBase.h"
#include "../CoreGameManager.h"
#include "Card.h"
#include "BoardGrid.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/MctNodeButton.h"
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

void ACoreCardGamePC::GetRaycastObj(FHitResult& outHitResult)
{
    FVector2D mouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
    FVector worldPosition = FVector::ZeroVector;
    FVector worldDirection = FVector::UpVector;
    UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), mouseViewportPos * viewportScale, worldPosition, worldDirection);
    FHitResult visibleChannelHitResult;
    TArray<AActor*> ignoreActorList;
    UKismetSystemLibrary::LineTraceSingle(this, worldPosition, worldPosition + worldDirection * 100000000, TraceTypeQuery1,
        true, ignoreActorList, EDrawDebugTrace::None, visibleChannelHitResult, true);

    outHitResult = visibleChannelHitResult;
}

void ACoreCardGamePC::DealHover()
{
    FHitResult hitResult;
    GetRaycastObj(hitResult);
    if (hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BattleCard"))))
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->RecoverHoverCardLocations();
            for (int32 i = 0; i < coreCardGameMode->battleCards.Num(); i++)
            {
                if (coreCardGameMode->battleCards[i] == hitResult.GetComponent()->GetOwner())
                {
                    coreCardGameMode->curHighlightCard = coreCardGameMode->battleCards[i];
                    coreCardGameMode->battleCards[i]->Highlight();
                    battleWidget->SetupCardDetail(coreCardGameMode->curHighlightCard->GetActorLocation());
                    coreCardGameMode->CalculateHoverCardLocations(i);
                    break;
                }
            }
        }
        else
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            if (coreCardGameMode->curHighlightCard)
            {
                coreCardGameMode->curHighlightCard->DeHighlight();
                battleWidget->HideCardDetail();
                coreCardGameMode->curHighlightCard = NULL;
            }
            coreCardGameMode->RecoverHoverCardLocations();
        }
    }
    else
    {
        AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
        ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
        if (coreCardGameMode->curHighlightCard)
        {
            coreCardGameMode->curHighlightCard->DeHighlight();
            battleWidget->HideCardDetail();
            coreCardGameMode->curHighlightCard = NULL;
        }
        coreCardGameMode->RecoverHoverCardLocations();
    }
}

void ACoreCardGamePC::DealLeftClick()
{
    FHitResult hitResult;
    GetRaycastObj(hitResult);
    if (hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BattleCard"))))
        {
            ACard* card = Cast<ACard>(hitResult.GetActor());
            if (card->cardStatus == BattleCardStatus::Standby)
            {
                // highlight edge of card and keep high space
                AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
                ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
                coreCardGameMode->selectPlayCard = card;
                coreCardGameMode->SetSelectPlayCard(card);
            }
        }
        else if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BoardGrid"))))
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            if (coreCardGameMode->selectPlayCard)
            {
                
            }
        }
        else
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->selectPlayCard = NULL;
            coreCardGameMode->RecoverSelectPlayCard();
        }
    }
    else
    {
        AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
        ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
        coreCardGameMode->selectPlayCard = NULL;
        coreCardGameMode->RecoverSelectPlayCard();
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

void ACoreCardGamePC::GetCursorScreenPose()
{
    FVector2D mouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
}

void ACoreCardGamePC::InitMenu()
{
    UUserWidget* initBattleWidget = CreateWidget(this, battleWidgetBPClass);
    battleWidget = Cast<UBattleWidget>(initBattleWidget);

    UUserWidget* initSelectCardWidget = CreateWidget(this, selectCardWidgetBPClass);
    selectCardWidget = Cast<USelectCardWidget>(initSelectCardWidget);
}

void ACoreCardGamePC::RegisterSelectCardWidget(UCardWidget* cardWidget)
{
    selectCardWidget->RegisterSelectCard(cardWidget);
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
        selectCardWidget->GenerateSelectCards();
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
        
        selectCardWidget->cardDetailWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        UCanvasPanelSlot* detailWidgetSlot = Cast<UCanvasPanelSlot>(selectCardWidget->cardDetailWidget->Slot);
        detailWidgetSlot->SetPosition(detailPanelPosition);
        selectCardWidget->cardDetailWidget->TriggerShowWidget();
    }
}

void ACoreCardGamePC::HideCardDetail(UCardWidget* cardWidget)
{
    if (cardWidget->battleCardWidgetType == BattleCardWidgetType::BattleSelectCard)
    {
        selectCardWidget->cardDetailWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ACoreCardGamePC::ShowCardDetailInBattle(ACard* card)
{

}

void ACoreCardGamePC::TestSaveString(FString testString)
{
    UCoreGameBlueprintFunctionLibrary::WriteStringToFile("test.json", "", testString);
}

void ACoreCardGamePC::RefreshMctReplayMenu(int32 simulationNb)
{
    
    
}

void ACoreCardGamePC::ConstructMctNodesTreeWidget(UMctsTreeNode* mctsNode)
{
    UUserWidget* newButton = CreateWidget(this, mctNodeButtonBPClass);
    UMctNodeButton* mctNodeButton = Cast<UMctNodeButton>(newButton);
    mctNodeButton->Init(mctsNode);

    //if ()
}

void ACoreCardGamePC::RefreshMctSimulationNbPage(int simulationNb)
{
    
}

