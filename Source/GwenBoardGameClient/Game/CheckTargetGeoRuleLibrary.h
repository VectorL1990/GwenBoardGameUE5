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
	static TArray<FGridXY> GetPossibleMoveGrids(FString rule, 
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, EAtkDistanceType distance);

	static TArray<FGridXY> GetPossibleMoveLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, 
		int32 launchX, int32 launchY, EAtkDistanceType distance);

	static TArray<FGridXY> GetPossibleMoveSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, 
		int32 launchX, int32 launchY, EAtkDistanceType distance);

	static TArray<FGridXY> GetPossibleMoveDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, 
		int32 launchX, int32 launchY, EAtkDistanceType distance);


	static TArray<FGridXY> GetPossibleTargetGeoGrids(FString geoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType);

	static TArray<FGridXY> GetPossibleLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType);

	static TArray<FGridXY> GetPossibleSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType);

	static TArray<FGridXY> GetPossibleDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType);

	static TArray<FGridXY> GetPossibleNext(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType);


	static TArray<FGridXY> GetAutoSkillTargetGrids(
		uint8 launchCamp,
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		FString tagetGeoType,
		FString targetCamp);

	static TArray<FGridXY> GetRoundEndAutoSkillTargetGrids(
		uint8 launchCamp,
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX,
		int32 launchY,
		FString targetGeoType,
		FString targetCamp);

	static TArray<FGridXY> GetPassiveSkillTargetGrids(
		uint8 launchCamp,
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardRows,
		int32 launchX,
		int32 launchY,
		int32 triggerX,
		int32 triggerY,
		FString targetGeoType,
		FString targetCamp);




	static void CheckPossibleTargetLocateGeoGrids(FString locateGeoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids);

	static void CheckPossibleTargetLocateThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
		TArray<FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids);



	static bool CheckTargetSkillGeoLegality(
		const FEffectInfo& effectInfo,
		TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
		TArray<FBoardRow>& boardCardInfo,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY);
};
