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
				static TArray<FGridXY> GetPossibleMoveGrids(FString rule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleMoveLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleMoveSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleMoveDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance);


				static TArray<FGridXY> GetPossibleTargetGeoGrids(FString geoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance);

				static TArray<FGridXY> GetPossibleDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance);




				static void CheckPossibleTargetLocateGeoGrids(FString locateGeoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids);

				static void CheckPossibleTargetLocateThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids);
};
