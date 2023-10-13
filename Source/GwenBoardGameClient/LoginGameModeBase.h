// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.h"
#include "LoginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ALoginGameModeBase : public ABasicGameMode
{
	GENERATED_BODY()
	public:

	UFUNCTION(BlueprintCallable)
	void RequestSignup(FString packMsg);
	
};
