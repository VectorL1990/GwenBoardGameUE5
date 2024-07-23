// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CoreGameBlueprintFunctionLibrary.h"
#include "EffectAffixFunctionLibrary.h"


void UCoreGameBlueprintFunctionLibrary::Softmax(const TArray<float>& x, float temp, TArray<float>& softmax)
{
    float expSum = 0.0;
    for (int32 i = 0; i < x.Num(); i++)
    {
        float exp = FMath::Exp(x[i] / temp);
        expSum += exp;
    }

    for (int32 i = 0; i < x.Num(); i++)
    {
        softmax[i] = x[i] / expSum;
    }
}


void UCoreGameBlueprintFunctionLibrary::QueryRemotePolicyValue(uint8* boardState, TMap<int32, float>& actionProbs, float& stateValue)
{
    
}

int32 UCoreGameBlueprintFunctionLibrary::GetDirichletAction(const TArray<int32>& actions, const TArray<float>& probs)
{
    std::vector<int> stdActions(actions.Num());
    FMemory::Memcpy(stdActions.data(), actions.GetData(), actions.Num()*sizeof(int));
    std::vector<float> stdProbs(probs.Num());
    FMemory::Memcpy(stdProbs.data(), probs.GetData(), probs.Num() * sizeof(float));

    std::random_device randomDevice;
    std::mt19937 randSeed(randomDevice());


    // dirichlet random
    std::gamma_distribution<float> gamma;
    std::vector<float> dirichletNoises(stdActions.size());
    float sum = 0.0;

    for (int32 i = 0; i < probs.Num(); i++)
    {
        gamma = std::gamma_distribution<float>(probs[i], 1.0);
        dirichletNoises[i] = gamma(randSeed);
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
    
    return stdActions[dist(randSeed)];
}

TArray<FGridXY> UCoreGameBlueprintFunctionLibrary::GetAoeTargetGrids(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
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
        // check right grid
        if (targetY < maxRow - 1 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1)
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
        }
    }
    else if (aoeType == "Sweep")
    {
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
        }
    }
    else if (aoeType == "NormalCross")
    {
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
    }
    else if (aoeType == "ObliqueCross")
    {
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

FEffectResultDict UCoreGameBlueprintFunctionLibrary::IncreaseDefence(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY)
{
    int32 effectValue = 0;
    if (effectInfo.effectAffix != "none")
    {
        effectValue = UEffectAffixFunctionLibrary::GetAffix(
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
        effectValue = effectInfo.values[0];
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
        allInstanceCardInfo[uid].curDefence = allInstanceCardInfo[uid].curDefence + effectValue;
        effectResultDict.modifyGrids.Add(targetGrids[i]);
    }
    return effectResultDict;
}

FEffectResultDict UCoreGameBlueprintFunctionLibrary::ReplaceDefence(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY)
{
    int32 effectValue = 0;
    if (effectInfo.effectAffix != "none")
    {
        effectValue = UEffectAffixFunctionLibrary::GetAffix(
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
        effectValue = effectInfo.values[0];
    }

    FEffectResultDict effectResultDict;
    effectResultDict.modifyType = "replaceDefence";
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
        allInstanceCardInfo[uid].curDefence = allInstanceCardInfo[uid].curDefence + effectValue;
        effectResultDict.modifyGrids.Add(targetGrids[i]);
    }
    return effectResultDict;
}

