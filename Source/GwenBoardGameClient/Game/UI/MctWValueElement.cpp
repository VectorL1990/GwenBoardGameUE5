// Fill out your copyright notice in the Description page of Project Settings.


#include "MctWValueElement.h"

void UMctWValueElement::NotifyInit_Implementation()
{

}

void UMctWValueElement::Init(UTextBlock* inWText)
{
	wText = inWText;
}

void UMctWValueElement::NotifySetText_Implementation(int32 inActionId, uint8 inLaunchSection, float inW, float inQU)
{

}


