// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckLaunchGeoLibrary.h"

bool UCheckLaunchGeoLibrary::CheckLaunchGeoType(FString geoType,
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX, int32 launchY)
{
	if (geoType == "point")
	{
		return true;
	}
	else if (geoType == "three")
	{
		bool successFlag = CheckThree(allInstanceCardInfo,
			boardCardInfo,
			launchX, launchY);

		return successFlag;
	}
	else if (geoType == "selfSection")
	{
		bool successFlag = CheckSelfSection(allInstanceCardInfo,
			boardCardInfo,
			launchX, launchY);

		return successFlag;
	}
	else if (geoType == "horn")
	{
		bool successFlag = CheckHorn(allInstanceCardInfo,
			boardCardInfo,
			launchX, launchY);

		return successFlag;
	}
	
	return true;
}

bool UCheckLaunchGeoLibrary::CheckSelfSection(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX, int32 launchY)
{
	if (boardCardInfo[launchY].colCardInfos[launchX] == -1)
	{
		return false;
	}

	int32 launchUid = boardCardInfo[launchY].colCardInfos[launchX];
	if (allInstanceCardInfo[launchUid].camp == 0)
	{
		if (launchY >= UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow &&
			launchY < UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow / 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (launchY >= UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow / 2 &&
			launchY < UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool UCheckLaunchGeoLibrary::CheckHorn(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX, int32 launchY)
{
	if (boardCardInfo[launchY].colCardInfos[launchX] == -1)
	{
		return false;
	}

	if (launchX == 0)
	{
		if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow + 
			UGlobalConstFunctionLibrary::playCardSectionRow)
		{
			if (boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 &&
				boardCardInfo[launchY].colCardInfos[launchX + 1] != -1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow - 1)
		{
			if (boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if ((boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1) ||
				(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else if (launchX == UGlobalConstFunctionLibrary::maxCol - 1)
	{
		if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow)
		{
			if (boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow - 1)
		{
			if (boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if ((boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1) ||
				(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow)
		{
			if ((boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1) ||
				(boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow - 1)
		{
			if ((boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1) ||
				(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if ((boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1) ||
				(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX - 1] != -1) ||
				(boardCardInfo[launchY + 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1) ||
				(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX + 1] != -1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

bool UCheckLaunchGeoLibrary::CheckThree(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	int32 launchX, int32 launchY)
{
	if (boardCardInfo[launchY].colCardInfos[launchX] == -1)
	{
		return false;
	}

	if (launchX == 0 || launchX == UGlobalConstFunctionLibrary::maxCol - 1)
	{
		// which means we only check vertical direction
		if (launchY > UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow ||
			launchY < UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow - 1)
		{
			if (boardCardInfo[launchY - 1].colCardInfos[launchX] == -1 ||
				boardCardInfo[launchY + 1].colCardInfos[launchX] == -1)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	
	if (launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow ||
		launchY == UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		// which means we only check horizontal direction
		if (launchX == 0 ||
			launchX == UGlobalConstFunctionLibrary::maxCol - 1)
		{
			return false;
		}
		else
		{
			if (boardCardInfo[launchY].colCardInfos[launchX - 1] == -1 ||
				boardCardInfo[launchY].colCardInfos[launchX + 1] == -1)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	if ((boardCardInfo[launchY].colCardInfos[launchX] != -1 && boardCardInfo[launchY].colCardInfos[launchX] != -1) ||
		(boardCardInfo[launchY - 1].colCardInfos[launchX] != -1 && boardCardInfo[launchY + 1].colCardInfos[launchX] != -1))
	{
		return true;
	}
	else
	{
		return false;
	}
}
