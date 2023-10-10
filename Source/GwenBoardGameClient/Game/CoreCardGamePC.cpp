// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGamePC.h"
#include "../CoreGameManager.h"
#include "../Base/GwenBoardGameInstance.h"

void ACoreCardGamePC::BeginPlay()
{
				SetShowMouseCursor(true);
				UGwenBoardGameInstance* gwenBoardGameInstance = Cast<UGwenBoardGameInstance>(GetGameInstance());
				if (gwenBoardGameInstance)
				{
								gwenBoardGameInstance->StartSkynetClientThread();
				}
}

void ACoreCardGamePC::Tick(float DeltaTime)
{
				
}

void ACoreCardGamePC::DealLeftClick()
{
				FHitResult hitResult;
				bool hitSomething = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, hitResult);
				if (hitSomething && hitResult.bBlockingHit)
				{
								if (hitResult.GetActor()->Tags.Contains("Card"))
								{
												
								}
				}
}

