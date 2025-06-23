// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MctWValueElement.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UMctWValueElement : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void NotifyInit();

	UFUNCTION(BlueprintCallable)
	void Init(UTextBlock* inWText);

	UFUNCTION(BlueprintNativeEvent)
	void NotifySetText(int32 inActionId, uint8 inLaunchSection, float inW, float inQU);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* wText;
};
