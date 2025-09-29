// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "CheckLaunchGeoLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckLaunchGeoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static bool CheckLaunchGeoType(FString geoType,
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX, int32 launchY);

	static bool CheckSelfSection(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX, int32 launchY);

	static bool CheckHorn(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX, int32 launchY);

	static bool CheckThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX, int32 launchY);
};
