// Fill out your copyright notice in the Description page of Project Settings.


#include "CardWidget3D.h"

void UCardWidget3D::NotifyInit_Implementation()
{

}

void UCardWidget3D::InitWidget(
	UTextBlock* inCurHpText, 
	UTextBlock* inCurDefenceText, 
	UTextBlock* inCurCdText,
	UTextBlock* inCurAvailableText)
{
	curHpText = inCurHpText;
	curDefenceText = inCurDefenceText;
	curCdText = inCurCdText;
	curAvailableText = inCurAvailableText;
}

void UCardWidget3D::SetupHpDefence(int32 inCurHp, int32 inCurDefence, int32 inCurCd, int32 inCurAvailable)
{
	curHpText->SetText(FText::FromString(FString::FromInt(inCurHp)));
	curDefenceText->SetText(FText::FromString(FString::FromInt(inCurDefence)));
	curCdText->SetText(FText::FromString(FString::FromInt(inCurCd)));
	curAvailableText->SetText(FText::FromString(FString::FromInt(inCurAvailable)));
}
