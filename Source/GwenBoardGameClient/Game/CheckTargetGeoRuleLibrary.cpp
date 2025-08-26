// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckTargetGeoRuleLibrary.h"


TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveGrids(FString rule, 
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, EAtkDistanceType distance)
{
	TArray<FGridXY> possibleGrids;
	if (rule == "line")
	{
		possibleGrids = GetPossibleMoveLine(allInstanceCardInfo,
			boardCardInfo,
			launchX,
			launchY,
			distance);
	}
	else if (rule == "seperate")
	{
		possibleGrids = GetPossibleMoveSeperate(allInstanceCardInfo,
			boardCardInfo,
			launchX,
			launchY,
			distance);
	}
	else if (rule == "diagonal")
	{
		possibleGrids = GetPossibleMoveDiagonal(allInstanceCardInfo,
			boardCardInfo,
			launchX,
			launchY,
			distance);
	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, 
	int32 launchX, 
	int32 launchY, 
	EAtkDistanceType distance)
{
	TArray<FGridXY> possibleGrids;
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	bool isRightBlock = false;
	bool isLeftBlock = false;
	bool isUpBlock = false;
	bool isDownBlock = false;
	int32 rightStep = 1;
	while (launchX + rightStep <= UGlobalConstFunctionLibrary::maxCol - 1)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[launchX + rightStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX + rightStep;
			grid.y = launchY;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		rightStep += 1;
	}

	int32 leftStep = 1;
	while (launchX - leftStep >= 0)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[launchX - leftStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX - leftStep;
			grid.y = launchY;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		leftStep += 1;
	}

	int32 upStep = 1;
	while (launchY + upStep <=
		UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + upStep].colCardInfos[launchX];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX;
			grid.y = launchY + upStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		upStep += 1;
	}

	int32 downStep = 1;
	while (launchY - downStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - downStep].colCardInfos[launchX];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX;
			grid.y = launchY - downStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		downStep += 1;
	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, 
	int32 launchX, int32 launchY, EAtkDistanceType distance)
{
	TArray<FGridXY> possibleGrids;
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];

	bool isRightBarbetteSet = false;
	bool isRightBlock = false;

	bool isLeftBarbetteSet = false;
	bool isLeftBlock = false;

	bool isUpBarbetteSet = false;
	bool isUpBlock = false;

	bool isDownBarbetteSet = false;
	bool isDownBlock = false;
	for (int32 i = launchX + 1; i < UGlobalConstFunctionLibrary::maxCol; i++)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[i];
		if (!isRightBarbetteSet)
		{
			if (uid != -1)
			{
				isRightBarbetteSet = true;
			}
		}
		else
		{
			if (uid == -1)
			{
				FGridXY grid;
				grid.x = i;
				grid.y = launchY;
				possibleGrids.Add(grid);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 i = launchX - 1; i >= 0; i--)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[i];
		if (!isLeftBarbetteSet)
		{
			if (uid != -1)
			{
				isLeftBarbetteSet = true;
			}
		}
		else
		{
			if (uid == -1)
			{
				FGridXY grid;
				grid.x = i;
				grid.y = launchY;
				possibleGrids.Add(grid);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 i = launchY + 1; i < UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow; i++)
	{
		int32 uid = boardCardInfo[i].colCardInfos[launchX];
		if (!isUpBarbetteSet)
		{
			if (uid != -1)
			{
				isUpBarbetteSet = true;
			}
		}
		else
		{
			if (uid == -1)
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = i;
				possibleGrids.Add(grid);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 i = launchY - 1; i >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow; i--)
	{
		int32 uid = boardCardInfo[i].colCardInfos[launchX];
		if (!isDownBarbetteSet)
		{
			if (uid != -1)
			{
				isDownBarbetteSet = true;
			}
		}
		else
		{
			if (uid == -1)
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = i;
				possibleGrids.Add(grid);
			}
			else
			{
				break;
			}
		}
	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, 
	int32 launchX, int32 launchY, EAtkDistanceType distance)
{
	TArray<FGridXY> possibleGrids;
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	bool isLeftUpBlock = false;
	bool isRightUpBlock = false;
	bool isLeftDownBlock = false;
	bool isRightDownBlock = false;
	int32 leftUpStep = 1;
	while (launchX - leftUpStep >= 0 &&
		launchY + leftUpStep <= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + leftUpStep].colCardInfos[launchX - leftUpStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX - leftUpStep;
			grid.y = launchY + leftUpStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		leftUpStep += 1;
	}

	int32 rightUpStep = 1;
	while (launchX + rightUpStep <= UGlobalConstFunctionLibrary::maxCol - 1 &&
		launchY + rightUpStep <= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + rightUpStep].colCardInfos[launchX + rightUpStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX + rightUpStep;
			grid.y = launchY + rightUpStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		rightUpStep += 1;
	}

	int32 leftDownStep = 1;
	while (launchX - leftDownStep >= 0 &&
		launchY - leftDownStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - leftDownStep].colCardInfos[launchX - leftDownStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX - leftDownStep;
			grid.y = launchY - leftDownStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		leftDownStep += 1;
	}

	int32 rightDownStep = 1;
	while (launchX + rightDownStep <= UGlobalConstFunctionLibrary::maxCol - 1 &&
		launchY - rightDownStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - rightDownStep].colCardInfos[launchX + rightDownStep];
		if (uid == -1)
		{
			FGridXY grid;
			grid.x = launchX + rightDownStep;
			grid.y = launchY - rightDownStep;
			possibleGrids.Add(grid);
		}
		else
		{
			break;
		}

		if (distance == EAtkDistanceType::Closed)
		{
			break;
		}
		rightDownStep += 1;
	}
	return possibleGrids;
}




TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(FString geoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType)
{
	TArray<FGridXY> possibleGrids;
	if (geoRule == "line")
	{
		possibleGrids = GetPossibleLine(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, distanceType);
	}
	else if (geoRule == "seperate")
	{
		possibleGrids = GetPossibleSeperate(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, distanceType);
	}
	else if (geoRule == "diagonal")
	{
		possibleGrids = GetPossibleDiagonal(allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY, distanceType);
	}
	else if (geoRule == "self")
	{
		FGridXY grid;
		grid.x = launchX;
		grid.y = launchY;
		possibleGrids.Add(grid);
	}
	else if (geoRule == "left")
	{
		int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
		uint8 launchCamp = allInstanceCardInfo[launchUid].camp;
		if (launchCamp == 0)
		{
			if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX + 1];
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX - 1;
					grid.y = launchY;
					possibleGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchX > 0)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX - 1];
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX + 1;
					grid.y = launchY;
					possibleGrids.Add(grid);
				}
			}
		}
	}
	else if (geoRule == "right")
	{
		int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
		uint8 launchCamp = allInstanceCardInfo[launchUid].camp;
		if (launchCamp == 0)
		{
			if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX + 1];
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX + 1;
					grid.y = launchY;
					possibleGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchX > 0)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX - 1];
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX - 1;
					grid.y = launchY;
					possibleGrids.Add(grid);
				}
			}
		}
	}
	else if (geoRule == "forward")
	{
		int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
		uint8 launchCamp = allInstanceCardInfo[launchUid].camp;
		if (launchCamp == 0)
		{
			if (launchY < (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow - 1))
			{
				int32 targetUid = boardCardInfo[launchY + 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY + 1;
					possibleGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchY > (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow))
			{
				int32 targetUid = boardCardInfo[launchY - 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(allInstanceCardInfo[launchUid].originCardInfo.targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY - 1;
					possibleGrids.Add(grid);
				}
			}
		}
	}
	else if (geoRule == "backward")
	{

	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType)
{
	TArray<FGridXY> possibleGrids;
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	bool isRightBlock = false;
	bool isLeftBlock = false;
	bool isUpBlock = false;
	bool isDownBlock = false;
	int32 rightStep = 1;
	while (launchX + rightStep <= UGlobalConstFunctionLibrary::maxCol - 1)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[launchX + rightStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX + rightStep;
				grid.y = launchY;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		rightStep += 1;
	}
	
	int32 leftStep = 1;
	while (launchX - leftStep >= 0)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[launchX - leftStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX - leftStep;
				grid.y = launchY;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		leftStep += 1;
	}

	int32 upStep = 1;
	while (launchY + upStep <= 
		UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow + 
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + upStep].colCardInfos[launchX];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = launchY + upStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		upStep += 1;
	}

	int32 downStep = 1;
	while (launchY - downStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - downStep].colCardInfos[launchX];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = launchY - downStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		downStep += 1;
	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType)
{
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];

	TArray<FGridXY> possibleGrids;
	bool isRightBarbetteSet = false;
	bool isRightBlock = false;

	bool isLeftBarbetteSet = false;
	bool isLeftBlock = false;

	bool isUpBarbetteSet = false;
	bool isUpBlock = false;

	bool isDownBarbetteSet = false;
	bool isDownBlock = false;
	for (int32 i = launchX + 1; i < UGlobalConstFunctionLibrary::maxCol; i++)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[i];
		if (!isRightBarbetteSet)
		{
			if (uid != -1)
			{
				isRightBarbetteSet = true;
			}
		}
		else
		{
			if (uid != -1 &&
				((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none")))
			{
				FGridXY grid;
				grid.x = i;
				grid.y = launchY;
				possibleGrids.Add(grid);
				break;
			}
		}
	}

	for (int32 i = launchX - 1; i >= 0; i--)
	{
		int32 uid = boardCardInfo[launchY].colCardInfos[i];
		if (!isLeftBarbetteSet)
		{
			if (uid != -1)
			{
				isLeftBarbetteSet = true;
			}
		}
		else
		{
			if (uid != -1 &&
				((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "none")))
			{
				FGridXY grid;
				grid.x = i;
				grid.y = launchY;
				possibleGrids.Add(grid);
				break;
			}
		}
	}

	for (int32 i = launchY + 1; i < UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow; i++)
	{
		int32 uid = boardCardInfo[i].colCardInfos[launchX];
		if (!isUpBarbetteSet)
		{
			if (uid != -1)
			{
				isUpBarbetteSet = true;
			}
		}
		else
		{
			if (uid != -1 &&
				((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "none")))
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = i;
				possibleGrids.Add(grid);
				break;
			}
		}
	}

	for (int32 i = launchY - 1; i >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow; i--)
	{
		int32 uid = boardCardInfo[i].colCardInfos[launchX];
		if (!isDownBarbetteSet)
		{
			if (uid != -1)
			{
				isDownBarbetteSet = true;
			}
		}
		else
		{
			if (uid != -1 &&
				((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
					(effectInfo.targetCamp == "none")))
			{
				FGridXY grid;
				grid.x = launchX;
				grid.y = i;
				possibleGrids.Add(grid);
				break;
			}
		}
	}
	return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, EAtkDistanceType distanceType)
{
	TArray<FGridXY> possibleGrids;

	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	bool isLeftUpBlock = false;
	bool isRightUpBlock = false;
	bool isLeftDownBlock = false;
	bool isRightDownBlock = false;
	int32 leftUpStep = 1;
	while (launchX - leftUpStep >= 0 &&
		launchY + leftUpStep <= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + leftUpStep].colCardInfos[launchX - leftUpStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX - leftUpStep;
				grid.y = launchY + leftUpStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		leftUpStep += 1;
	}

	int32 rightUpStep = 1;
	while (launchX + rightUpStep <= UGlobalConstFunctionLibrary::maxCol - 1 &&
		launchY + rightUpStep <= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 uid = boardCardInfo[launchY + rightUpStep].colCardInfos[launchX + rightUpStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX + rightUpStep;
				grid.y = launchY + rightUpStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		rightUpStep += 1;
	}

	int32 leftDownStep = 1;
	while (launchX - leftDownStep >= 0 &&
		launchY - leftDownStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - leftDownStep].colCardInfos[launchX - leftDownStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX - leftDownStep;
				grid.y = launchY - leftDownStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		leftDownStep += 1;
	}

	int32 rightDownStep = 1;
	while (launchX + rightDownStep <= UGlobalConstFunctionLibrary::maxCol - 1 &&
		launchY - rightDownStep >= UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow)
	{
		int32 uid = boardCardInfo[launchY - rightDownStep].colCardInfos[launchX + rightDownStep];
		if (uid != -1)
		{
			if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
				(effectInfo.targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX + rightDownStep;
				grid.y = launchY - rightDownStep;
				possibleGrids.Add(grid);
			}
			break;
		}

		if (distanceType == EAtkDistanceType::Closed)
		{
			break;
		}
		rightDownStep += 1;
	}


	return possibleGrids;
}


TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetAutoSkillTargetGrids(
	uint8 launchCamp,
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX,
	int32 launchY,
	int32 targetX,
	int32 targetY,
	FString targetGeoType,
	FString targetCamp)
{
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	//uint8 launchCamp = allInstanceCardInfo[launchUid].camp;

	TArray<FGridXY> modifyGrids;
	if (targetGeoType == "left")
	{
		/*
		if (launchX > 0 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1)
		{
			int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX - 1];
			// which means target grid left is not empty
			if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
				(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
				(targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX - 1;
				grid.y = launchY;
				modifyGrids.Add(grid);
			}
		}*/
	}
	else if (targetGeoType == "right")
	{
		/*
		if (launchX < maxCol - 1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1)
		{
			int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX + 1];
			// which means target grid left is not empty
			if ((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
				(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
				(targetCamp == "none"))
			{
				FGridXY grid;
				grid.x = launchX + 1;
				grid.y = launchY;
				modifyGrids.Add(grid);
			}
		}
		*/
	}
	else if (targetGeoType == "forward")
	{
		if (launchCamp == 0)
		{
			if (targetY < (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow - 1))
			{
				int32 targetUid = boardCardInfo[targetY + 1].colCardInfos[targetX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = targetX;
					grid.y = targetY + 1;
					modifyGrids.Add(grid);
				}
			}
		}
		else
		{
			if (targetY > (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow))
			{
				int32 targetUid = boardCardInfo[targetY - 1].colCardInfos[targetX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = targetX;
					grid.y = targetY - 1;
					modifyGrids.Add(grid);
				}
			}
		}
	}
	else if (targetGeoType == "backward")
	{

	}
	return modifyGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetRoundEndAutoSkillTargetGrids(
	uint8 launchCamp,
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX,
	int32 launchY,
	FString targetGeoType,
	FString targetCamp)
{
	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	//uint8 launchCamp = allInstanceCardInfo[launchUid].camp;

	TArray<FGridXY> modifyGrids;
	if (targetGeoType == "self")
	{
		FGridXY grid;
		grid.x = launchX;
		grid.y = launchY;
		modifyGrids.Add(grid);
	}
	else if (targetGeoType == "left")
	{
		if (launchCamp == 0)
		{
			if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX + 1];
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX - 1;
					grid.y = launchY;
					modifyGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchX > 0)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX - 1];
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX + 1;
					grid.y = launchY;
					modifyGrids.Add(grid);
				}
			}
		}
	}
	else if (targetGeoType == "right")
	{
		if (launchCamp == 0)
		{
			if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX + 1];
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX + 1;
					grid.y = launchY;
					modifyGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchX > 0)
			{
				int32 targetUid = boardCardInfo[launchY].colCardInfos[launchX - 1];
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX - 1;
					grid.y = launchY;
					modifyGrids.Add(grid);
				}
			}
		}
	}
	else if (targetGeoType == "forward")
	{
		if (launchCamp == 0)
		{
			if (launchY < (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow - 1))
			{
				int32 targetUid = boardCardInfo[launchY + 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY + 1;
					modifyGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchY > (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow))
			{
				int32 targetUid = boardCardInfo[launchY - 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY - 1;
					modifyGrids.Add(grid);
				}
			}
		}
	}
	else if (targetGeoType == "backward")
	{

	}
	return modifyGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPassiveSkillTargetGrids(
	uint8 launchCamp,
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	int32 triggerX,
	int32 triggerY,
	FString targetGeoType,
	FString targetCamp)
{
	int32 launchUid = boardRows[launchY].colCardInfos[launchX];
	//uint8 launchCamp = allInstanceCardInfo[launchUid].camp;

	TArray<FGridXY> modifyGrids;
	if (targetGeoType == "self")
	{
		FGridXY grid;
		grid.x = launchX;
		grid.y = launchY;
		modifyGrids.Add(grid);
	}
	else if (targetGeoType == "reflect")
	{
		FGridXY grid;
		grid.x = triggerX;
		grid.y = triggerY;
		modifyGrids.Add(grid);
	}
	else if (targetGeoType == "left")
	{

	}
	else if (targetGeoType == "right")
	{
		FGridXY grid;
		if (launchCamp == 0)
		{
			if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
			{
				grid.x = launchX + 1;
				grid.y = launchY;
				modifyGrids.Add(grid);
			}
		}
		else
		{
			if (launchX > 0)
			{
				grid.x = launchX - 1;
				grid.y = launchY;
				modifyGrids.Add(grid);
			}
		}
	}
	else if (targetGeoType == "forward")
	{
		if (launchCamp == 0)
		{
			if (launchY < (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow - 1))
			{
				int32 targetUid = boardRows[launchY + 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY + 1;
					modifyGrids.Add(grid);
				}
			}
		}
		else
		{
			if (launchY > (UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow))
			{
				int32 targetUid = boardRows[launchY - 1].colCardInfos[launchX];
				// which means target grid left is not empty
				if (targetUid != -1 &&
					((targetCamp == "self" && allInstanceCardInfo[targetUid].camp == launchCamp) ||
						(targetCamp == "oppo" && allInstanceCardInfo[targetUid].camp != launchCamp) ||
						(targetCamp == "none")))
				{
					FGridXY grid;
					grid.x = launchX;
					grid.y = launchY - 1;
					modifyGrids.Add(grid);
				}
			}
		}
	}
	else if (targetGeoType == "backward")
	{

	}
	return modifyGrids;
}







void UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateGeoGrids(FString locateGeoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TArray<FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids)
{
	if (locateGeoRule == "three")
	{
		CheckPossibleTargetLocateThree(allInstanceCardInfo, boardCardInfo, checkGrids);
	}
}

void UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids)
{
	int32 checkGridNb = 0;
	while (checkGridNb < checkGrids.Num())
	{
		int32 targetX = checkGrids[checkGridNb].x;
		int32 targetY = checkGrids[checkGridNb].y;
		if (targetX == 0 || targetX == UGlobalConstFunctionLibrary::maxCol - 1)
		{
			// which means we only need to check vertical direction
			if (targetY != 0 && targetY != UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow + 
				UGlobalConstFunctionLibrary::boardSectionRow - 1)
			{
				if (boardCardInfo[targetY - 1].colCardInfos[targetX] != -1 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1)
				{
					checkGridNb += 1;
					continue;
				}
				else
				{
					checkGrids.RemoveAt(checkGridNb);
				}
			}
			else
			{
				checkGrids.RemoveAt(checkGridNb);
			}
		}
		else
		{
			if (targetY == 0 || targetY == UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow + 
				UGlobalConstFunctionLibrary::boardSectionRow - 1)
			{
				// which means we only need to check horizontal direction
				if (boardCardInfo[targetY].colCardInfos[targetX - 1] != -1 && boardCardInfo[targetY].colCardInfos[targetX + 1] != -1)
				{
					checkGridNb += 1;
					continue;
				}
				else
				{
					checkGrids.RemoveAt(checkGridNb);
				}
			}
			else
			{
				// which means we need to check horizontal and vertical directions
				if ((boardCardInfo[targetY].colCardInfos[targetX - 1] != -1 && boardCardInfo[targetY].colCardInfos[targetX + 1] != -1) ||
					(boardCardInfo[targetY - 1].colCardInfos[targetX] != -1 && boardCardInfo[targetY + 1].colCardInfos[targetX] != -1))
				{
					checkGridNb += 1;
					continue;
				}
				else
				{
					checkGrids.RemoveAt(checkGridNb);
				}
			}
		}
	}
}

