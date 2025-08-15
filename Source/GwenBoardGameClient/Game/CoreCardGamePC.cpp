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
    //mctReplayMenu->RefreshMctsMenu();
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
            bool findCard = false;

            // Is it hand card of section zero ???
            for (int32 i = 0; i < coreCardGameMode->sectionZeroHandBattleCards.Num(); i++)
            {
                if (coreCardGameMode->sectionZeroHandBattleCards[i] == hitResult.GetComponent()->GetOwner())
                {
                    coreCardGameMode->curHighlightCard = coreCardGameMode->sectionZeroHandBattleCards[i];
                    coreCardGameMode->sectionZeroHandBattleCards[i]->Highlight();
                    battleWidget->SetupCardDetail(coreCardGameMode->curHighlightCard->GetActorLocation(),
                        coreCardGameMode->sectionZeroHandBattleCards[i]->cardName);
                    coreCardGameMode->CalculateHoverCardLocations(0, i);
                    findCard = true;
                    break;
                }
            }

            // Is it hand card of section one ???
            if (!findCard)
            {
                for (int32 i = 0; i < coreCardGameMode->sectionOneHandBattleCards.Num(); i++)
                {
                    if (coreCardGameMode->sectionOneHandBattleCards[i] == hitResult.GetComponent()->GetOwner())
                    {
                        coreCardGameMode->curHighlightCard = coreCardGameMode->sectionOneHandBattleCards[i];
                        coreCardGameMode->sectionOneHandBattleCards[i]->Highlight();
                        battleWidget->SetupCardDetail(coreCardGameMode->curHighlightCard->GetActorLocation(),
                            coreCardGameMode->sectionOneHandBattleCards[i]->cardName);
                        coreCardGameMode->CalculateHoverCardLocations(1, i);
                        findCard = true;
                        break;
                    }
                }
            }

            if (!findCard)
            {
                for (TMap<int32, ACard*>::TConstIterator iter = coreCardGameMode->allBattleCards.CreateConstIterator(); iter; ++iter)
                {
                    if (iter->Value == hitResult.GetComponent()->GetOwner())
                    {
                        coreCardGameMode->curHighlightCard = iter->Value;
                        iter->Value->Highlight();
                        battleWidget->SetupCardDetail(iter->Value->GetActorLocation(), iter->Value->cardName);
                        findCard = true;
                        break;
                    }
                }
            }
        }
        else if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("ReplayCard"))) &&
            hitResult.GetComponent()->GetOwner())
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            for (int32 i = 0; i < coreCardGameMode->allReplayCards.Num(); i++)
            {
                if (coreCardGameMode->allReplayCards[i] == hitResult.GetComponent()->GetOwner())
                {
                    battleWidget->SetupCardDetail(coreCardGameMode->allReplayCards[i]->GetActorLocation(),
                        coreCardGameMode->allReplayCards[i]->cardName);
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
                coreCardGameMode->curHighlightCard = NULL;
            }
            battleWidget->HideCardDetail();
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
            if (card->cardStatus == BattleCardStatus::InHand)
            {
                // highlight edge of card and keep high space
                AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
                ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
                coreCardGameMode->selectPlayCard = card;
                coreCardGameMode->SetSelectPlayCard(card->camp, card);
            }
            else if (card->cardStatus == BattleCardStatus::InBattle)
            {
                AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
                ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
                coreCardGameMode->selectBoardCard = card;
            }
        }
        else if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BoardGrid"))))
        {
            DrawDebugSphere(GetWorld(), hitResult.Location, 50.0, 10, FColor::Red, false, 1.0);
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            if (coreCardGameMode->selectPlayCard)
            {
                ABoardGrid* grid = Cast<ABoardGrid>(hitResult.GetActor());
                int32 launchX = coreCardGameMode->selectPlayCard->gridX;
                int32 launchY = coreCardGameMode->selectPlayCard->gridY;
                int32 targetX = grid->gridX;
                int32 targetY = grid->gridY + UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow;
                coreCardGameMode->TestTriggerAction(coreCardGameMode->selectPlayCard->camp, launchX, launchY, targetX, targetY, ActionType::PlayCard);
            }
        }
        else
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->selectPlayCard = NULL;
            coreCardGameMode->selectBoardCard = NULL;
            coreCardGameMode->RecoverSelectPlayCard();
        }
    }
    else
    {
        AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
        ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
        coreCardGameMode->selectPlayCard = NULL;
        coreCardGameMode->selectBoardCard = NULL;
        coreCardGameMode->RecoverSelectPlayCard();
    }
}

void ACoreCardGamePC::DealRightClick()
{
    FHitResult hitResult;
    GetRaycastObj(hitResult);
    if (hitResult.bBlockingHit)
    {
        if (hitResult.GetComponent() && hitResult.GetComponent()->ComponentHasTag(FName(TEXT("BoardGrid"))))
        {
            DrawDebugSphere(GetWorld(), hitResult.Location, 50.0, 10, FColor::Cyan, false, 1.0);
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            if (coreCardGameMode->selectBoardCard)
            {
                ABoardGrid* grid = Cast<ABoardGrid>(hitResult.GetActor());
                int32 launchX = coreCardGameMode->selectBoardCard->gridX;
                int32 launchY = coreCardGameMode->selectBoardCard->gridY;
                int32 targetX = grid->gridX;
                int32 targetY = grid->gridY + UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow;
                coreCardGameMode->TestTriggerAction(coreCardGameMode->selectBoardCard->camp, launchX, launchY, targetX, targetY, ActionType::Move);
            }
        }
        else
        {
            AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
            ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
            coreCardGameMode->selectPlayCard = NULL;
            coreCardGameMode->selectBoardCard = NULL;
            coreCardGameMode->RecoverSelectPlayCard();
        }
    }
    else
    {
        AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
        ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
        coreCardGameMode->selectPlayCard = NULL;
        coreCardGameMode->selectBoardCard = NULL;
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
    battleWidget->NotifyInit();
    SwitchMenu("BattleMenu");

    UUserWidget* initSelectCardWidget = CreateWidget(this, selectCardWidgetBPClass);
    selectCardWidget = Cast<USelectCardWidget>(initSelectCardWidget);

    UUserWidget* initReplayMenu = CreateWidget(this, replayMenuBPClass);
    mctReplayMenu = Cast<UMctReplayMenuWidget>(initReplayMenu);
    mctReplayMenu->NotifyInit();
    mctReplayMenu->AddToViewport();
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

void ACoreCardGamePC::TestTriggerSimulation()
{
    AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
    ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
    coreCardGameMode->TestTriggerSimulation();
}

void ACoreCardGamePC::TestTriggerTritonInference()
{
    AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
    ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
    coreCardGameMode->TestTriggerTritonInference();
}

void ACoreCardGamePC::NextMctsTrainNodeMenu()
{
    if (mctReplayMenu)
    {
        mctReplayMenu->NextMctsNodeMenu();
    }
}

