// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/BattleWidget.h"
#include "CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Game/CoreCardGameModeBase.h"


void UBattleWidget::Init(UCardDetailWidget* inCardDetailWidget)
{
	cardDetailWidget = inCardDetailWidget;
	cardDetailWidget->NotifyInit();
	//HideCardDetail();
}

void UBattleWidget::NotifyInit_Implementation()
{

}

void UBattleWidget::SetupCardDetail(FVector cardWorldPose, FString cardName)
{
	cardDetailWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	cardDetailWidget->SetupDetailDescription(cardName);
	FVector2D cardScreenPose;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), cardWorldPose, cardScreenPose);
	float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	UCanvasPanelSlot* cardDetailSlot = Cast<UCanvasPanelSlot>(cardDetailWidget->Slot);
	cardDetailSlot->SetPosition(cardScreenPose / viewportScale);
	cardDetailWidget->TriggerShowWidget();
}

void UBattleWidget::HideCardDetail()
{
	cardDetailWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UBattleWidget::ClickButton(FString buttonName)
{
	if (buttonName == "FinishCardSelection")
	{
		AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
		ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
		coreCardGameMode->ReqFinishSelectCards();
	}
	else if (buttonName == "EndRound")
	{
		AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
		ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
		coreCardGameMode->TestTriggerAction(0, 0, 0, 0, 0, ActionType::EndRound);
	}
}

void UBattleWidget::SetFinishCardSelectionText()
{
	finishCardSelectionText->SetText(FText::FromString("Please wait"));
}
