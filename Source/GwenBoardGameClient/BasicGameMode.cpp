// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Base/GwenBoardGameInstance.h"

void ABasicGameMode::CallGameInstanceSendData()
{
				UGwenBoardGameInstance* gi = Cast<UGwenBoardGameInstance>(GetWorld()->GetGameInstance());
				
}