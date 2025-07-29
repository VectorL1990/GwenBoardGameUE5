// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "CheckPrereqFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckPrereqFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static bool CheckPrereqRule(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		FString prereq, 
		int32 launchX, 
		int32 launchY, 
		int32 launchCamp, 
		FString prereqCampType, 
		int32 prereqValue);
	
	static bool CheckSelfSameRowDefenceMore(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows, 
		int32 launchX, 
		int32 launchY, 
		int32 launchCamp, 
		int32 prereqValue, 
		FString prereqCampType);

	static bool CheckOppoSameRowDefenceMore(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		int32 launchCamp,
		int32 prereqValue,
		FString prereqCampType);

	static bool CheckSameRowDefenceMore(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		int32 launchCamp,
		int32 prereqValue,
		FString prereqCampType);

	static bool CheckMaxDefenceIsSelf(
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		int32 launchCamp,
		int32 prereqValue,
		FString prereqCampType);
};
