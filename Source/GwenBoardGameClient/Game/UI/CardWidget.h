// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CoreGameBlueprintFunctionLibrary.h"
#include "Image.h"
#include "Button.h"
#include "CanvasPanel.h"
#include "CardWidget.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCardWidget : public UUserWidget
{
				GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    BattleCardWidgetType battleCardWidgetType;

    UPROPERTY(EditDefaultsOnly)
    UMaterialInterface* parentMat;

    UPROPERTY()
    UCanvasPanel* canvasPanel;

    UPROPERTY()
    UButton* cardButton;

    UPROPERTY()
    UTexture* cardTexture;

    UPROPERTY(EditDefaultsOnly)
    FVector2D detailPanelOffset = FVector2D(100.0, -170.0);

    UFUNCTION(BlueprintCallable)
    void Init(UButton* inButton, UCanvasPanel* inCanvas);

    UFUNCTION(BlueprintCallable)
    void ClickButton(FString buttonName);

    UFUNCTION(BlueprintCallable)
    void Hover();

    UFUNCTION(BlueprintCallable)
    void UnHover();
};
