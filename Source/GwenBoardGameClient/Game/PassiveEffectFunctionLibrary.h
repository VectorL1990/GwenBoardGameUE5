// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "PassiveEffectFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UPassiveEffectFunctionLibrary : public UBlueprintFunctionLibrary
{
				GENERATED_BODY()
public:
    static FEffectResultDict GetPassiveEffect(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TMap<int32, FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        TArray<int32>& triggeredUids);
};
