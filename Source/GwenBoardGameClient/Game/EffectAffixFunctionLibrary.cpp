// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EffectAffixFunctionLibrary.h"

int32 UEffectAffixFunctionLibrary::GetAffix(
    FString affixType,
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY)
{
    int32 affixValue = 0;
    if (affixType == "ColDiff")
    {
        affixValue = ColDiff(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY);
    }
    return affixValue;
}

int32 UEffectAffixFunctionLibrary::ColDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 colDiff = FMath::Abs(targetX - launchX);
    return colDiff;
}

int32 UEffectAffixFunctionLibrary::RowDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 rowDiff = FMath::Abs(targetY - launchY);
    return rowDiff;
}

int32 UEffectAffixFunctionLibrary::DistDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 colDiff = FMath::Abs(targetX - launchX);
    int32 rowDiff = FMath::Abs(targetY - launchY);
    return colDiff + rowDiff;
}

int32 UEffectAffixFunctionLibrary::SameRowNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (uid != -1)
        {
            /*
            if ((effectInfo.effectAffixCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
                (effectInfo.effectAffixCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
                effectInfo.effectAffixCamp == "none")
            {
                
            }*/
            sameRowNb += 1;
        }
    }
    return sameRowNb;
}

int32 UEffectAffixFunctionLibrary::SameColNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (uid != -1)
        {
            sameColNb += 1;
        }
    }
    return sameColNb;
}

int32 UEffectAffixFunctionLibrary::SameRowSelfCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (uid != -1 && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp)
        {
            sameRowNb += 1;
        }
    }
    return sameRowNb;
}

int32 UEffectAffixFunctionLibrary::SameColSelfCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (uid != -1 && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp)
        {
            sameColNb += 1;
        }
    }
    return sameColNb;
}

int32 UEffectAffixFunctionLibrary::SameRowOppoCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (uid != -1 && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp)
        {
            sameRowNb += 1;
        }
    }
    return sameRowNb;
}

int32 UEffectAffixFunctionLibrary::SameColOppoCampNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (uid != -1 && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp)
        {
            sameColNb += 1;
        }
    }
    return sameColNb;
}

int32 UEffectAffixFunctionLibrary::SameRowSameHpNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowSameHpNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSameHpNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSameHpNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSameHpNb += 1;
            }
        }
    }
    return sameRowSameHpNb;
}

void UEffectAffixFunctionLibrary::testFunc1(int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
}
