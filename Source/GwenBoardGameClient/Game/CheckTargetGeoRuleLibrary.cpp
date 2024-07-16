// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckTargetGeoRuleLibrary.h"

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance)
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

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance)
{
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
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = i;
																grid.y = launchY;
																possibleGrids.Add(grid);
																break;
												}
								}
				}

				for (int32 i = launchX - 1; i > 0; i--)
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
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = i;
																grid.y = launchY;
																possibleGrids.Add(grid);
																break;
												}
								}
				}

				for (int32 i = launchY + 1; i < UGlobalConstFunctionLibrary::maxRow; i++)
				{
								int32 uid = boardCardInfo[i].colCardInfos[launchY];
								if (!isUpBarbetteSet)
								{
												if (uid != -1)
												{
																isUpBarbetteSet = true;
												}
								}
								else
								{
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = launchX;
																grid.y = i;
																possibleGrids.Add(grid);
																break;
												}
								}
				}

				for (int32 i = launchY - 1; i > 0; i--)
				{
								int32 uid = boardCardInfo[i].colCardInfos[launchY];
								if (!isUpBarbetteSet)
								{
												if (uid != -1)
												{
																isUpBarbetteSet = true;
												}
								}
								else
								{
												if (uid != -1)
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

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;

				int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
				bool isLeftUpBlock = false;
				bool isRightUpBlock = false;
				bool isLeftDownBlock = false;
				bool isRightDownBlock = false;
				for (int32 i = 0; i < distance; i++)
				{
								if (!isLeftUpBlock && launchX - i - 1 >= 0 && launchY + i + 1 <= UGlobalConstFunctionLibrary::maxCol - 1)
								{
												int32 uid = boardCardInfo[launchY + i + 1].colCardInfos[launchX - i - 1];
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = launchX - i - 1;
																grid.y = launchY + i + 1;
																possibleGrids.Add(grid);
																isLeftUpBlock = true;
												}
								}
								else
								{
												isLeftUpBlock = true;
								}

								if (!isRightUpBlock &&
												launchX + i + 1 <= UGlobalConstFunctionLibrary::maxCol - 1 &&
												launchY + i + 1 <= UGlobalConstFunctionLibrary::maxRow - 1)
								{
												int32 uid = boardCardInfo[launchY + i + 1].colCardInfos[launchX + i + 1];
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = launchX + i + 1;
																grid.y = launchY + i + 1;
																possibleGrids.Add(grid);
																isRightUpBlock = true;
												}
								}
								else
								{
												isRightUpBlock = true;
								}

								if (!isLeftDownBlock &&
												launchX - i - 1 >= 0 &&
												launchY - i - 1 >= 0)
								{
												int32 uid = boardCardInfo[launchY - i - 1].colCardInfos[launchX - i - 1];
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = launchX - i - 1;
																grid.y = launchY - i - 1;
																possibleGrids.Add(grid);
																isLeftDownBlock = true;
												}
								}
								else
								{
												isLeftDownBlock = true;
								}

								if (!isRightDownBlock &&
												launchX + i + 1 <= UGlobalConstFunctionLibrary::maxCol - 1 &&
												launchY - i - 1 >= 0)
								{
												int32 uid = boardCardInfo[launchY - i - 1].colCardInfos[launchX + i + 1];
												if (uid != -1)
												{
																FGridXY grid;
																grid.x = launchX + i + 1;
																grid.y = launchY - i - 1;
																possibleGrids.Add(grid);
																isLeftDownBlock = true;
												}
								}
								else
								{
												isRightDownBlock = true;
								}
				}

				return possibleGrids;
}

