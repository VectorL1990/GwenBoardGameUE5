// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/CardWidget.h"
#include "CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../BasicPlayerController.h"
#include "Kismet/KismetMaterialLibrary.h"


void UCardWidget::Init(UButton* inButton, UCanvasPanel* inCanvas)
{
				if (inCanvas)
				{
								canvasPanel = inCanvas;
				}

				if (inButton)
				{
								APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
								ABasicPlayerController* basicPlayerController = Cast<ABasicPlayerController>(playerController);
								UMaterialInstanceDynamic* mi = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, parentMat);
								FSlateBrush normalBrush;
								FSlateBrush hoverBrush;
								FSlateBrush pressBrush;
								normalBrush.TintColor = basicPlayerController->buttonNormalColor;
								hoverBrush.TintColor = basicPlayerController->buttonHoverColor;
								pressBrush.TintColor = basicPlayerController->buttonPressColor;
								normalBrush.SetResourceObject(mi);
								hoverBrush.SetResourceObject(mi);
								pressBrush.SetResourceObject(mi);
								cardButton = inButton;
								cardButton->WidgetStyle.Normal = normalBrush;
								cardButton->WidgetStyle.Hovered = hoverBrush;
								cardButton->WidgetStyle.Pressed = pressBrush;
				}
				
}

void UCardWidget::ClickButton(FString buttonName)
{
				if (battleCardWidgetType == BattleCardWidgetType::BattleSelectCard)
				{
								if (buttonName == "ClickSelectCard")
								{
												// In select card widget, 
								}
				}
}

void UCardWidget::Hover()
{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ABasicPlayerController* basicPlayerController = Cast<ABasicPlayerController>(playerController);
				FWidgetTransform widgetTransform;
				widgetTransform.Scale *= basicPlayerController->hoverAmplify;
				SetRenderTransform(widgetTransform);

				basicPlayerController->ShowCardDetail(this);
}

void UCardWidget::UnHover()
{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ABasicPlayerController* basicPlayerController = Cast<ABasicPlayerController>(playerController);
				FWidgetTransform widgetTransform;
				widgetTransform.Scale = FVector2D(1.0, 1.0);
				SetRenderTransform(widgetTransform);

				basicPlayerController->HideCardDetail(this);
}
