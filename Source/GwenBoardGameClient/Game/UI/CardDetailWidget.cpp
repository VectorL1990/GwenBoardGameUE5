// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/CardDetailWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../Base/GwenBoardGameInstance.h"

void UCardDetailWidget::NotifyInit_Implementation()
{

}

void UCardDetailWidget::TriggerShowWidget_Implementation()
{

}

void UCardDetailWidget::SetupDetailDescription(FString cardName)
{
	UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
	UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
	if (gwenGI->allCardInfos.Contains(cardName))
	{
		descriptionTextBlock->SetText(FText::FromString(gwenGI->allCardInfos[cardName].cardFunctionCN));
	}
	else
	{
		descriptionTextBlock->SetText(FText::FromString(""));
	}
}
