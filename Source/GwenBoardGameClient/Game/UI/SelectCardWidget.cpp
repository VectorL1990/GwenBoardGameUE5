// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/SelectCardWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Base/GwenBoardGameInstance.h"
#include "../CoreCardGameModeBase.h"
#include "Game/GlobalConstFunctionLibrary.h"

void USelectCardWidget::Init(UCardDetailWidget* inCardDetailWidget)
{
				cardDetailWidget = inCardDetailWidget;
				cardDetailWidget->SetVisibility(ESlateVisibility::Hidden);
}

void USelectCardWidget::RegisterSelectCard(UCardWidget* inCardWidget)
{
				if (inCardWidget)
				{
								selectedCards.Add(inCardWidget);
				}
}

void USelectCardWidget::ClickButton(FString buttonName)
{

}

void USelectCardWidget::TriggerClose_Implementation()
{

}

void USelectCardWidget::GenerateSelectCards()
{
				UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
				UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);

				AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
				ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);

				TArray<int32> cardNbList;
				if (gwenGI->playerSectionNb == 0)
				{
								for (int32 i = 0; i < gwenGI->sectionZeroPileCards.Num(); i++)
								{
												cardNbList.Add(i);
								}
				}
				else
				{
								for (int32 i = 0; i < gwenGI->sectionOnePileCards.Num(); i++)
								{
												cardNbList.Add(i);
								}
				}
				

				TArray<int32> selectCardNbList;
				for (int32 i = 0; i < selectedCards.Num(); i++)
				{
								int32 randCardNb = FMath::RandRange(0, cardNbList.Num() - 1);
								selectCardNbList.Add(cardNbList[randCardNb]);
								cardNbList.RemoveAt(randCardNb);
				}

				for (int32 i = 0; i < selectCardNbList.Num(); i++)
				{
								FString selectCardName;
								if (gwenGI->playerSectionNb == 0)
								{
												selectCardName = gwenGI->sectionZeroPileCards[selectCardNbList[i]];
								}
								else
								{
												selectCardName = gwenGI->sectionOnePileCards[selectCardNbList[i]];
								}
								if (gwenGI->allCardInfos.Contains(selectCardName))
								{
												if (selectedCards[i]->texture)
												{
																selectedCards[i]->SetupCardInfo(gwenGI->allCardInfos[selectCardName]);
																UMaterialInstanceDynamic* mi = Cast<UMaterialInstanceDynamic>(selectedCards[i]->cardButton->WidgetStyle.Normal.GetResourceObject());
																mi->SetTextureParameterValue("", selectedCards[i]->texture);
												}
								}
				}
}

