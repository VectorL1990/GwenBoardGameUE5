// Fill out your copyright notice in the Description page of Project Settings.


#include "CardWidget3D.h"

void UCardWidget3D::NotifyInit_Implementation()
{

}

void UCardWidget3D::InitWidget(UTextBlock* inCurHpText, UTextBlock* inCurDefenceText)
{
	curHpText = inCurHpText;
	curDefenceText = inCurDefenceText;
}

void UCardWidget3D::SetupHpDefence(int32 inCurHp, int32 inCurDefence)
{
	curHpText->SetText(FText::FromString(FString::FromInt(inCurHp)));
	curDefenceText->SetText(FText::FromString(FString::FromInt(inCurDefence)));
}
