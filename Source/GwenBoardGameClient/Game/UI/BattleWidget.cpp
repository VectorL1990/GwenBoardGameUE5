// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/BattleWidget.h"
#include "Game/CoreCardGameModeBase.h"

void UBattleWidget::SetupCardDetail()
{

}

void UBattleWidget::ClickButton(FString buttonName)
{
				if (buttonName == "FinishCardSelection")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
								coreCardGameMode->ReqFinishSelectCards();
				}
}

void UBattleWidget::SetFinishCardSelectionText()
{
				finishCardSelectionText->SetText(FText::FromString("Please wait"));
}
