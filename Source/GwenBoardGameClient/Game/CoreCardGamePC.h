// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoreCardGameManager.h"
#include "CoreCardGamePC.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGamePC : public APlayerController
{
				GENERATED_BODY()
public:
				UFUNCTION(BlueprintCallable)
				void DealLeftClick();

				UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
				ACoreCardGameManager* coreCardGameManager;

protected:
				virtual void BeginPlay() override;

				virtual void Tick(float DeltaTime) override;
};
