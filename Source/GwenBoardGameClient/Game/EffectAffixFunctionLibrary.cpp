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
    else if (affixType == "RowDiff")
    {
        affixValue = RowDiff(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY);
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

int32 UEffectAffixFunctionLibrary::SameColSameHpNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColSameHpNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameColSameHpNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameColSameHpNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp == allInstanceCardInfo[launchUid].curHp)
            {
                sameColSameHpNb += 1;
            }
        }
    }
    return sameColSameHpNb;
}

int32 UEffectAffixFunctionLibrary::HpDiff(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hpDiff = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
    if (targetUid != -1)
    {
        return 0;
    }
    int32 launchHp = allInstanceCardInfo[launchUid].curHp;
    int32 targetHp = allInstanceCardInfo[targetUid].curHp;
    hpDiff = FMath::Abs(launchHp - targetHp);
    return hpDiff;
}

int32 UEffectAffixFunctionLibrary::HpSum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hpDiff = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
    if (targetUid != -1)
    {
        return 0;
    }
    int32 launchHp = allInstanceCardInfo[launchUid].curHp;
    int32 targetHp = allInstanceCardInfo[targetUid].curHp;
    hpDiff = FMath::Abs(launchHp + targetHp);
    return hpDiff;
}

int32 UEffectAffixFunctionLibrary::SameRowLargerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowLargerHpCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameRowLargerHpCardNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameRowLargerHpCardNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameRowLargerHpCardNb += 1;
            }
        }
    }
}

int32 UEffectAffixFunctionLibrary::SameColLargerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColLargerHpCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameColLargerHpCardNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameColLargerHpCardNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp > allInstanceCardInfo[launchUid].curHp)
            {
                sameColLargerHpCardNb += 1;
            }
        }
    }
}

int32 UEffectAffixFunctionLibrary::SameRowSmallerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowSmallerHpCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSmallerHpCardNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSmallerHpCardNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameRowSmallerHpCardNb += 1;
            }
        }
    }
}

int32 UEffectAffixFunctionLibrary::SameColSmallerHpCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColSmallerHpCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameColSmallerHpCardNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameColSmallerHpCardNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[launchUid].curHp)
            {
                sameColSmallerHpCardNb += 1;
            }
        }
    }
}

int32 UEffectAffixFunctionLibrary::SameRowHpMoreNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    TArray<FString> affixStrs;
    effectInfo.effectAffix.ParseIntoArray(affixStrs, TEXT("="), true);
    if (affixStrs.Num() < 2)
    {
        return 0;
    }

    int32 sameRowHpMoreNb = 0;
    int32 targetHp = FCString::Atoi(*affixStrs[1]);
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > targetHp)
            {
                sameRowHpMoreNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp > targetHp)
            {
                sameRowHpMoreNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curHp > targetHp)
            {
                sameRowHpMoreNb += 1;
            }
        }
    }
}

int32 UEffectAffixFunctionLibrary::SameRowSameDefenceNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowSameDefenceNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameRowSameDefenceNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameRowSameDefenceNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameRowSameDefenceNb += 1;
            }
        }
    }
    return sameRowSameDefenceNb;
}

int32 UEffectAffixFunctionLibrary::SameColSameDefenceNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColSameDefenceNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        if (effectInfo.effectAffixCamp == "self")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameColSameDefenceNb += 1;
            }
        }
        else if (effectInfo.effectAffixCamp == "oppo")
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameColSameDefenceNb += 1;
            }
        }
        else
        {
            if (uid != -1 &&
                allInstanceCardInfo[uid].curDefence == allInstanceCardInfo[launchUid].curDefence)
            {
                sameColSameDefenceNb += 1;
            }
        }
    }
    return sameColSameDefenceNb;
}

int32 UEffectAffixFunctionLibrary::HurtNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hurtNb = 0;
    int32 uid = boardCardInfo[targetY].colCardInfos[targetX];
    hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
    return hurtNb;
}

int32 UEffectAffixFunctionLibrary::HurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
        {
            int32 uid = boardCardInfo[i].colCardInfos[j];
            if (uid == -1)
            {
                continue;
            }

            if (allInstanceCardInfo[uid].curHp < allInstanceCardInfo[uid].originCardInfo.hp)
            {
                hurtCardNb += 1;
            }
        }
    }
    return hurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 selfHurtCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
        {
            if (i == launchY && j == launchX)
            {
                continue;
            }

            int32 uid = boardCardInfo[i].colCardInfos[j];
            if (uid == -1)
            {
                continue;
            }

            if (allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[uid].originCardInfo.hp)
            {
                selfHurtCardNb += 1;
            }
        }
    }
    return selfHurtCardNb;
}


int32 UEffectAffixFunctionLibrary::OppoHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 oppoHurtCardNb = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
        {
            if (i == launchY && j == launchX)
            {
                continue;
            }

            int32 uid = boardCardInfo[i].colCardInfos[j];
            if (uid == -1)
            {
                continue;
            }

            if (allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp &&
                allInstanceCardInfo[uid].curHp < allInstanceCardInfo[uid].originCardInfo.hp)
            {
                oppoHurtCardNb += 1;
            }
        }
    }
    return oppoHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::HurtSum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hurtSum = 0;
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
    if (targetUid == -1)
    {
        return 0;
    }
    int32 launchHurtNb = allInstanceCardInfo[launchUid].originCardInfo.hp - allInstanceCardInfo[launchUid].curHp;
    if (launchHurtNb < 0)
    {
        launchHurtNb = 0;
    }
    int32 targetHurtNb = allInstanceCardInfo[launchUid].originCardInfo.hp - allInstanceCardInfo[launchUid].curHp;
    if (targetHurtNb < 0)
    {
        targetHurtNb = 0;
    }
    hurtSum = launchHurtNb + targetHurtNb;
    return hurtSum;
}

int32 UEffectAffixFunctionLibrary::HurtThreeH3Sum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hurtSum = 0;
    if (targetX > 0)
    {
        int32 leftUid = boardCardInfo[targetY].colCardInfos[targetX - 1];
        int32 leftHurtNb = allInstanceCardInfo[leftUid].curHp - allInstanceCardInfo[leftUid].originCardInfo.hp;
        if (leftHurtNb > 0)
        {
            hurtSum += leftHurtNb;
        }
    }

    if (targetX < UGlobalConstFunctionLibrary::maxCol - 1)
    {
        int32 rightUid = boardCardInfo[targetY].colCardInfos[targetX + 1];
        int32 rightHurtNb = allInstanceCardInfo[rightUid].curHp - allInstanceCardInfo[rightUid].originCardInfo.hp;
        if (rightHurtNb > 0)
        {
            hurtSum += rightHurtNb;
        }
    }

    int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
    int32 hurtNb = allInstanceCardInfo[targetUid].curHp - allInstanceCardInfo[targetUid].originCardInfo.hp;
    if (hurtNb > 0)
    {
        hurtSum += hurtNb;
    }
    return hurtSum;
}

int32 UEffectAffixFunctionLibrary::HurtThreeV3Sum(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 hurtSum = 0;
    if (targetY > 0)
    {
        int32 downUid = boardCardInfo[targetY - 1].colCardInfos[targetX];
        int32 downHurtNb = allInstanceCardInfo[downUid].curHp - allInstanceCardInfo[downUid].originCardInfo.hp;
        if (downHurtNb > 0)
        {
            hurtSum += downHurtNb;
        }
    }

    if (targetY < UGlobalConstFunctionLibrary::maxRow - 1)
    {
        int32 upUid = boardCardInfo[targetY + 1].colCardInfos[targetX];
        int32 upHurtNb = allInstanceCardInfo[upUid].curHp - allInstanceCardInfo[upUid].originCardInfo.hp;
        if (upHurtNb > 0)
        {
            hurtSum += upHurtNb;
        }
    }

    int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
    int32 hurtNb = allInstanceCardInfo[targetUid].curHp - allInstanceCardInfo[targetUid].originCardInfo.hp;
    if (hurtNb > 0)
    {
        hurtSum += hurtNb;
    }

    return hurtSum;
}

int32 UEffectAffixFunctionLibrary::SameRowHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameRowHurtCardNb = 0;
    for (int32 i=0; i<UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0)
        {
            sameRowHurtCardNb += 1;
        }
    }
    return sameRowHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SameColHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 sameColHurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0)
        {
            sameColHurtCardNb += 1;
        }
    }
    return sameColHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SameRowSelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 sameRowHurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0 && allInstanceCardInfo[launchUid].camp == allInstanceCardInfo[uid].camp)
        {
            sameRowHurtCardNb += 1;
        }
    }
    return sameRowHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SameColSelfHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 sameColHurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0 && allInstanceCardInfo[launchUid].camp == allInstanceCardInfo[uid].camp)
        {
            sameColHurtCardNb += 1;
        }
    }
    return sameColHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SameRowOppoHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 sameRowHurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
    {
        int32 uid = boardCardInfo[targetY].colCardInfos[i];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0 && allInstanceCardInfo[launchUid].camp != allInstanceCardInfo[uid].camp)
        {
            sameRowHurtCardNb += 1;
        }
    }
    return sameRowHurtCardNb;
}

int32 UEffectAffixFunctionLibrary::SameColOppoHurtCardNb(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    int32 sameColHurtCardNb = 0;
    for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxRow; i++)
    {
        int32 uid = boardCardInfo[i].colCardInfos[targetX];
        int32 hurtNb = allInstanceCardInfo[uid].originCardInfo.hp - allInstanceCardInfo[uid].curHp;
        if (hurtNb > 0 && allInstanceCardInfo[launchUid].camp != allInstanceCardInfo[uid].camp)
        {
            sameColHurtCardNb += 1;
        }
    }
    return sameColHurtCardNb;
}

void UEffectAffixFunctionLibrary::testFunc1(int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
}
