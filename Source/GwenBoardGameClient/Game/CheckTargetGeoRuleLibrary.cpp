// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckTargetGeoRuleLibrary.h"


TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveGrids(FString rule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveLine(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveSeperate(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				return possibleGrids;
}

TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleMoveDiagonal(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				return possibleGrids;
}




TArray<FGridXY> UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(FString geoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY, int32 distance)
{
				TArray<FGridXY> possibleGrids;
				return possibleGrids;
}

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







void UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateGeoGrids(FString locateGeoRule, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids)
{
				if (locateGeoRule == "three")
				{
								CheckPossibleTargetLocateThree(allInstanceCardInfo, boardCardInfo, checkGrids);
				}
}

void UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, TMap<int32, FBoardRow>& boardCardInfo, TArray<FGridXY>& checkGrids)
{
				int32 checkGridNb = 0;
				while (checkGridNb < checkGrids.Num())
				{
								int32 targetX = checkGrids[checkGridNb].x;
								int32 targetY = checkGrids[checkGridNb].y;
								if (targetX == 0 || targetX == UGlobalConstFunctionLibrary::maxCol - 1)
								{
												// which means we only need to check vertical direction
												if (targetY != 0 && targetY != UGlobalConstFunctionLibrary::maxRow - 1)
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
												if (targetY == 0 || targetY == UGlobalConstFunctionLibrary::maxRow - 1)
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

