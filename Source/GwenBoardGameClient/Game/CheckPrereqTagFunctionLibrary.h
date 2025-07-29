// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "CheckPrereqTagFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckPrereqTagFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static bool CheckLaunchPrereqTagRule(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY);

	static bool CheckLaunchNextToTag(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		FString prereqTag);

	static bool CheckLaunchSelfHasTag(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		FString prereqTag);



	static bool CheckTargetPrereqTagRule(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		FEffectInfo& effectInfo,
		int32 targetX,
		int32 targetY);

	static bool CheckTargetHasTag(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 targetX,
		int32 targetY,
		FString prereqTag);
};
