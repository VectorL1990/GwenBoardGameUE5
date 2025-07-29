// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckPrereqFunctionLibrary.h"

bool UCheckPrereqFunctionLibrary::CheckPrereqRule(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	FString prereq,
	int32 launchX,
	int32 launchY,
	int32 launchCamp,
	FString prereqCampType,
	int32 prereqValue)
{
	if (prereq == "selfSameRowDefenceMore")
	{
		return CheckSelfSameRowDefenceMore(allInstanceCardInfo,
			boardRows,
			launchX,
			launchY,
			launchCamp,
			prereqValue,
			prereqCampType);
	}
	else if (prereq == "oppoSameRowDefenceMore")
	{
		return CheckOppoSameRowDefenceMore(allInstanceCardInfo,
			boardRows,
			launchX,
			launchY,
			launchCamp,
			prereqValue,
			prereqCampType);
	}
	else if (prereq == "sameRowDefenceMore")
	{
		return CheckSameRowDefenceMore(allInstanceCardInfo,
			boardRows,
			launchX,
			launchY,
			launchCamp,
			prereqValue,
			prereqCampType);
	}
	else if (prereq == "maxDefenceIsSelf")
	{
		return CheckMaxDefenceIsSelf(allInstanceCardInfo,
			boardRows,
			launchX,
			launchY,
			launchCamp,
			prereqValue,
			prereqCampType);
	}
	return true;
}

bool UCheckPrereqFunctionLibrary::CheckSelfSameRowDefenceMore(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows, 
	int32 launchX, 
	int32 launchY, 
	int32 launchCamp, 
	int32 prereqValue, 
	FString prereqCampType)
{
	int32 accDefence = 0;
	for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
	{
		int32 uid = boardRows[launchY].colCardInfos[i];
		if (uid == -1 || (launchCamp != allInstanceCardInfo[uid].camp))
		{
			continue;
		}

		accDefence += allInstanceCardInfo[uid].curDefence;
	}

	if (accDefence >= prereqValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UCheckPrereqFunctionLibrary::CheckOppoSameRowDefenceMore(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	int32 launchCamp,
	int32 prereqValue,
	FString prereqCampType)
{
	int32 accDefence = 0;
	for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
	{
		int32 uid = boardRows[launchY].colCardInfos[i];
		if (uid == -1 || (launchCamp == allInstanceCardInfo[uid].camp))
		{
			continue;
		}

		accDefence += allInstanceCardInfo[uid].curDefence;
	}

	if (accDefence >= prereqValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UCheckPrereqFunctionLibrary::CheckSameRowDefenceMore(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	int32 launchCamp,
	int32 prereqValue,
	FString prereqCampType)
{
	int32 accDefence = 0;
	for (int32 i = 0; i < UGlobalConstFunctionLibrary::maxCol; i++)
	{
		int32 uid = boardRows[launchY].colCardInfos[i];
		if (uid == -1)
		{
			continue;
		}

		accDefence += FMath::Abs(allInstanceCardInfo[uid].curDefence);
	}

	if (accDefence >= prereqValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UCheckPrereqFunctionLibrary::CheckMaxDefenceIsSelf(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	int32 launchCamp,
	int32 prereqValue,
	FString prereqCampType)
{
	return true;
}

