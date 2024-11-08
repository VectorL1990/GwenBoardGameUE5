// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/SelectCardWidget.h"

void USelectCardWidget::Init(UCardDetailWidget* inCardDetailWidget)
{
				cardDetailWidget = inCardDetailWidget;
				cardDetailWidget->SetVisibility(ESlateVisibility::Hidden);
}

void USelectCardWidget::ClickButton(FString buttonName)
{

}

void USelectCardWidget::TriggerClose_Implementation()
{

}
