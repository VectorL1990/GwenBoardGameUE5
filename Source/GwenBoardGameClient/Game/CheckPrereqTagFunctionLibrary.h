// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheckPrereqTagFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckPrereqTagFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static bool CheckPrereqTagRule(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY);
};
