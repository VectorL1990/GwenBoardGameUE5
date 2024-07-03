// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EffectAffixFunctionLibrary.h"

int32 UEffectAffixFunctionLibrary::GetAffix(
    FString affixType,
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY)
{
    int32 affixValue = 0;
    if (affixType == "ColDiff")
    {
        affixValue = ColDiff(allInstanceCardInfo, boardCardInfo, launchX, launchY, targetX, targetY);
    }
}

int32 UEffectAffixFunctionLibrary::ColDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 colDiff = FMath::Abs(targetX - launchX);
    return colDiff;
}

int32 UEffectAffixFunctionLibrary::RowDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 rowDiff = FMath::Abs(targetY - launchY);
    return rowDiff;
}

int32 UEffectAffixFunctionLibrary::DistDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 colDiff = FMath::Abs(targetX - launchX);
    int32 rowDiff = FMath::Abs(targetY - launchY);
    return colDiff + rowDiff;
}

int32 UEffectAffixFunctionLibrary::SameRowNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];

    }
}

void UEffectAffixFunctionLibrary::testFunc1(int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
}
