// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ABasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
				UFUNCTION(BlueprintCallable)
				void CallGameInstanceSendData(FString sendData);
	
};
