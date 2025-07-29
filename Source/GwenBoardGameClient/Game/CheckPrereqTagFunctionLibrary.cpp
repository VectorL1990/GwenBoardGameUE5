// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckPrereqTagFunctionLibrary.h"

bool UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, 
	TArray<FBoardRow>& boardCardInfo, 
	FEffectInfo& effectInfo, 
	int32 launchX, 
	int32 launchY)
{
	if (effectInfo.prereqTagCondition == "none")
	{
		return true;
	}
	else if (effectInfo.prereqTagCondition == "launchNextToTag")
	{
		return CheckLaunchNextToTag(allInstanceCardInfo,
			boardCardInfo,
			launchX,
			launchY,
			effectInfo.prereqTag);
	}
	else if (effectInfo.prereqTagCondition == "launchHasTag")
	{
		return CheckLaunchSelfHasTag(allInstanceCardInfo,
			boardCardInfo,
			launchX,
			launchY,
			effectInfo.prereqTag);
	}

	return true;
}

bool UCheckPrereqTagFunctionLibrary::CheckLaunchNextToTag(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	FString prereqTag)
{
	if (launchX > 0)
	{
		int32 leftUid = boardRows[launchY].colCardInfos[launchX - 1];
		if (leftUid != -1 && allInstanceCardInfo[leftUid].originCardInfo.cardTags.Contains(prereqTag))
		{
			return true;
		}
	}

	if (launchX < UGlobalConstFunctionLibrary::maxCol - 1)
	{
		int32 rightUid = boardRows[launchY].colCardInfos[launchX + 1];
		if (rightUid != -1 && allInstanceCardInfo[rightUid].originCardInfo.cardTags.Contains(prereqTag))
		{
			return true;
		}
	}

	if (launchY > 0)
	{
		int32 downUid = boardRows[launchY - 1].colCardInfos[launchX];
		if (downUid != -1 && allInstanceCardInfo[downUid].originCardInfo.cardTags.Contains(prereqTag))
		{
			return true;
		}
	}

	if (launchY < UGlobalConstFunctionLibrary::graveCardSectionRow +
		UGlobalConstFunctionLibrary::playCardSectionRow +
		UGlobalConstFunctionLibrary::boardSectionRow - 1)
	{
		int32 upUid = boardRows[launchY + 1].colCardInfos[launchX];
		if (upUid != -1 && allInstanceCardInfo[upUid].originCardInfo.cardTags.Contains(prereqTag))
		{
			return true;
		}
	}

	return false;
}

bool UCheckPrereqTagFunctionLibrary::CheckLaunchSelfHasTag(
	TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 launchX,
	int32 launchY,
	FString prereqTag)
{
	int32 launchUid = boardRows[launchY].colCardInfos[launchX];
	if (allInstanceCardInfo[launchUid].originCardInfo.cardTags.Contains(prereqTag))
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool UCheckPrereqTagFunctionLibrary::CheckTargetPrereqTagRule(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardCardInfo,
	FEffectInfo& effectInfo,
	int32 targetX,
	int32 targetY)
{
	if (effectInfo.prereqTagCondition == "none")
	{
		return true;
	}
	else if (effectInfo.prereqTagCondition == "targetHasTag")
	{
		return CheckTargetHasTag(allInstanceCardInfo,
			boardCardInfo,
			targetX,
			targetY,
			effectInfo.prereqTag);
	}
	return true;
}

bool UCheckPrereqTagFunctionLibrary::CheckTargetHasTag(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
	TArray<FBoardRow>& boardRows,
	int32 targetX,
	int32 targetY,
	FString prereqTag)
{
	int32 targetUid = boardRows[targetY].colCardInfos[targetX];
	if (allInstanceCardInfo[targetUid].originCardInfo.cardTags.Contains(prereqTag))
	{
		return true;
	}
	else
	{
		return false;
	}
}
