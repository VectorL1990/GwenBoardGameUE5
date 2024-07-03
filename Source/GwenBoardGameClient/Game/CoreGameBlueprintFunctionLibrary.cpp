// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CoreGameBlueprintFunctionLibrary.h"

TArray<FGridXY> UCoreGameBlueprintFunctionLibrary::GetAoeTargetGrids(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    AoeType aoeType,
    FString targetCamp)
{
    int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
    uint8 launchCamp = allInstanceCardInfo[launchUid].camp;

				TArray<FGridXY> modifyGrids;
				if (aoeType == AoeType::H3)
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
    else if (aoeType == AoeType::V3)
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
    else if (aoeType == AoeType::Sweep)
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
    else if (aoeType == AoeType::NormalCross)
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
    else if (aoeType == AoeType::ObliqueCross)
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
    if (effectInfo.effectAffix != "none")
    {

    }
    FEffectResultDict effectResultDict;
    return effectResultDict;
}

