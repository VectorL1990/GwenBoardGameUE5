// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "EffectAffixFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UEffectAffixFunctionLibrary : public UBlueprintFunctionLibrary
{
				GENERATED_BODY()
public:
    static FGetAffixInfo GetAffix(
        FString affixType,
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 ColDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 RowDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 DistDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameLaunchRowNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameTargetRowNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameLaunchColNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameTargetColNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 SameLaunchRowSelfCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameTargetRowSelfCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameLaunchColSelfCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameTargetColSelfCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 SameLaunchRowOppoCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 SameTargetRowOppoCampNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 SameColOppoCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowSameHpNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColSameHpNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HpDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HpSum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowLargerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColLargerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowSmallerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColSmallerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowHpMoreNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowSameDefenceNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColSameDefenceNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HurtNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 OppoHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HurtSum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HurtThreeH3Sum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 HurtThreeV3Sum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowSelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColSelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowOppoHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColOppoHurtCardNb(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
        TArray<FBoardRow>& boardCardInfo, 
        FEffectInfo& effectInfo, 
        int32 launchX, 
        int32 launchY, 
        int32 targetX, 
        int32 targetY);

    static int32 UseSelfDefence_2(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 HurtVal(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static void testFunc1(int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void (*FuncPtr)(int32 launchX, int32 launchY, int32 targetX, int32 targetY) = testFunc1;
};
