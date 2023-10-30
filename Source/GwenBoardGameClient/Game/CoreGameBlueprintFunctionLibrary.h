// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreGameBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class BattleCardStatus : uint8
{
	Standby = 0,
	InBattle = 1,
	InGrave,
	UnUsed,
};

UENUM(BlueprintType)
enum class BattleStatus : uint8
{
	Default = 0,
	BattleCountDown = 1,
	SwitchControllerAnimationCounting = 2,
	
};

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCoreGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
