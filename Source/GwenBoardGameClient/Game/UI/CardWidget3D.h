// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CardWidget3D.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCardWidget3D : public UUserWidget
{
	GENERATED_BODY()
public:
	UTextBlock* curHpText;

	UTextBlock* curDefenceText;

	UFUNCTION(BlueprintNativeEvent)
	void NotifyInit();

	UFUNCTION(BlueprintCallable)
	void InitWidget(UTextBlock* inCurHpText, UTextBlock* inCurDefenceText);

	UFUNCTION(BlueprintCallable)
	void SetupHpDefence(int32 inCurHp, int32 inCurDefence);
};
