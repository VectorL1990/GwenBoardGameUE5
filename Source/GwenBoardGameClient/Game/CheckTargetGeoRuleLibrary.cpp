// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckTargetGeoRuleLibrary.h"

TArray<FGridXY> UCehckTargetGeoRuleLibrary::Line(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
				bool isRightBlock = false;
				bool isLeftBlock = false;
				bool isUpBlock = false;
				bool isDownBlock = false;
				for (int32 i = 0; i < distance; i++)
				{
								if (!isRightBlock && launchX + i + 1 <= UGlobalConstFunctionLibrary::maxCol - 1)
								{
												int32 uid = boardCardInfo[launchY].colCardInfos[launchX + i + 1];
												if (uid != -1)
												{
																if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "none"))
																{
																				FGridXY grid;
																				grid.x = launchX + i + 1;
																				grid.y = launchY;
																				possibleGrids.Add(grid);
																				isRightBlock = true;
																}
												}
								}
								else
								{
												isRightBlock = true;
								}

								if (!isLeftBlock && launchX - i - 1 >= 0)
								{
												int32 uid = boardCardInfo[launchY].colCardInfos[launchX - i - 1];
												if (uid != -1)
												{
																if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) || 
																				(effectInfo.targetCamp == "none"))
																{
																				FGridXY grid;
																				grid.x = launchX - i - 1;
																				grid.y = launchY;
																				possibleGrids.Add(grid);
																				isRightBlock = true;
																}
												}
								}
								else
								{
												isLeftBlock = true;
								}


								if (!isUpBlock && launchY + i + 1 <= UGlobalConstFunctionLibrary::maxRow - 1)
								{
												int32 uid = boardCardInfo[launchY + i + 1].colCardInfos[launchX];
												if (uid != -1)
												{
																if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "none"))
																{
																				FGridXY grid;
																				grid.x = launchX;
																				grid.y = launchY + i + 1;
																				possibleGrids.Add(grid);
																				isUpBlock = true;
																}
												}
								}
								else
								{
												isUpBlock = true;
								}


								if (!isDownBlock && launchY - i - 1 >= 0)
								{
												int32 uid = boardCardInfo[launchY - i - 1].colCardInfos[launchX];
												if (uid != -1)
												{
																if ((effectInfo.targetCamp == "self" && allInstanceCardInfo[uid].camp == allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "oppo" && allInstanceCardInfo[uid].camp != allInstanceCardInfo[launchUid].camp) ||
																				(effectInfo.targetCamp == "none"))
																{
																				FGridXY grid;
																				grid.x = launchX;
																				grid.y = launchY - i - 1;
																				possibleGrids.Add(grid);
																				isDownBlock = true;
																}
												}
								}
								else
								{
												isDownBlock = true;
								}
				}
				return possibleGrids;
}

