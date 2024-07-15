// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "CheckTargetGeoRuleLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckTargetGeoRuleLibrary : public UBlueprintFunctionLibrary
{
				GENERATED_BODY()
public:
				static TArray<FGridXY> Line(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance);
	
};
