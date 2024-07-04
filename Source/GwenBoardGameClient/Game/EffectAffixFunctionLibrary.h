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
    static int32 GetAffix(
        FString affixType,
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TMap<int32, FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);

    static int32 ColDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 RowDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 DistDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowSelfCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColSelfCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameRowOppoCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static int32 SameColOppoCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    static void testFunc1(int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void (*FuncPtr)(int32 launchX, int32 launchY, int32 targetX, int32 targetY) = testFunc1;
};
