// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CoreGameBlueprintFunctionLibrary.h"
#include "CoreCardGamePC.h"
#include "Kismet/GameplayStatics.h"
#include "EffectAffixFunctionLibrary.h"


void UCoreGameBlueprintFunctionLibrary::Softmax(const TArray<float>& x, TArray<float>& softmax)
{
    //TArray<float> Output;
    //if (x.Num() == 0) return Output;

    float maxVal = x[0];
    for (float val : x)
    {
        if (val > maxVal) maxVal = val;
    }

    float sum = 0.0f;
    for (float val : x)
    {
        float expVal = FMath::Exp(val - maxVal);
        softmax.Add(expVal);
        sum += expVal;
    }

    for (int32 i = 0; i < softmax.Num(); i++)
    {
        softmax[i] /= sum;
    }

    //return Output;

    /*
    float expSum = 0.0;
    for (int32 i = 0; i < x.Num(); i++)
    {
        float exp = FMath::Exp(x[i] / temp);
        expSum += exp;
    }

    softmax.Init(0.0, x.Num());
    for (int32 i = 0; i < x.Num(); i++)
    {
        softmax[i] = x[i] / expSum;
    }
    */
}

int32 UCoreGameBlueprintFunctionLibrary::GetRealDirichletAction(const TArray<int32>& actions,
    const TArray<ActionType>& actionTypes,
    const TArray<float>& probs,
    int32& outAction,
    ActionType& outActionType)
{
    return 0;
}


int32 UCoreGameBlueprintFunctionLibrary::GetDirichletAction(
const TArray<int32>& actions,
    const TArray<ActionType>& actionTypes,
    const TArray<float>& probs,
    int32& outAction,
    ActionType& outActionType)
{
    std::vector<int> stdActions(actions.Num());
    FMemory::Memcpy(stdActions.data(), actions.GetData(), actions.Num()*sizeof(int));
    std::vector<float> stdProbs(probs.Num());
    FMemory::Memcpy(stdProbs.data(), probs.GetData(), probs.Num() * sizeof(float));

    std::random_device randomDevice;
    std::mt19937 randSeed(randomDevice());


    // dirichlet random
    //std::normal_distribution<float> gamma;
    std::vector<float> dirichletNoises(stdActions.size());
    float sum = 0.0;

    for (int32 i = 0; i < probs.Num(); i++)
    {
        std::normal_distribution<double> normalDistribution(probs[i], sqrt(probs[i]));
        std::mt19937 gen(std::random_device{}());
        dirichletNoises[i] = normalDistribution(gen);
        sum += dirichletNoises[i];
    }

    for (float& dirichletNoise : dirichletNoises)
    {
        dirichletNoise /= sum;
    }


    std::vector<float> combineProbs(probs.Num());
    // combine orginal probs and noise probs
    for (int32 i = 0; i < probs.Num(); i++)
    {
        combineProbs[i] = 0.75*probs[i] + 0.25*dirichletNoises[i];
    }


    // choose action by discrete distribution
    std::discrete_distribution<int> dist(combineProbs.begin(), combineProbs.end());

    int32 randSeedNb = dist(randSeed);
    outAction = stdActions[dist(randSeed)];
    outActionType = actionTypes[randSeedNb];
    
    return randSeedNb;
}

void UCoreGameBlueprintFunctionLibrary::GetActionDetailFromId(int32 actionId, int32& launchX, int32& launchY, int32& targetX, int32& targetY, ActionType& actionType)
{

}

TArray<FGridXY> UCoreGameBlueprintFunctionLibrary::GetAoeTargetGrids(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    FString aoeType,
    FString targetCamp)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    uint8 launchCamp = allInstanceCardInfo[launchUid].camp;

    TArray<FGridXY> modifyGrids;
    if (aoeType == "H3")
    {
        // check left grid
        if (targetX > 0 && boardCardInfo[targetY].colCardInfos[targetX - 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX - 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX - 1;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }
        // check right grid
        if (targetX < UGlobalConstFunctionLibrary::maxCol - 1 && boardCardInfo[targetY].colCardInfos[targetX + 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX + 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX + 1;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }
        // check central grid
        if (boardCardInfo[targetY].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }
    }
    else if (aoeType == "V3")
    {
        // check left grid
        /*if (targetY > 0 && boardCardInfo[targetY - 1].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY - 1;
                modifyGrids.Add(grid);
            }
        }
        // check right grid
        if (targetY < UGlobalConstFunctionLibrary::graveCardSectionRow maxRow - 1 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX + 1;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }
        // check central grid
        if (boardCardInfo[targetY].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }*/
    }
    else if (aoeType == "Sweep")
    {
        /*
        int32 xOffset = targetX - launchX;
        int32 yOffset = targetY - launchY;
        if (xOffset > 0)
        {
            if (targetX + 1 < maxCol && boardCardInfo[targetY].colCardInfos[targetX + 1] != -1)
            {
                int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX + 1];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX + 1;
                    grid.y = targetY;
                    modifyGrids.Add(grid);
                }
            }

            if (targetX + 2 < maxCol && boardCardInfo[targetY].colCardInfos[targetX + 2] != -1)
            {
                int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX + 2];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX + 2;
                    grid.y = targetY;
                    modifyGrids.Add(grid);
                }
            }
        }
        else if (xOffset < 0)
        {
            if (targetX - 1 < maxCol && boardCardInfo[targetY].colCardInfos[targetX - 1] != -1)
            {
                int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX - 1];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX - 1;
                    grid.y = targetY;
                    modifyGrids.Add(grid);
                }
            }

            if (targetX - 2 < maxCol && boardCardInfo[targetY].colCardInfos[targetX - 2] != -1)
            {
                int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX - 2];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX - 2;
                    grid.y = targetY;
                    modifyGrids.Add(grid);
                }
            }
        }
        else if (yOffset < 0)
        {
            if (targetY - 1 >= 0 && boardCardInfo[targetY - 1].colCardInfos[targetX] != -1)
            {
                int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX;
                    grid.y = targetY - 1;
                    modifyGrids.Add(grid);
                }
            }

            if (targetY - 2 >= 0 && boardCardInfo[targetY - 2].colCardInfos[targetX] != -1)
            {
                int32 targetUid = boardCardInfo[targetY - 2].colCardInfos[targetX];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX;
                    grid.y = targetY - 2;
                    modifyGrids.Add(grid);
                }
            }
        }
        else if (yOffset > 0)
        {
            if (targetY + 1 >= 0 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1)
            {
                int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX;
                    grid.y = targetY + 1;
                    modifyGrids.Add(grid);
                }
            }

            if (targetY + 2 >= 0 && boardCardInfo[targetY + 2].colCardInfos[targetX] != -1)
            {
                int32 targetUid = boardCardInfo[targetY + 2].colCardInfos[targetX];
                if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                    (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                    (targetCamp == "none"))
                {
                    FGridXY grid;
                    grid.x = targetX;
                    grid.y = targetY + 2;
                    modifyGrids.Add(grid);
                }
            }
        }*/
    }
    else if (aoeType == "NormalCross")
    {
        /*
        if (targetX > 0 && boardCardInfo[targetY].colCardInfos[targetX - 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX - 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX - 1;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }

        if (targetX < maxCol - 1 && boardCardInfo[targetY].colCardInfos[targetX + 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX + 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX + 1;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }

        if (targetY > 0 && boardCardInfo[targetY - 1].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY - 1;
                modifyGrids.Add(grid);
            }
        }

        if (targetY < maxRow - 1 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY + 1;
                modifyGrids.Add(grid);
            }
        }
        */
    }
    else if (aoeType == "ObliqueCross")
    {
        /*
        if (targetX - 1 >= 0 && targetY - 1 >= 0 && boardCardInfo[targetY - 1].colCardInfos[targetX - 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX - 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX - 1;
                grid.y = targetY - 1;
                modifyGrids.Add(grid);
            }
        }

        if (targetX + 1 < maxCol && targetY - 1 >= 0 && boardCardInfo[targetY - 1].colCardInfos[targetX + 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX + 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX + 1;
                grid.y = targetY - 1;
                modifyGrids.Add(grid);
            }
        }

        if (targetX + 1 < maxCol && targetY + 1 < maxRow && boardCardInfo[targetY + 1].colCardInfos[targetX + 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX + 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX + 1;
                grid.y = targetY + 1;
                modifyGrids.Add(grid);
            }
        }

        if (targetX - 1 >= 0 && targetY + 1 < maxRow && boardCardInfo[targetY + 1].colCardInfos[targetX - 1] != -1)
        {
            int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX - 1];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX - 1;
                grid.y = targetY + 1;
                modifyGrids.Add(grid);
            }
        }
        */
    }
    else
    {
        if (boardCardInfo[targetY].colCardInfos[targetX] != -1)
        {
            int32 targetUid = boardCardInfo[targetY].colCardInfos[targetX];
            // which means target grid left is not empty
            if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
                (targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
                (targetCamp == "none"))
            {
                FGridXY grid;
                grid.x = targetX;
                grid.y = targetY;
                modifyGrids.Add(grid);
            }
        }
    }

    return modifyGrids;
}



FEffectResultDict UCoreGameBlueprintFunctionLibrary::LaunchRoundEndSkillDict(
    uint8 launchCamp,
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    bool isPotentialVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    TArray<FGridXY> modifyGrids = UCheckTargetGeoRuleLibrary::GetRoundEndAutoSkillTargetGrids(launchCamp,
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        effectInfo.targetGeoType,
        effectInfo.targetCamp);

    FEffectResultDict effectResultDict;
    for (int32 i = 0; i < modifyGrids.Num(); i++)
    {
        int32 targetX = modifyGrids[i].x;
        int32 targetY = modifyGrids[i].y;
        if (effectInfo.effectType == "heal")
        {
            effectResultDict = Heal(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isPotentialVirtual, sectionZeroScore, sectionOneScore);
        }
    }

    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::LaunchPlayCardSkillDict(
    uint8 launchCampNb,
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    TArray<FGridXY> modifyGrids = UCheckTargetGeoRuleLibrary::GetAutoSkillTargetGrids(
        launchCampNb,
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        targetX,
        targetY,
        effectInfo.targetGeoType,
        effectInfo.targetCamp);

    FEffectResultDict effectResultDict;
    for (int32 i = 0; i < modifyGrids.Num(); i++)
    {
        int32 targetX = modifyGrids[i].x;
        int32 targetY = modifyGrids[i].y;
        if (effectInfo.effectType == "hurt")
        {
            effectResultDict = Hurt(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual, sectionZeroScore, sectionZeroScore);
        }
        else if (effectInfo.effectType == "heal")
        {
            effectResultDict = Heal(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual, sectionZeroScore, sectionOneScore);
        }
        else if (effectInfo.effectType == "increaseDefence")
        {
            effectResultDict = IncreaseDefence(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual);
        }
    }
    
    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::LaunchPassiveSkillDict(
    uint8 launchCamp,
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    FString triggerEffectType,
    int32 launchX,
    int32 launchY,
    int32 triggerX,
    int32 triggerY,
    bool isVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    TArray<FGridXY> modifyGrids = UCheckTargetGeoRuleLibrary::GetPassiveSkillTargetGrids(
        launchCamp,
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        triggerX,
        triggerY,
        effectInfo.targetGeoType,
        effectInfo.targetCamp);

    FEffectResultDict effectResultDict;
    for (int32 i = 0; i < modifyGrids.Num(); i++)
    {
        int32 targetX = modifyGrids[i].x;
        int32 targetY = modifyGrids[i].y;
        if (effectInfo.effectType == "revenge")
        {
            if (triggerEffectType == "hurt")
            {
                effectResultDict = Hurt(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual, sectionZeroScore, sectionOneScore);
            }
        }
        else if (effectInfo.effectType == "revengeWound")
        {
            if (triggerEffectType == "hurt")
            {
                effectResultDict = Wound(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual);
            }
        }
        else if (effectInfo.effectType == "repayHeal")
        {
            if (triggerEffectType == "heal")
            {
                effectResultDict = Heal(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isVirtual, sectionZeroScore, sectionOneScore);
            }
        }
    }

    return effectResultDict;
}


FEffectResultDict UCoreGameBlueprintFunctionLibrary::LaunchSkillDict(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isPotentialVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    FEffectResultDict effectResultDict;
    if (effectInfo.effectType == "hurt")
    {
        effectResultDict = Hurt(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isPotentialVirtual, sectionZeroScore, sectionOneScore);
    }
    else if (effectInfo.effectType == "heal")
    {
        effectResultDict = Heal(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isPotentialVirtual, sectionZeroScore, sectionOneScore);
    }
    else if (effectInfo.effectType == "increaseDefence")
    {
        effectResultDict = IncreaseDefence(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, targetX, targetY, isPotentialVirtual);
    }

    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::Hurt(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isPotentialVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    FGetAffixInfo affixInfo;
    if (effectInfo.effectAffix != "none")
    {
        affixInfo = UEffectAffixFunctionLibrary::GetAffix(
            effectInfo.effectAffix,
            allInstanceCardInfo,
            boardCardInfo,
            effectInfo,
            launchX,
            launchY,
            targetX,
            targetY);
    }
    else
    {
        affixInfo.effectValue = effectInfo.values[0];
    }

    FEffectResultDict effectResultDict;
    effectResultDict.modifyType = "hurt";
    effectResultDict.success = true;

    TArray<FGridXY> targetGrids = GetAoeTargetGrids(
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        targetX,
        targetY,
        effectInfo.aoeType,
        effectInfo.targetCamp);

    for (int32 i = 0; i < targetGrids.Num(); i++)
    {
        int32 uid = boardCardInfo[targetGrids[i].y].colCardInfos[targetGrids[i].x];

        if (!isPotentialVirtual)
        {
            if (allInstanceCardInfo[uid].curDefence > 0)
            {
                if (affixInfo.effectValue > allInstanceCardInfo[uid].curDefence)
                {
                    int32 dfcConsumeHurt = affixInfo.effectValue - allInstanceCardInfo[uid].curDefence;
                    allInstanceCardInfo[uid].curDefence = 0;
                    if (allInstanceCardInfo[uid].curHp >= dfcConsumeHurt)
                    {
                        allInstanceCardInfo[uid].curHp = allInstanceCardInfo[uid].curHp - dfcConsumeHurt;
                        if (allInstanceCardInfo[uid].camp == 0)
                        {
                            sectionZeroScore -= dfcConsumeHurt;
                        }
                        else
                        {
                            sectionOneScore -= dfcConsumeHurt;
                        }
                    }
                    else
                    {
                        if (allInstanceCardInfo[uid].camp == 0)
                        {
                            sectionZeroScore -= allInstanceCardInfo[uid].curHp;
                        }
                        else
                        {
                            sectionOneScore -= allInstanceCardInfo[uid].curHp;
                        }
                        allInstanceCardInfo[uid].curHp = 0;
                    }
                }
                else
                {
                    allInstanceCardInfo[uid].curDefence -= affixInfo.effectValue;
                }
            }
            else
            {
                if (allInstanceCardInfo[uid].curHp >= affixInfo.effectValue)
                {
                    allInstanceCardInfo[uid].curHp -= affixInfo.effectValue;
                    if (allInstanceCardInfo[uid].camp == 0)
                    {
                        sectionZeroScore -= affixInfo.effectValue;
                    }
                    else
                    {
                        sectionOneScore -= affixInfo.effectValue;
                    }
                }
                else
                {
                    if (allInstanceCardInfo[uid].camp == 0)
                    {
                        sectionZeroScore -= allInstanceCardInfo[uid].curHp;
                    }
                    else
                    {
                        sectionOneScore -= allInstanceCardInfo[uid].curHp;
                    }
                    allInstanceCardInfo[uid].curHp = 0;
                }
            }

            if (affixInfo.costType == "useSelfDefence")
            {
                int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
                allInstanceCardInfo[launchUid].curDefence -= affixInfo.costValue;
            }
        }
        
        effectResultDict.modifyValues.Add(affixInfo.effectValue);
        effectResultDict.modifyGrids.Add(targetGrids[i]);
        effectResultDict.modifyUids.Add(uid);
    }
    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::Heal(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isPotentialVirtual,
    int32& sectionZeroScore,
    int32& sectionOneScore)
{
    FGetAffixInfo affixInfo;
    if (effectInfo.effectAffix != "none")
    {
        affixInfo = UEffectAffixFunctionLibrary::GetAffix(
            effectInfo.effectAffix,
            allInstanceCardInfo,
            boardCardInfo,
            effectInfo,
            launchX,
            launchY,
            targetX,
            targetY);
    }
    else
    {
        affixInfo.effectValue = effectInfo.values[0];
    }

    FEffectResultDict effectResultDict;
    effectResultDict.modifyType = "heal";
    effectResultDict.success = true;

    TArray<FGridXY> targetGrids = GetAoeTargetGrids(
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        targetX,
        targetY,
        effectInfo.aoeType,
        effectInfo.targetCamp);

    for (int32 i = 0; i < targetGrids.Num(); i++)
    {
        int32 uid = boardCardInfo[targetGrids[i].y].colCardInfos[targetGrids[i].x];

        if (!isPotentialVirtual)
        {
            allInstanceCardInfo[uid].curHp = allInstanceCardInfo[uid].curHp + affixInfo.effectValue;
            if (allInstanceCardInfo[uid].camp == 0)
            {
                sectionZeroScore += affixInfo.effectValue;
            }
            else
            {
                sectionOneScore += affixInfo.effectValue;
            }

            if (affixInfo.costType == "useSelfDefence")
            {
                int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
                allInstanceCardInfo[launchUid].curDefence -= affixInfo.costValue;
            }
        }
        
        effectResultDict.modifyValues.Add(affixInfo.effectValue);
        effectResultDict.modifyGrids.Add(targetGrids[i]);
        effectResultDict.modifyUids.Add(uid);
    }
    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::IncreaseDefence(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isPotentialVirtual)
{
    FGetAffixInfo affixInfo;
    if (effectInfo.effectAffix != "none")
    {
        affixInfo = UEffectAffixFunctionLibrary::GetAffix(
            effectInfo.effectAffix,
            allInstanceCardInfo,
            boardCardInfo,
            effectInfo,
            launchX,
            launchY,
            targetX,
            targetY);
    }
    else
    {
        affixInfo.effectValue = effectInfo.values[0];
    }

    FEffectResultDict effectResultDict;
    effectResultDict.modifyType = "increaseDefence";
    effectResultDict.success = true;

    TArray<FGridXY> targetGrids = GetAoeTargetGrids(
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        targetX,
        targetY,
        effectInfo.aoeType,
        effectInfo.targetCamp);

    for (int32 i = 0; i < targetGrids.Num(); i++)
    {
        int32 uid = boardCardInfo[targetGrids[i].y].colCardInfos[targetGrids[i].x];

        if (!isPotentialVirtual)
        {
            allInstanceCardInfo[uid].curDefence = allInstanceCardInfo[uid].curDefence + affixInfo.effectValue;

            if (affixInfo.costType == "useSelfDefence")
            {
                int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
                allInstanceCardInfo[launchUid].curDefence -= affixInfo.costValue;
            }
        }
        
        effectResultDict.modifyValues.Add(affixInfo.effectValue);
        effectResultDict.modifyGrids.Add(targetGrids[i]);
        effectResultDict.modifyUids.Add(uid);
    }
    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::Wound(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TArray<FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    bool isPotentialVirtual)
{
    FGetAffixInfo affixInfo;
    if (effectInfo.effectAffix != "none")
    {
        affixInfo = UEffectAffixFunctionLibrary::GetAffix(
            effectInfo.effectAffix,
            allInstanceCardInfo,
            boardCardInfo,
            effectInfo,
            launchX,
            launchY,
            targetX,
            targetY);
    }
    else
    {
        affixInfo.effectValue = effectInfo.values[0];
    }

    FEffectResultDict effectResultDict;
    effectResultDict.modifyType = "wound";
    effectResultDict.success = true;

    TArray<FGridXY> targetGrids = GetAoeTargetGrids(
        allInstanceCardInfo,
        boardCardInfo,
        launchX,
        launchY,
        targetX,
        targetY,
        effectInfo.aoeType,
        effectInfo.targetCamp);

    for (int32 i = 0; i < targetGrids.Num(); i++)
    {
        int32 uid = boardCardInfo[targetGrids[i].y].colCardInfos[targetGrids[i].x];

        if (!isPotentialVirtual)
        {
            if (allInstanceCardInfo[uid].curExtraTags.Contains("wound"))
            {
                allInstanceCardInfo[uid].curExtraTags["wound"] = allInstanceCardInfo[uid].curExtraTags["wound"] + affixInfo.effectValue;
            }
            else
            {
                allInstanceCardInfo[uid].curExtraTags.Add("wound", affixInfo.effectValue);
            }

            if (affixInfo.costType == "useSelfDefence")
            {
                int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
                allInstanceCardInfo[launchUid].curDefence -= affixInfo.costValue;
            }
        }

        effectResultDict.modifyValues.Add(affixInfo.effectValue);
        effectResultDict.modifyGrids.Add(targetGrids[i]);
        effectResultDict.modifyUids.Add(uid);
    }
    return effectResultDict;
}


void UCoreGameBlueprintFunctionLibrary::ConvertStateToJson(
    const TArray<FString>& stateArray,
    const int32 col,
    const int32 row)
{
    TSharedRef<FJsonObject> jsonObj = MakeShareable(new FJsonObject);

    TArray<TSharedPtr<FJsonValue>> stateJsonStrings;
    for (int32 i = 0; i < stateArray.Num(); i++)
    {
        stateJsonStrings.Add(MakeShareable(new FJsonValueString(stateArray[i])));
    }

    jsonObj->SetArrayField("state", stateJsonStrings);
    jsonObj->SetNumberField("col", col);
    jsonObj->SetNumberField("row", row);
}

bool UCoreGameBlueprintFunctionLibrary::WriteStringToFile(const FString& fileName, const FString& relativePath, const FString& writeString)
{
    FString absolutePath = FPaths::ProjectContentDir() + relativePath + fileName;

    if (FFileHelper::SaveStringToFile(writeString, *absolutePath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), EFileWrite::FILEWRITE_Append))
    {
        return true;
    }
    else
    {
        return false;
    }
}

