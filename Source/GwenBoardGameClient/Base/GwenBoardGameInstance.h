// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GwenBoardGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UGwenBoardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
				void StartSkynetClientThread();

				void CallLuaRead();

				FRunnableThread* skynetRunnableThread;
};
