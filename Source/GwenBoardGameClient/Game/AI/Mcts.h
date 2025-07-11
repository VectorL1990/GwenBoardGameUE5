// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GlobalConstFunctionLibrary.h"
#include "../BattleBoard.h"
#include "MctsTreeNode.h"
#include "TritonHttpClient.h"
#include "StateDecodingGrid.h"
#include "Mcts.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FBoardInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBoardRow> boardRows;

	UPROPERTY()
		TMap<int32, FInstanceCardInfo> allInstanceCardInfo;

	TArray<int32> curRoundPassiveEffectTriggeredUids;


	UPROPERTY()
		TArray<int32> sectionZeroHandCards;

	UPROPERTY()
		TArray<int32> sectionOneHandCards;

	UPROPERTY()
		TArray<int32> sectionZeroGraveCards;

	UPROPERTY()
		TArray<int32> sectionOneGraveCards;

	int32 sectionZeroScores = 0;

	int32 sectionOneScores = 0;

	uint8 curLaunchPlaySectionNb = 0;

	uint8 curPlayingSectionNb = 0;

	bool sectionZeroPlayCardAvailable = true;
	bool sectionZeroMoveAvailable = true;
	bool sectionOnePlayCardAvailable = true;
	bool sectionOneMoveAvailable = true;

	void ResetBoard()
	{
		boardRows.Empty();
		allInstanceCardInfo.Empty();
		curRoundPassiveEffectTriggeredUids.Empty();
		sectionZeroHandCards.Empty();
		sectionOneHandCards.Empty();
		sectionZeroGraveCards.Empty();
		sectionOneGraveCards.Empty();
		sectionZeroScores = 0;
		sectionOneScores = 0;
		if (curLaunchPlaySectionNb == 0)
		{
			curLaunchPlaySectionNb = 1;
			sectionZeroPlayCardAvailable = false;
			sectionZeroMoveAvailable = false;
			sectionOnePlayCardAvailable = true;
			sectionOneMoveAvailable = true;
		}
		else
		{
			curLaunchPlaySectionNb = 0;
			sectionZeroPlayCardAvailable = true;
			sectionZeroMoveAvailable = true;
			sectionOnePlayCardAvailable = false;
			sectionOneMoveAvailable = false;
		}
		//curPlayingSectionNb = FMath::RandRange(0, 1);
		curPlayingSectionNb = curLaunchPlaySectionNb;
		
	}

	FBoardInfo GetCopyBoard()
	{
		FBoardInfo copyBoard;
		copyBoard.boardRows = boardRows;
		copyBoard.allInstanceCardInfo = allInstanceCardInfo;
		copyBoard.sectionZeroHandCards = sectionZeroHandCards;
		copyBoard.sectionOneHandCards = sectionOneHandCards;
		copyBoard.sectionZeroGraveCards = sectionZeroGraveCards;
		copyBoard.sectionOneGraveCards = sectionOneGraveCards;
		copyBoard.sectionZeroPlayCardAvailable = sectionZeroPlayCardAvailable;
		copyBoard.sectionZeroMoveAvailable = sectionZeroMoveAvailable;
		copyBoard.curPlayingSectionNb = curPlayingSectionNb;
		copyBoard.sectionOnePlayCardAvailable = sectionOnePlayCardAvailable;
		copyBoard.sectionOneMoveAvailable = sectionOneMoveAvailable;
		copyBoard.sectionZeroScores = sectionZeroScores;
		copyBoard.sectionOneScores = sectionOneScores;
		return copyBoard;
	}

	void StateDecoding(UMctsTreeNode* node, int32* boardCoding, TArray<AStateDecodingGrid*>& decodingGrids)
	{
		int32 channelLen = UGlobalConstFunctionLibrary::maxCol * (
			UGlobalConstFunctionLibrary::graveCardSectionRow * 2 +
			UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
			UGlobalConstFunctionLibrary::boardSectionRow);


		for (int32 i = 0; i < node->replayBoardRows.Num(); i++)
		{
			for (int32 j = 0; j < node->replayBoardRows[i].colCardInfos.Num(); j++)
			{
				AStateDecodingGrid* decodeGrid = decodingGrids[i * UGlobalConstFunctionLibrary::maxCol + j];
				int32 posInChannel = i * UGlobalConstFunctionLibrary::maxCol + j;
				int32 skillLaunchTypeStartChannelNb = 0;
				FString decodeSkillLaunchType;
				for (int32 k = 0; k < 2; k++)
				{
					int32 code = boardCoding[channelLen * (k + skillLaunchTypeStartChannelNb) + posInChannel];
					if (k == 0)
					{
						if (code == 1)
						{
							decodeSkillLaunchType = "auto";
						}
					}
					else if (k == 1)
					{
						if (code == 1)
						{
							decodeSkillLaunchType = "manual";
						}
					}
				}
				decodeGrid->skillLaunchType = decodeSkillLaunchType;
				
				int32 autoSkillTargetGeoTypeStartChannelNb = 2;
				FString decodeSkillTargetGeoType;
				for (int32 k = 0; k < 2; k++)
				{
					int32 code = boardCoding[channelLen * (k + autoSkillTargetGeoTypeStartChannelNb) + posInChannel];
					if (k == 0)
					{
						if (code == 1)
						{
							decodeSkillTargetGeoType = "left";
						}
					}
					else
					{
						if (code == 1)
						{
							decodeSkillTargetGeoType = "forward";
						}
					}
				}
				decodeGrid->skillTargetGeoType = decodeSkillTargetGeoType;

				int32 targetCampTypeStartChannelNb = 4;
				FString decodeTargetCampType;
				for (int32 k = 0; k < 2; k++)
				{
					int32 code = boardCoding[channelLen * (k + targetCampTypeStartChannelNb) + posInChannel];
					if (k == 0)
					{
						if (code == 1)
						{
							decodeTargetCampType = "self";
						}
					}
					else
					{
						if (code == 1)
						{
							decodeTargetCampType = "oppo";
						}
					}
				}
				decodeGrid->targetCampType = decodeTargetCampType;

				int32 effectTypeStartChannelNb = 6;
				FString effectType;
				for (int32 k = 0; k < 2; k++)
				{
					int32 code = boardCoding[channelLen * (k + effectTypeStartChannelNb) + posInChannel];
					if (k == 0)
					{
						if (code == 1)
						{
							effectType = "hurt";
						}
					}
					else
					{
						if (code == 1)
						{
							effectType = "heal";
						}
					}
				}
				decodeGrid->effectType = effectType;

				int32 launchGeoTypeStartChannelNb = 8;
				FString launchGeoType;
				for (int32 k = 0; k < 2; k++)
				{
					int32 code = boardCoding[channelLen * (k + launchGeoTypeStartChannelNb) + posInChannel];
					if (k == 0)
					{
						if (code == 1)
						{
							launchGeoType = "point";
						}
					}
					else
					{
						if (code == 1)
						{
							launchGeoType = "three";
						}
					}
				}
				decodeGrid->launchGeoType = launchGeoType;

				int32 sectionTagStartChannelNb = 10;
				if (boardCoding[channelLen * sectionTagStartChannelNb + posInChannel] == 0)
				{
					decodeGrid->curSection = 0;
				}
				else
				{
					decodeGrid->curSection = 1;
				}
			}
		}
	}

	void StateCoding(uint8 curSectionNb, int32* boardCoding)
	{
		int32 channelLen = UGlobalConstFunctionLibrary::maxCol *
			(UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
				UGlobalConstFunctionLibrary::graveCardSectionRow * 2 +
				UGlobalConstFunctionLibrary::boardSectionRow);

		int32 sectionZeroTotalHp = 0;
		int32 sectionOneTotalHp = 0;
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow;
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + 
			UGlobalConstFunctionLibrary::playCardSectionRow * 2 + 
			UGlobalConstFunctionLibrary::boardSectionRow; i++)
		{
			for (int32 j = 0; j < boardRows[i].colCardInfos.Num(); j++)
			{
				int32 uid = boardRows[i].colCardInfos[j];

				if (uid == -1)
				{
					continue;
				}

				if (allInstanceCardInfo[uid].camp == 0)
				{
					sectionZeroTotalHp += allInstanceCardInfo[uid].curHp;
				}
				else
				{
					sectionOneTotalHp += allInstanceCardInfo[uid].curHp;
				}
			}
		}

		for (int32 i = 0; i < boardRows.Num(); i++)
		{
			for (int32 j = 0; j < boardRows[i].colCardInfos.Num(); j++)
			{
				int32 posInChannel = i * UGlobalConstFunctionLibrary::maxCol + j;

				int32 sectionTagStartChannelNb = 23;
				if (curSectionNb == 0)
				{
					boardCoding[channelLen * sectionTagStartChannelNb + posInChannel] = 1;
				}
				else
				{
					boardCoding[channelLen * sectionTagStartChannelNb + posInChannel] = -1;
				}

				int32 hpDiffStartChannelNb = 24;
				if (curSectionNb == 0)
				{
					boardCoding[channelLen * hpDiffStartChannelNb + posInChannel] = sectionZeroTotalHp - sectionOneTotalHp;
				}
				else
				{
					boardCoding[channelLen * hpDiffStartChannelNb + posInChannel] = sectionOneTotalHp - sectionZeroTotalHp;
				}



				int32 uid = boardRows[i].colCardInfos[j];

				if (uid == -1)
				{
					continue;
				}

				// For testing, only contains:
				// skillLaunchType: auto manual
				// targetGeoType: line, seperate
				// targetCampType: self, oppo
				// effectType: hurt
				// launchGeoType: point, three
				int32 testSkillLaunchTypeCoding[2] = { 0 };
				int32 testAutoSkillTargetGeoTypeCoding[2] = { 0 };
				int32 testTargetCampTypeCoding[2] = { 0 };
				int32 testEffectTypeCoding[2] = { 0 };
				int32 testLaunchGeoType[2] = { 0 };
				int32 testTargetGeoType[2] = { 0 };
				int32 testMoveType[2] = { 0 };
				int32 testAtkDisType[2] = { 0 };

				GetTestSkillLaunchTypeCoding(allInstanceCardInfo[uid].originCardInfo.launchType, testSkillLaunchTypeCoding);
				GetTestAutoSkillGeoTargetCoding(allInstanceCardInfo[uid].originCardInfo.autoSkillTargetGeoType, testAutoSkillTargetGeoTypeCoding);
				GetSkillTargetCampCoding(allInstanceCardInfo[uid].originCardInfo.targetCamp, testTargetCampTypeCoding);
				GetTestSkillEffectCoding(allInstanceCardInfo[uid].originCardInfo.effectType, testEffectTypeCoding);
				GetTestSkillLaunchGeoCoding(allInstanceCardInfo[uid].originCardInfo.launchGeoType, testLaunchGeoType);
				GetTestSkillTargetGeoCoding(allInstanceCardInfo[uid].originCardInfo.targetGeoType, testTargetGeoType);
				GetTestMoveTypeCoding(allInstanceCardInfo[uid].originCardInfo.moveType, testMoveType);
				GetTestAtkDistanceCoding(allInstanceCardInfo[uid].originCardInfo.attackDistanceType, testAtkDisType);

				/*
				int32 autoSkillGeoTargetTypeCoding[17] = { 0 };
				int32 skillLaunchTypeCoding[6] = { 0 };
				int32 skillLaunchGeoCoding[10] = { 0 };
				int32 skillTargetGeoCoding[6] = { 0 };
				int32 skillTargetLocateGeoCoding[9] = { 0 };
				int32 skillAoeCoding[6] = { 0 };
				int32 skillTargetCampCoding[3] = { 0 };
				int32 skillEffectCoding[86] = { 0 };
				int32 skillAffixCampCoding[3] = { 0 };
				int32 skillAffixCoding[84] = { 0 };
				int32 skillTagConditionCoding[5] = { 0 };
				int32 cardTagCoding[42] = { 0 };
				int32 skillPrereqTagCoding[42] = { 0 };
				int32 skillPrereqCampCoding[3] = { 0 };
				int32 skillPrereqCoding[126] = { 0 };
				int32 passiveSkillPrereqCoding[11] = { 0 };
				// 6
				GetSkillLaunchTypeCoding(allInstanceCardInfo[uid].originCardInfo.launchType, skillLaunchTypeCoding);
				GetAutoSkillGeoTargetCoding(allInstanceCardInfo[uid].originCardInfo.autoSkillTargetGeoType, autoSkillGeoTargetTypeCoding);
				// 10
				GetSkillLaunchGeoCoding(allInstanceCardInfo[uid].originCardInfo.launchGeoType, skillLaunchGeoCoding);
				// 6
				GetSkillTargetGeoCoding(allInstanceCardInfo[uid].originCardInfo.targetGeoType, skillTargetGeoCoding);
				// 9
				//uint8* skillTargetLocateGeoCoding = GetSkillTargetLocateGeoCoding(allInstanceCardInfo[uid].originCardInfo.target);
				// 6
				GetSkillAoeCoding(allInstanceCardInfo[uid].originCardInfo.aoeType, skillAoeCoding);
				// 3

				*/

				// There are 10 channels, which means there are 10 images
				// Every image size is W x H

				int32 boardSectionTagStartChannelNb = 0;
				if (i >= UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow &&
					i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * boardSectionTagStartChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * boardSectionTagStartChannelNb + posInChannel] = -1;
					}
				}

				int32 hpStartChannelNb = 1;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * hpStartChannelNb + posInChannel] = allInstanceCardInfo[uid].curHp;
				}
				else
				{
					boardCoding[channelLen * hpStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].curHp;
				}

				int32 defenceStartChannelNb = 2;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * defenceStartChannelNb + posInChannel] = allInstanceCardInfo[uid].curDefence;
				}
				else
				{
					boardCoding[channelLen * defenceStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].curDefence;
				}

				int32 originCoolDownStartChannelNb = 3;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					if (allInstanceCardInfo[uid].originCardInfo.coolDown == -1)
					{
						boardCoding[channelLen * originCoolDownStartChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * originCoolDownStartChannelNb + posInChannel] = allInstanceCardInfo[uid].originCardInfo.coolDown;
					}
				}
				else
				{
					if (allInstanceCardInfo[uid].originCardInfo.coolDown == -1)
					{
						boardCoding[channelLen * originCoolDownStartChannelNb + posInChannel] = -1;
					}
					else
					{
						boardCoding[channelLen * originCoolDownStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].originCardInfo.coolDown;
					}
				}

				int32 curCoolDownStartChannelnb = 4;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * curCoolDownStartChannelnb + posInChannel] = allInstanceCardInfo[uid].curCoolDown;
				}
				else
				{
					boardCoding[channelLen * curCoolDownStartChannelnb + posInChannel] = -allInstanceCardInfo[uid].curCoolDown;
				}

				int32 originAvailableTimesStartChannelNb = 5;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					if (allInstanceCardInfo[uid].originCardInfo.availableTimes == -1)
					{
						boardCoding[channelLen * originAvailableTimesStartChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * originAvailableTimesStartChannelNb + posInChannel] = allInstanceCardInfo[uid].originCardInfo.availableTimes;
					}
				}
				else
				{
					if (allInstanceCardInfo[uid].originCardInfo.availableTimes == -1)
					{
						boardCoding[channelLen * originAvailableTimesStartChannelNb + posInChannel] = -1;
					}
					else
					{
						boardCoding[channelLen * originAvailableTimesStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].originCardInfo.availableTimes;
					}
				}

				int32 availableTimesStartChannelNb = 6;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * availableTimesStartChannelNb + posInChannel] = allInstanceCardInfo[uid].curAvailableTimes;
				}
				else
				{
					boardCoding[channelLen * availableTimesStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].curAvailableTimes;
				}

				int32 skillLaunchTypeStartChannelNb = 7;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + skillLaunchTypeStartChannelNb) + posInChannel] = testSkillLaunchTypeCoding[k];
					}
					else
					{
						boardCoding[channelLen * (k + skillLaunchTypeStartChannelNb) + posInChannel] = -testSkillLaunchTypeCoding[k];
					}
				}

				int32 autoSkillTargetGeoTypeStartChannelNb = 9;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + autoSkillTargetGeoTypeStartChannelNb) + posInChannel] = testAutoSkillTargetGeoTypeCoding[k];
					}
					else
					{
						boardCoding[channelLen * (k + autoSkillTargetGeoTypeStartChannelNb) + posInChannel] = -testAutoSkillTargetGeoTypeCoding[k];
					}
				}

				int32 targetCampTypeStartChannelNb = 11;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + targetCampTypeStartChannelNb) + posInChannel] = testTargetCampTypeCoding[k];
					}
					else
					{
						boardCoding[channelLen * (k + targetCampTypeStartChannelNb) + posInChannel] = -testTargetCampTypeCoding[k];
					}
				}

				int32 effectTypeStartChannelNb = 13;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + effectTypeStartChannelNb) + posInChannel] = testEffectTypeCoding[k];
					}
					else
					{
						boardCoding[channelLen * (k + effectTypeStartChannelNb) + posInChannel] = -testEffectTypeCoding[k];
					}
				}

				int32 launchGeoTypeStartChannelNb = 15;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + launchGeoTypeStartChannelNb) + posInChannel] = testLaunchGeoType[k];
					}
					else
					{
						boardCoding[channelLen * (k + launchGeoTypeStartChannelNb) + posInChannel] = -testLaunchGeoType[k];
					}
				}

				int32 targetGeoTypeStartChannelNb = 17;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + targetGeoTypeStartChannelNb) + posInChannel] = testTargetGeoType[k];
					}
					else
					{
						boardCoding[channelLen * (k + targetGeoTypeStartChannelNb) + posInChannel] = -testTargetGeoType[k];
					}
				}

				int32 moveTypeStartChannelNb = 19;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + moveTypeStartChannelNb) + posInChannel] = testMoveType[k];
					}
					else
					{
						boardCoding[channelLen * (k + moveTypeStartChannelNb) + posInChannel] = -testMoveType[k];
					}
				}

				int32 atkDisTypeStartChannelNb = 21;
				for (int32 k = 0; k < 2; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + atkDisTypeStartChannelNb) + posInChannel] = testAtkDisType[k];
					}
					else
					{
						boardCoding[channelLen * (k + atkDisTypeStartChannelNb) + posInChannel] = -testAtkDisType[k];
					}
				}
			}
		}
	}

	int32 ActionCoding(int32 launchX, int32 launchY, int32 targetX, int32 targetY, ActionType actionType)
	{
		int32 actionId = -1;
		int32 totalBattleBoardGrids = UGlobalConstFunctionLibrary::boardSectionRow * UGlobalConstFunctionLibrary::maxCol;
		if (actionType == ActionType::PlayCard)
		{
			bool sectionOnePlay = false;
			int32 launchPlaySectionBoardY = launchY - UGlobalConstFunctionLibrary::graveCardSectionRow;
			if (launchY > UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow)
			{
				launchPlaySectionBoardY = launchY -
					UGlobalConstFunctionLibrary::graveCardSectionRow -
					UGlobalConstFunctionLibrary::playCardSectionRow -
					UGlobalConstFunctionLibrary::boardSectionRow;
				sectionOnePlay = true;
			}
			int32 launchGridNb = launchPlaySectionBoardY * UGlobalConstFunctionLibrary::maxCol + launchX;

			int32 targetPlayBoardY = targetY -
				UGlobalConstFunctionLibrary::graveCardSectionRow -
				UGlobalConstFunctionLibrary::playCardSectionRow;

			if (!sectionOnePlay)
			{
				actionId = totalBattleBoardGrids/2 * launchGridNb +
					targetPlayBoardY * UGlobalConstFunctionLibrary::maxCol + targetX;
			}
			else
			{
				actionId = UGlobalConstFunctionLibrary::playCardSectionRow *
					UGlobalConstFunctionLibrary::maxCol *
					totalBattleBoardGrids/2 +
					totalBattleBoardGrids/2 * launchGridNb +
					(targetPlayBoardY - UGlobalConstFunctionLibrary::boardSectionRow /2) * UGlobalConstFunctionLibrary::maxCol + targetX;
			}
		}
		else if (actionType == ActionType::LaunchSkill)
		{
			int32 totalPlayCardActionNb = UGlobalConstFunctionLibrary::playCardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				totalBattleBoardGrids/2 * 2;

			int32 launchPlayBoardY = launchY -
				UGlobalConstFunctionLibrary::playCardSectionRow -
				UGlobalConstFunctionLibrary::graveCardSectionRow;

			int32 launchGridNb = launchPlayBoardY * UGlobalConstFunctionLibrary::maxCol + launchX;

			int32 targetPlayBoardY = targetY -
				UGlobalConstFunctionLibrary::playCardSectionRow -
				UGlobalConstFunctionLibrary::graveCardSectionRow;

			actionId = totalBattleBoardGrids * launchGridNb +
				targetPlayBoardY * UGlobalConstFunctionLibrary::maxCol + targetX +
				totalPlayCardActionNb;
		}
		else if (actionType == ActionType::Move)
		{
			int32 totalPlayCardActionNb = UGlobalConstFunctionLibrary::playCardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				totalBattleBoardGrids/2 * 2;

			int32 totalLaunchSkillActionNb = UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol;

			int32 launchPlayBoardY = launchY -
				UGlobalConstFunctionLibrary::playCardSectionRow -
				UGlobalConstFunctionLibrary::graveCardSectionRow;

			int32 targetPlayBoardY = targetY -
				UGlobalConstFunctionLibrary::graveCardSectionRow -
				UGlobalConstFunctionLibrary::playCardSectionRow;

			int32 launchGridNb = launchPlayBoardY * UGlobalConstFunctionLibrary::maxCol + launchX;

			actionId = totalBattleBoardGrids * launchGridNb +
				targetPlayBoardY * UGlobalConstFunctionLibrary::maxCol + targetX +
				totalPlayCardActionNb + totalLaunchSkillActionNb;
		}
		else if (actionType == ActionType::EndRound)
		{
			int32 totalPlayCardActionNb = UGlobalConstFunctionLibrary::playCardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				totalBattleBoardGrids / 2 * 2;

			int32 totalLaunchSkillActionNb = UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol;

			int32 totalMoveActionNb = UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol *
				UGlobalConstFunctionLibrary::boardSectionRow *
				UGlobalConstFunctionLibrary::maxCol;

			actionId = totalPlayCardActionNb + totalLaunchSkillActionNb + totalMoveActionNb;
		}
		return actionId;
	}

	void ActionDecoding(int32 actionId, int32& launchX, int32& launchY, int32& targetX, int32& targetY, ActionType& actionType)
	{
		int32 totalPlayBoardGridNb = UGlobalConstFunctionLibrary::boardSectionRow * UGlobalConstFunctionLibrary::maxCol;

		int32 totalPlayCardActions = UGlobalConstFunctionLibrary::playCardSectionRow * UGlobalConstFunctionLibrary::maxCol *
			UGlobalConstFunctionLibrary::boardSectionRow/2 * UGlobalConstFunctionLibrary::maxCol * 2;

		int32 totalLaunchSkillActions = UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::boardSectionRow *
			UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::boardSectionRow;

		int32 totalMoveActions = UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::boardSectionRow *
			UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::boardSectionRow;
		if (actionId < totalPlayCardActions)
		{
			// which means it's play card action
			int32 playCardSectionGridNb = FMath::FloorToInt((float)actionId / (float)(totalPlayBoardGridNb/2));
			bool sectionOnePlay = false;
			if (playCardSectionGridNb >= UGlobalConstFunctionLibrary::playCardSectionRow * UGlobalConstFunctionLibrary::maxCol)
			{
				// which means this action is launched by section one
				playCardSectionGridNb -= UGlobalConstFunctionLibrary::playCardSectionRow * UGlobalConstFunctionLibrary::maxCol;
				sectionOnePlay = true;
			}
			int32 targetPlayBoardGridNb = actionId % (totalPlayBoardGridNb/2);

			int32 launchPlaySectionBoardRow = FMath::FloorToInt((float)playCardSectionGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 launchPlaySectionBoardCol = playCardSectionGridNb % UGlobalConstFunctionLibrary::maxCol;

			int32 targetPlayBoardRow = FMath::FloorToInt((float)targetPlayBoardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 targetPlayBoardCol = targetPlayBoardGridNb % UGlobalConstFunctionLibrary::maxCol;

			launchX = launchPlaySectionBoardCol;
			targetX = targetPlayBoardCol;
			if (!sectionOnePlay)
			{
				launchY = launchPlaySectionBoardRow + UGlobalConstFunctionLibrary::graveCardSectionRow;
				targetY = targetPlayBoardRow +
					UGlobalConstFunctionLibrary::graveCardSectionRow +
					UGlobalConstFunctionLibrary::playCardSectionRow;
			}
			else
			{
				launchY = UGlobalConstFunctionLibrary::graveCardSectionRow +
					UGlobalConstFunctionLibrary::playCardSectionRow +
					UGlobalConstFunctionLibrary::boardSectionRow +
					launchPlaySectionBoardRow;
				targetY = targetPlayBoardRow +
					UGlobalConstFunctionLibrary::graveCardSectionRow +
					UGlobalConstFunctionLibrary::playCardSectionRow +
					UGlobalConstFunctionLibrary::boardSectionRow / 2;
			}

			actionType = ActionType::PlayCard;
		}
		else if (actionId < totalPlayCardActions + totalLaunchSkillActions)
		{
			// which means it's launch skill action
			int32 launchSkillActionId = actionId - totalPlayCardActions;

			int32 launchPlayBoardGridNb = FMath::FloorToInt((float)launchSkillActionId / (float)totalPlayBoardGridNb);
			int32 targetPlayBoardGridNb = launchSkillActionId % totalPlayBoardGridNb;

			int32 launchPlayBoardRow = FMath::FloorToInt((float)launchPlayBoardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 launchPlayBoardCol = launchPlayBoardGridNb % UGlobalConstFunctionLibrary::maxCol;

			int32 targetPlayBoardRow = FMath::FloorToInt((float)targetPlayBoardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 targetPlayBoardCol = targetPlayBoardGridNb % UGlobalConstFunctionLibrary::maxCol;

			launchX = launchPlayBoardCol;

			launchY = launchPlayBoardRow +
				UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow;

			targetX = targetPlayBoardCol;

			targetY = targetPlayBoardRow +
				UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow;

			actionType = ActionType::LaunchSkill;
		}
		else if (actionId < totalPlayCardActions + totalLaunchSkillActions + totalMoveActions)
		{
			// which means it's move action
			int32 moveActionId = actionId - totalPlayCardActions - totalLaunchSkillActions;

			int32 launchPlayBoardGridNb = FMath::FloorToInt((float)moveActionId / (float)totalPlayBoardGridNb);
			int32 targetPlayBoardGridNb = moveActionId % totalPlayBoardGridNb;

			int32 launchPlayBoardRow = FMath::FloorToInt((float)launchPlayBoardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 launchPlayBoardCol = launchPlayBoardGridNb % UGlobalConstFunctionLibrary::maxCol;

			int32 targetPlayBoardRow = FMath::FloorToInt((float)targetPlayBoardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
			int32 targetPlayBoardCol = targetPlayBoardGridNb % UGlobalConstFunctionLibrary::maxCol;

			launchX = launchPlayBoardCol;

			launchY = launchPlayBoardRow +
				UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow;

			targetX = targetPlayBoardCol;
			targetY = targetPlayBoardRow +
				UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow;

			actionType = ActionType::Move;
		}
		else
		{
			launchX = 0;
			launchY = 0;
			targetX = 0;
			targetY = 0;
			actionType = ActionType::EndRound;
		}
	}

	void GetLegalMoves(uint8 curPlayingSectionNb, TArray<int32>& legalMoves, TArray<ActionType>& actionTypes)
	{
		for (int32 row = 0; row < UGlobalConstFunctionLibrary::boardSectionRow; row++)
		{
			for (int32 col = 0; col < UGlobalConstFunctionLibrary::maxCol; col++)
			{
				int32 checkRow = UGlobalConstFunctionLibrary::graveCardSectionRow +
					UGlobalConstFunctionLibrary::playCardSectionRow + row;
				if (boardRows[checkRow].colCardInfos[col] == -1)
				{
					if ((curPlayingSectionNb == 0 && !sectionZeroPlayCardAvailable) ||
						(curPlayingSectionNb == 1 && !sectionOnePlayCardAvailable))
					{
						continue;
					}

					// which means target grid is empty, we can play card at that grid
					for (int32 playSectionBoardRow = 0; playSectionBoardRow < UGlobalConstFunctionLibrary::playCardSectionRow; playSectionBoardRow++)
					{
						for (int32 playCardCol = 0; playCardCol < UGlobalConstFunctionLibrary::maxCol; playCardCol++)
						{
							if (curPlayingSectionNb == 0 && row < UGlobalConstFunctionLibrary::boardSectionRow / 2)
							{
								if (boardRows[playSectionBoardRow + UGlobalConstFunctionLibrary::graveCardSectionRow].colCardInfos[playCardCol] == -1)
								{
									continue;
								}

								int32 actionId = ActionCoding(
									playCardCol, 
									playSectionBoardRow + UGlobalConstFunctionLibrary::graveCardSectionRow, 
									col, 
									checkRow, 
									ActionType::PlayCard);
								legalMoves.Add(actionId);
								actionTypes.Add(ActionType::PlayCard);
							}
							else if (curPlayingSectionNb == 1 && row >= UGlobalConstFunctionLibrary::boardSectionRow / 2)
							{
								if (boardRows[playSectionBoardRow +
									UGlobalConstFunctionLibrary::graveCardSectionRow +
									UGlobalConstFunctionLibrary::playCardSectionRow +
									UGlobalConstFunctionLibrary::boardSectionRow].colCardInfos[playCardCol] == -1)
								{
									continue;
								}

								int32 launchRow = UGlobalConstFunctionLibrary::graveCardSectionRow +
									UGlobalConstFunctionLibrary::playCardSectionRow +
									UGlobalConstFunctionLibrary::boardSectionRow +
									playSectionBoardRow;
								int32 actionId = ActionCoding(
									playCardCol,
									launchRow,
									col,
									checkRow,
									ActionType::PlayCard);
								legalMoves.Add(actionId);
								actionTypes.Add(ActionType::PlayCard);
							}
						}
					}
				}
				else
				{
					// which means this grid is not empty, we could launch skill or move card
					// check possible skills
					FInstanceCardInfo cardInfo = allInstanceCardInfo[boardRows[checkRow].colCardInfos[col]];
					if (allInstanceCardInfo[boardRows[checkRow].colCardInfos[col]].camp != curPlayingSectionNb)
					{
						continue;
					}

					bool manualSkillAvailable = false;
					if (cardInfo.originCardInfo.launchType == "manual")
					{
						if (cardInfo.originCardInfo.coolDown == -1)
						{
							if (cardInfo.originCardInfo.availableTimes == -1)
							{
								manualSkillAvailable = true;
							}
							else
							{
								if (cardInfo.curAvailableTimes > 0)
								{
									manualSkillAvailable = true;
								}
							}
						}
						else
						{
							if (cardInfo.curCoolDown == 0 &&
								(cardInfo.originCardInfo.availableTimes == -1 ||
								cardInfo.curAvailableTimes > 0))
							{
								manualSkillAvailable = true;
							}
						}
					}

					if (manualSkillAvailable)
					{
						FEffectInfo effectInfo;
						effectInfo.aoeType = cardInfo.originCardInfo.aoeType;
						effectInfo.autoSkillTargetGeoType = cardInfo.originCardInfo.autoSkillTargetGeoType;
						effectInfo.availableTimes = cardInfo.originCardInfo.availableTimes;
						effectInfo.coolDown = cardInfo.originCardInfo.coolDown;
						effectInfo.effectAffix = cardInfo.originCardInfo.effectAffix;
						effectInfo.effectAffixCamp = cardInfo.originCardInfo.effectAffixCamp;
						effectInfo.effectType = cardInfo.originCardInfo.effectType;
						effectInfo.launchGeoType = cardInfo.originCardInfo.launchGeoType;
						effectInfo.launchType = cardInfo.originCardInfo.launchType;
						effectInfo.passivePrereqType = cardInfo.originCardInfo.passivePrereqType;
						effectInfo.prereqCampType = cardInfo.originCardInfo.prereqCampType;
						effectInfo.prereqTag = cardInfo.originCardInfo.prereqTag;
						effectInfo.prereqTagCondition = cardInfo.originCardInfo.prereqTagCondition;
						effectInfo.prereqType = cardInfo.originCardInfo.prereqType;
						effectInfo.targetCamp = cardInfo.originCardInfo.targetCamp;
						effectInfo.targetGeoType = cardInfo.originCardInfo.targetGeoType;
						effectInfo.values = cardInfo.originCardInfo.values;
						TArray<FGridXY> possibleGrids;
						if (effectInfo.targetGeoType.Contains("&"))
						{
							TArray<FString> targetGeoTypes;
							effectInfo.targetGeoType.ParseIntoArray(targetGeoTypes, TEXT("&"), true);
							possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
								targetGeoTypes[0],
								allInstanceCardInfo,
								boardRows,
								effectInfo,
								col,
								checkRow,
								cardInfo.originCardInfo.attackDistanceType);

							UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateGeoGrids(
								targetGeoTypes[1],
								allInstanceCardInfo,
								boardRows,
								possibleGrids);
						}
						else
						{
							possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
								effectInfo.targetGeoType,
								allInstanceCardInfo,
								boardRows,
								effectInfo,
								col,
								checkRow,
								cardInfo.originCardInfo.attackDistanceType);
						}

						int32 checkGridNb = 0;
						while (checkGridNb < possibleGrids.Num())
						{
							// do prereq check first
							if (effectInfo.prereqType != "none")
							{
								if (!UCheckPrereqFunctionLibrary::CheckPrereqRule(
									effectInfo.prereqType,
									col,
									checkRow,
									possibleGrids[checkGridNb].x,
									possibleGrids[checkGridNb].y,
									cardInfo.camp,
									effectInfo.prereqCampType))
								{
									possibleGrids.RemoveAt(checkGridNb);
									continue;
								}
							}

							if (effectInfo.prereqTagCondition != "none")
							{
								if (!UCheckPrereqTagFunctionLibrary::CheckPrereqTagRule(
									allInstanceCardInfo,
									boardRows,
									effectInfo,
									col,
									checkRow))
								{
									possibleGrids.RemoveAt(checkGridNb);
									continue;
								}
							}

							checkGridNb += 1;
						}

						for (int32 i = 0; i < possibleGrids.Num(); i++)
						{
							int32 actionId = ActionCoding(
								col,
								checkRow,
								possibleGrids[i].x,
								possibleGrids[i].y,
								ActionType::LaunchSkill);
							legalMoves.Add(actionId);
							actionTypes.Add(ActionType::LaunchSkill);
						}
					}

					if ((curPlayingSectionNb == 0 && sectionZeroMoveAvailable) ||
						(curPlayingSectionNb == 1 && sectionOneMoveAvailable))
					{
						// check move action
						TArray<FGridXY> possibleMoveGrids = UCheckTargetGeoRuleLibrary::GetPossibleMoveGrids(
							cardInfo.originCardInfo.moveType,
							allInstanceCardInfo,
							boardRows,
							col,
							checkRow,
							cardInfo.originCardInfo.moveDistance);

						for (int32 i = 0; i < possibleMoveGrids.Num(); i++)
						{
							int32 actionId = ActionCoding(
								col,
								checkRow,
								possibleMoveGrids[i].x,
								possibleMoveGrids[i].y,
								ActionType::Move);

							legalMoves.Add(actionId);
							actionTypes.Add(ActionType::Move);
						}
					}
				}
			}
		}


		if (!actionTypes.Contains(ActionType::PlayCard))
		{
			int32 endRoundActionId = ActionCoding(0, 0, 0, 0, ActionType::EndRound);
			legalMoves.Add(endRoundActionId);
			actionTypes.Add(ActionType::EndRound);
		}
	}



	ActionType TriggerAction(
		bool testInference,
		uint8 campNb,
		int32 actionId,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{
		int32 launchX = 0;
		int32 launchY = 0;
		int32 targetX = 0;
		int32 targetY = 0;
		ActionType actionType = ActionType::EndRound;
		ActionDecoding(actionId, launchX, launchY, targetX, targetY, actionType);
		if (actionType == ActionType::LaunchSkill)
		{
			TriggerManualSkill(
				campNb,
				launchX,
				launchY,
				targetX,
				targetY,
				renderEffectRoundList);
		}
		else if (actionType == ActionType::PlayCard)
		{
			TriggerPlayCard(
				testInference,
				campNb,
				launchX,
				launchY,
				targetX,
				targetY,
				renderEffectRoundList);
			if (campNb == 0)
			{
				sectionZeroPlayCardAvailable = false;
			}
			else
			{
				sectionOnePlayCardAvailable = false;
			}
		}
		else if (actionType == ActionType::Move)
		{
			TriggerMove(launchX, launchY, targetX, targetY);
			if (campNb == 0)
			{
				sectionZeroMoveAvailable = false;
			}
			else
			{
				sectionOneMoveAvailable = false;
			}
		}
		else if (actionType == ActionType::EndRound)
		{
			if (curPlayingSectionNb == 0)
			{
				curPlayingSectionNb = 1;
				sectionZeroPlayCardAvailable = false;
				sectionZeroMoveAvailable = false;
				sectionOnePlayCardAvailable = true;
				sectionOneMoveAvailable = true;
			}
			else
			{
				curPlayingSectionNb = 0;
				sectionZeroPlayCardAvailable = true;
				sectionZeroMoveAvailable = true;
				sectionOnePlayCardAvailable = false;
				sectionOneMoveAvailable = false;
			}
		}

		return actionType;
	}



	void TriggerPlayCard(
		bool testInference,
		uint8 launchCampNb,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{
		int32 playCardUid = boardRows[launchY].colCardInfos[launchX];
		// Trigger play card skill
		if (allInstanceCardInfo[playCardUid].originCardInfo.launchType == "auto")
		{
			TriggerPlayCardSkill(launchCampNb, launchX, launchY, targetX, targetY, renderEffectRoundList);
		}
		boardRows[targetY].colCardInfos[targetX] = playCardUid;
		boardRows[launchY].colCardInfos[launchX] = -1;
		if (launchCampNb == 0)
		{
			int32 playCardIndex = sectionZeroHandCards.Find(playCardUid);
			if (playCardIndex != -1)
			{
				sectionZeroHandCards.RemoveAt(playCardIndex);
			}
			
			sectionZeroScores += allInstanceCardInfo[playCardUid].curHp;
		}
		else
		{
			int32 playCardIndex = sectionOneHandCards.Find(playCardUid);
			if (playCardIndex != -1)
			{
				sectionOneHandCards.RemoveAt(playCardIndex);
			}
			/*
			if (testInference)
			{
				allInstanceCardInfo[playCardUid].curHp -= 3;
			}
			*/
			sectionOneScores += allInstanceCardInfo[playCardUid].curHp;
		}
		allInstanceCardInfo[playCardUid].curCol = targetX;
		allInstanceCardInfo[playCardUid].curRow = targetY;
		
	}

	void TriggerPlayCardSkill(
		uint8 launchCamp,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{
		int32 launchUid = boardRows[launchY].colCardInfos[launchX];
		FEffectInfo effectInfo;
		effectInfo.launchType = allInstanceCardInfo[launchUid].originCardInfo.launchType;
		effectInfo.coolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
		effectInfo.availableTimes = allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
		effectInfo.launchGeoType = allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
		effectInfo.autoSkillTargetGeoType = allInstanceCardInfo[launchUid].originCardInfo.autoSkillTargetGeoType;
		effectInfo.targetGeoType = allInstanceCardInfo[launchUid].originCardInfo.targetGeoType;
		effectInfo.aoeType = allInstanceCardInfo[launchUid].originCardInfo.aoeType;
		effectInfo.targetCamp = allInstanceCardInfo[launchUid].originCardInfo.targetCamp;
		effectInfo.effectType = allInstanceCardInfo[launchUid].originCardInfo.effectType;
		effectInfo.effectAffix = allInstanceCardInfo[launchUid].originCardInfo.effectAffix;
		effectInfo.effectAffixCamp = allInstanceCardInfo[launchUid].originCardInfo.effectAffixCamp;
		effectInfo.prereqTagCondition = allInstanceCardInfo[launchUid].originCardInfo.prereqTagCondition;
		effectInfo.prereqTag = allInstanceCardInfo[launchUid].originCardInfo.prereqTag;
		effectInfo.prereqCampType = allInstanceCardInfo[launchUid].originCardInfo.prereqCampType;
		effectInfo.prereqType = allInstanceCardInfo[launchUid].originCardInfo.prereqType;
		effectInfo.passivePrereqType = allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
		effectInfo.values = allInstanceCardInfo[launchUid].originCardInfo.values;

		FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchPlayCardSkillDict(
			launchCamp, allInstanceCardInfo, boardRows, effectInfo, launchX, launchY, targetX, targetY, sectionZeroScores, sectionOneScores);

		if (effectResultInfo.success)
		{
			for (int32 i = 0; i < effectResultInfo.modifyGrids.Num(); i++)
			{
				if (allInstanceCardInfo[effectResultInfo.modifyUids[i]].curHp <= 0.0)
				{
					// move this card to grave
					MoveCard2Grave(allInstanceCardInfo[effectResultInfo.modifyUids[i]].camp,
						effectResultInfo.modifyGrids[i].x,
						effectResultInfo.modifyGrids[i].y,
						effectResultInfo.modifyUids[i]);
				}
			}
			effectResultInfo.triggerRound = 0;
			TriggerPassiveEffect(effectResultInfo, renderEffectRoundList);
		}
	}

	void TriggerRoundEndSkill(
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{

	}

	void TriggerManualSkill(
		uint8 launchCampNb,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{
		int32 launchUid = boardRows[launchY].colCardInfos[launchX];
		FEffectInfo effectInfo;
		effectInfo.launchType = allInstanceCardInfo[launchUid].originCardInfo.launchType;
		effectInfo.coolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
		effectInfo.availableTimes = allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
		effectInfo.launchGeoType = allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
		effectInfo.autoSkillTargetGeoType = allInstanceCardInfo[launchUid].originCardInfo.autoSkillTargetGeoType;
		effectInfo.targetGeoType = allInstanceCardInfo[launchUid].originCardInfo.targetGeoType;
		effectInfo.aoeType = allInstanceCardInfo[launchUid].originCardInfo.aoeType;
		effectInfo.targetCamp = allInstanceCardInfo[launchUid].originCardInfo.targetCamp;
		effectInfo.effectType = allInstanceCardInfo[launchUid].originCardInfo.effectType;
		effectInfo.effectAffix = allInstanceCardInfo[launchUid].originCardInfo.effectAffix;
		effectInfo.effectAffixCamp = allInstanceCardInfo[launchUid].originCardInfo.effectAffixCamp;
		effectInfo.prereqTagCondition = allInstanceCardInfo[launchUid].originCardInfo.prereqTagCondition;
		effectInfo.prereqTag = allInstanceCardInfo[launchUid].originCardInfo.prereqTag;
		effectInfo.prereqCampType = allInstanceCardInfo[launchUid].originCardInfo.prereqCampType;
		effectInfo.prereqType = allInstanceCardInfo[launchUid].originCardInfo.prereqType;
		effectInfo.passivePrereqType = allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
		effectInfo.values = allInstanceCardInfo[launchUid].originCardInfo.values;

		FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchSkillDict(
			allInstanceCardInfo, boardRows, effectInfo, launchX, launchY, targetX, targetY, sectionZeroScores, sectionOneScores);
		if (effectResultInfo.success)
		{
			if (allInstanceCardInfo[launchUid].curAvailableTimes > 0)
			{
				allInstanceCardInfo[launchUid].curAvailableTimes -= 1;
			}
			for (int32 i = 0; i < effectResultInfo.modifyGrids.Num(); i++)
			{
				if (allInstanceCardInfo[effectResultInfo.modifyUids[i]].curHp <= 0.0)
				{
					// move this card to grave
					MoveCard2Grave(allInstanceCardInfo[effectResultInfo.modifyUids[i]].camp,
						effectResultInfo.modifyGrids[i].x,
						effectResultInfo.modifyGrids[i].y,
						effectResultInfo.modifyUids[i]);
				}
			}
			effectResultInfo.triggerRound = 0;
			TriggerPassiveEffect(effectResultInfo, renderEffectRoundList);
		}
	}

	void TriggerPassiveEffect(
		FEffectResultDict effectResultDict,
		TArray<FRenderEffectRound>& renderEffectRoundList)
	{
		// Traverse all cards that are modified, try to trigger their passive effects
		for (int32 i = 0; i < effectResultDict.modifyUids.Num(); i++)
		{
			if (curRoundPassiveEffectTriggeredUids.Contains(effectResultDict.modifyUids[i]))
			{
				continue;
			}

			int32 modifyGridX = effectResultDict.modifyGrids[i].x;
			int32 modifyGridY = effectResultDict.modifyGrids[i].y;
			if (effectResultDict.modifyUids[i] != -1 &&
				allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType == "passive" &&
				effectResultDict.modifyType == allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType)
			{
				FEffectInfo secondaryEffectInfo;
				FEffectResultDict secondaryEffectResult = UPassiveEffectFunctionLibrary::GetPassiveEffect(
					allInstanceCardInfo,
					boardRows,
					secondaryEffectInfo,
					modifyGridX,
					modifyGridY,
					effectResultDict.triggerGridX,
					effectResultDict.triggerGridY,
					curRoundPassiveEffectTriggeredUids
				);
				secondaryEffectResult.triggerRound = effectResultDict.triggerRound + 1;

				FRenderEffectDict renderEffectDict(secondaryEffectResult);
				if (secondaryEffectResult.triggerRound >= renderEffectRoundList.Num())
				{
					FRenderEffectRound renderEffectRound;
					renderEffectRound.renderEffectList.Add(renderEffectDict);
					renderEffectRoundList.Add(renderEffectRound);
				}
				else
				{
					// which means there's already an equal RenderEffectRound exist
					if (renderEffectRoundList[secondaryEffectResult.triggerRound].renderTime < secondaryEffectResult.renderTime)
					{
						renderEffectRoundList[secondaryEffectResult.triggerRound].renderTime = secondaryEffectResult.renderTime;
					}
					renderEffectRoundList[secondaryEffectResult.triggerRound].renderEffectList.Add(renderEffectDict);
				}


				curRoundPassiveEffectTriggeredUids.Add(effectResultDict.modifyUids[i]);

				if (secondaryEffectResult.modifyUids.Num() > 0)
				{
					TriggerPassiveEffect(secondaryEffectResult, renderEffectRoundList);
				}
			}
		}
	}

	void TriggerMove(
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY)
	{
		int32 moveCardUid = boardRows[launchY].colCardInfos[launchX];
		boardRows[targetY].colCardInfos[targetX] = moveCardUid;
		boardRows[launchY].colCardInfos[launchX] = -1;
		allInstanceCardInfo[moveCardUid].curCol = targetX;
		allInstanceCardInfo[moveCardUid].curRow = targetY;
	}

	void MoveCard2Grave(uint8 section, int32 deadGridX, int32 deadGridY, int32 deadUid)
	{
		if (section == 0)
		{
			for (int32 i = 0; i < UGlobalConstFunctionLibrary::graveCardSectionRow; i++)
			{
				bool move2Grave = false;
				for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
				{
					int32 graveUid = boardRows[i].colCardInfos[j];
					if (graveUid == -1 || (i == UGlobalConstFunctionLibrary::graveCardSectionRow - 1 && j == UGlobalConstFunctionLibrary::maxCol - 1))
					{
						// which means this grave lot is empty
						boardRows[i].colCardInfos[j] = deadUid;
						boardRows[deadGridY].colCardInfos[deadGridX] = -1;
						move2Grave = true;
						break;
					}
				}
				if (move2Grave)
				{
					break;
				}
			}
		}
		else
		{
			for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
				UGlobalConstFunctionLibrary::boardSectionRow; 
				i < UGlobalConstFunctionLibrary::graveCardSectionRow*2 +
				UGlobalConstFunctionLibrary::playCardSectionRow * 2 + 
				UGlobalConstFunctionLibrary::boardSectionRow; 
				i++)
			{
				bool move2Grave = false;
				for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
				{
					int32 graveUid = boardRows[i].colCardInfos[j];
					if (graveUid == -1 || (i == UGlobalConstFunctionLibrary::graveCardSectionRow*2 +
						UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
						UGlobalConstFunctionLibrary::boardSectionRow - 1 && j == UGlobalConstFunctionLibrary::maxCol - 1))
					{
						// which means this grave lot is empty
						boardRows[i].colCardInfos[j] = deadUid;
						boardRows[deadGridY].colCardInfos[deadGridX] = -1;
						move2Grave = true;
						break;
					}
				}
				if (move2Grave)
				{
					break;
				}
			}
		}
	}


	void GetTestSkillLaunchTypeCoding(FString launchType, int32* coding)
	{
		if (launchType == "auto") coding[0] = 1;
		else if (launchType == "manual") coding[1] = 1;
	}

	void GetSkillLaunchTypeCoding(FString launchType, int32* coding)
	{
		if (launchType == "auto")							coding[0] = 1;
		else if (launchType == "autoRoundEnd")			coding[1] = 1;
		else if (launchType == "manual")			coding[2] = 1;
		else if (launchType == "manualImmediate")			coding[3] = 1;
		else if (launchType == "passive")					coding[4] = 1;
		else if (launchType == "playCard")				coding[5] = 1;
	}

	void GetTestAutoSkillGeoTargetCoding(FString geoTargetType, int32* coding)
	{
		if (geoTargetType == "left") coding[0] = 1;
		else if (geoTargetType == "forward") coding[1] = 1;
	}

	void GetAutoSkillGeoTargetCoding(FString geoTargetType, int32* coding)
	{
		if (geoTargetType == "self")								coding[0] = 1;
		else if (geoTargetType == "left")									coding[1] = 1;
		else if (geoTargetType == "right")								coding[2] = 1;
		else if (geoTargetType == "forward")								coding[3] = 1;
		else if (geoTargetType == "backward")									coding[4] = 1;
		else if (geoTargetType == "leftLine")								coding[5] = 1;
		else if (geoTargetType == "rightLine")							coding[6] = 1;
		else if (geoTargetType == "upLine")							coding[7] = 1;
		else if (geoTargetType == "downLine")								coding[8] = 1;
		else if (geoTargetType == "upLeft")							coding[9] = 1;
		else if (geoTargetType == "upRight")						coding[10] = 1;
		else if (geoTargetType == "downLeft")								coding[11] = 1;
		else if (geoTargetType == "downRight")							coding[12] = 1;
		else if (geoTargetType == "upLeftDiagonal")					coding[13] = 1;
		else if (geoTargetType == "upRightDiagonal")				coding[14] = 1;
		else if (geoTargetType == "downLeftDiagonal")						coding[15] = 1;
		else if (geoTargetType == "downRightDiagonal")					coding[16] = 1;
	}

	void GetTestSkillLaunchGeoCoding(FString launchGeoType, int32* coding)
	{
		if (launchGeoType == "point") coding[0] = 1;
		else if (launchGeoType == "three") coding[1] = 1;
	}

	void GetTestSkillTargetGeoCoding(FString targetGeoType, int32* coding)
	{
		if (targetGeoType == "line") coding[0] = 1;
		else if (targetGeoType == "seperate") coding[1] = 1;
	}

	void GetTestMoveTypeCoding(FString moveType, int32* coding)
	{
		if (moveType == "line") coding[0] = 1;
		else if (moveType == "diagonal") coding[1] = 1;
	}

	void GetTestAtkDistanceCoding(EAtkDistanceType atkDisType, int32* coding)
	{
		if (atkDisType == EAtkDistanceType::Closed) coding[0] = 1;
		else if (atkDisType == EAtkDistanceType::Far) coding[1] = 1;
	}

	void GetSkillLaunchGeoCoding(FString launchGeoType, int32* coding)
	{
		if (launchGeoType == "point")						coding[0] = 1;
		else if (launchGeoType == "three")				coding[1] = 1;
		else if (launchGeoType == "horn")					coding[2] = 1;
		else if (launchGeoType == "triangle")				coding[3] = 1;
		else if (launchGeoType == "obliqueCross")			coding[4] = 1;
		else if (launchGeoType == "normalCross")	coding[5] = 1;
		else if (launchGeoType == "selfSection")	coding[6] = 1;
		else if (launchGeoType == "selfFrontLine")		coding[7] = 1;
		else if (launchGeoType == "oppoSection")	coding[8] = 1;
		else if (launchGeoType == "oppoFrontLine")		coding[9] = 1;
	}
	void GetSkillTargetGeoCoding(FString targetGeoType, int32* coding)
	{
		if (targetGeoType == "line")							coding[0] = 1;
		else if (targetGeoType == "seperate")			coding[1] = 1;
		else if (targetGeoType == "diagonal")				coding[2] = 1;
		else if (targetGeoType == "diagonalSeperated") coding[3] = 1;
		else if (targetGeoType == "connect")					coding[4] = 1;
		else if (targetGeoType == "arbitrary")			coding[5] = 1;
	}
	void GetSkillTargetLocateGeoCoding(FString targetLocateGeoType, int32* coding)
	{
		if (targetLocateGeoType == "three")					coding[0] = 1;
		else if (targetLocateGeoType == "horn")				coding[1] = 1;
		else if (targetLocateGeoType == "triangle")			coding[2] = 1;
		else if (targetLocateGeoType == "obliqueCross")		coding[3] = 1;
		else if (targetLocateGeoType == "normalCross")			coding[4] = 1;
		else if (targetLocateGeoType == "selfSection")			coding[5] = 1;
		else if (targetLocateGeoType == "selfFrontLine")	coding[6] = 1;
		else if (targetLocateGeoType == "oppoSection")			coding[7] = 1;
		else if (targetLocateGeoType == "oppoFrontLine")	coding[8] = 1;
	}
	void GetSkillAoeCoding(FString aoeType, int32* coding)
	{
		if (aoeType == "point")					coding[0] = 1;
		else if (aoeType == "sweep")			coding[1] = 1;
		else if (aoeType == "H3")						coding[2] = 1;
		else if (aoeType == "V3")						coding[3] = 1;
		else if (aoeType == "normalCross")			coding[4] = 1;
		else if (aoeType == "obliqueCross")		coding[5] = 1;
	}
	void GetSkillTargetCampCoding(FString targetCampType, int32* coding)
	{
		if (targetCampType == "self")				coding[0] = 1;
		else if (targetCampType == "oppo")		coding[1] = 1;
	}

	void GetTestSkillEffectCoding(FString effectType, int32* coding)
	{
		if (effectType == "hurt") coding[0] = 1;
		else if (effectType == "heal") coding[1] = 1;
	}
	void GetSkillEffectCoding(FString effectType, int32* coding)
	{
		if (effectType == "hurt")									coding[0] = 1;
		else if (effectType == "heal")							coding[1] = 1;
		else if (effectType == "increaseDefence")					coding[2] = 1;
		else if (effectType == "replaceDefence")			coding[3] = 1;
		else if (effectType == "increaseSelfDefence")				coding[4] = 1;
		else if (effectType == "giveTempArmor")				coding[5] = 1;
		else if (effectType == "giveArmor")					coding[6] = 1;
		else if (effectType == "tempArmor")					coding[7] = 1;
		else if (effectType == "defenceHurt")						coding[8] = 1;
		else if (effectType == "defenceHeal")						coding[9] = 1;
		else if (effectType == "defenceDetonate")					coding[10] = 1;
		else if (effectType == "useArmorHurt")					coding[11] = 1;
		else if (effectType == "useArmorHeal")					coding[12] = 1;
		else if (effectType == "armorDetonate")				coding[13] = 1;
		else if (effectType == "armorDetonateSilence")			coding[14] = 1;
		else if (effectType == "armorDetonateWound")		coding[15] = 1;
		else if (effectType == "armorDetonatePoison")				coding[16] = 1;
		else if (effectType == "armorDetonateSublime")			coding[17] = 1;
		else if (effectType == "switchCamp")				coding[18] = 1;
		else if (effectType == "capture")							coding[19] = 1;
		else if (effectType == "hurtTransfer")					coding[20] = 1;
		else if (effectType == "hurtLink")						coding[21] = 1;
		else if (effectType == "healTransfer")					coding[22] = 1;
		else if (effectType == "healLink")						coding[23] = 1;
		else if (effectType == "sublimeTransfer")					coding[24] = 1;
		else if (effectType == "silenceTransfer")					coding[25] = 1;
		else if (effectType == "woundTransfer")				coding[26] = 1;
		else if (effectType == "wound")						coding[27] = 1;
		else if (effectType == "convertSublimeToWound")		coding[28] = 1;
		else if (effectType == "transferWound")				coding[29] = 1;
		else if (effectType == "revenge")							coding[30] = 1;
		else if (effectType == "revengeWound")					coding[31] = 1;
		else if (effectType == "repayHeal")					coding[32] = 1;
		else if (effectType == "repaySublime")					coding[33] = 1;
		else if (effectType == "devour")					coding[34] = 1;
		else if (effectType == "devourSublime")				coding[35] = 1;
		else if (effectType == "devourButWound")			coding[36] = 1;
		else if (effectType == "deadWishConvert")					coding[37] = 1;
		else if (effectType == "deadWishHurt")					coding[38] = 1;
		else if (effectType == "deadWishSpawnUp")					coding[39] = 1;
		else if (effectType == "deadWishSpawnDown")			coding[40] = 1;
		else if (effectType == "deadWishSpawnRight")		coding[41] = 1;
		else if (effectType == "deadWishSpawnLeft")			coding[42] = 1;
		else if (effectType == "deadWishDestroyUp")			coding[43] = 1;
		else if (effectType == "deadWishDestroyDown")				coding[44] = 1;
		else if (effectType == "deadWishDestroyRight")			coding[45] = 1;
		else if (effectType == "deadWishDestroyLeft")				coding[46] = 1;
		else if (effectType == "deadWishHeal")					coding[47] = 1;
		else if (effectType == "deadWishHealUp")			coding[48] = 1;
		else if (effectType == "deadWishHealDown")				coding[49] = 1;
		else if (effectType == "deadWishHealRight")			coding[50] = 1;
		else if (effectType == "deadWishHealLeft")				coding[51] = 1;
		else if (effectType == "deadWishSublime")					coding[52] = 1;
		else if (effectType == "deadWishTackle")			coding[53] = 1;
		else if (effectType == "deadWishWound")				coding[54] = 1;
		else if (effectType == "explode")							coding[55] = 1;
		else if (effectType == "explodeToPoison")					coding[56] = 1;
		else if (effectType == "explodeToWound")			coding[57] = 1;
		else if (effectType == "exchange")						coding[58] = 1;
		else if (effectType == "drag")							coding[59] = 1;
		else if (effectType == "push") coding[60] = 1;
		else if (effectType == "sublime") coding[61] = 1;
		else if (effectType == "lock") coding[62] = 1;
		else if (effectType == "tempLock") coding[63] = 1;
		else if (effectType == "purify") coding[64] = 1;
		else if (effectType == "spawn") coding[65] = 1;
		else if (effectType == "cloneUp") coding[66] = 1;
		else if (effectType == "cloneDown") coding[67] = 1;
		else if (effectType == "cloneRight") coding[68] = 1;
		else if (effectType == "cloneLeft") coding[69] = 1;
		else if (effectType == "silence") coding[70] = 1;
		else if (effectType == "tempSilence") coding[71] = 1;
		else if (effectType == "copyHp") coding[72] = 1;
		else if (effectType == "summonFirstDeath") coding[73] = 1;
		else if (effectType == "exchangeFirstDeath") coding[74] = 1;
		else if (effectType == "tackle") coding[75] = 1;
		else if (effectType == "summonFirstPile") coding[76] = 1;
		else if (effectType == "exchangeFirstPile") coding[77] = 1;
		else if (effectType == "duel") coding[78] = 1;
		else if (effectType == "recover") coding[79] = 1;
		else if (effectType == "assignHp") coding[80] = 1;
		else if (effectType == "copyGraveFirstHp") coding[81] = 1;
		else if (effectType == "healFromWound") coding[82] = 1;
		else if (effectType == "copyFirstGraveHp") coding[83] = 1;
		else if (effectType == "increaseFirstDeathHpDefence") coding[84] = 1;
		else if (effectType == "increaseFirstPileHp") coding[85] = 1;

	}
	void GetSkillAffixCampCoding(FString affixCampType, int32* coding)
	{
		if (affixCampType == "self") coding[0] = 1;
		else if (affixCampType == "oppo") coding[1] = 1;
		else if (affixCampType == "none") coding[2] = 1;
	}
	void GetSkillAffixCoding(FString affixType, int32* coding)
	{
		
	}
	void GetSkillTagConditionCoding(FString tagCondition, int32* coding)
	{
		
	}
	void GetCardTagCoding(FString tag, int32* coding)
	{
		
	}
	void GetPrereqTagCoding(FString prereqTag, int32* coding)
	{
		
	}
	void GetPrereqCampCoding(FString prereqCampType, int32* coding)
	{
		
	}
	void GetSkillPrereqCoding(FString skillPrereq, int32* coding)
	{
		
	}
	void GetPassiveSkillPrereqTypeCoding(FString passiveSkillPrereq, int32* coding)
	{
		
	}

	bool GameEnd(int32& winner)
	{
		if (sectionZeroHandCards.Num() <= 0 && sectionOneHandCards.Num() <= 0)
		{
			if (sectionZeroScores > sectionOneScores)
			{
				winner = 0;
				return true;
			}
			else if (sectionOneScores > sectionZeroScores)
			{
				winner = 1;
				return true;
			}
			else
			{
				winner = -1;
				return true;
			}
		}
		else
		{
			winner = -1;
			return false;
		}
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FTritonResponseData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	UMctsTreeNode* curMctsTreeNode;

	UPROPERTY()
	FBoardInfo curBoardInfo;

	//uint8 curPlayingSectionNb;

	UPROPERTY()
	TArray<float> policies;

	float boardValue;
};

USTRUCT()
struct FTrainDataStateCodingAndActionProbs
{
	GENERATED_USTRUCT_BODY()
public:
	int32 stateCoding[StateCodingTotalCHW] = { 0 };

	TMap<int32, float> actionProbs;
};

USTRUCT(BlueprintType, Blueprintable)
struct FTrainingData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	TArray<FTrainDataStateCodingAndActionProbs> stateCodingAndActionProbs;

	UPROPERTY()
	TArray<uint8> playSectionNbs;

	UPROPERTY()
	TArray<float> scores;
};

UCLASS(Blueprintable)
class GWENBOARDGAMECLIENT_API UMcts : public UObject
{
	GENERATED_BODY()
	
public:	

protected:

public:
	UPROPERTY(EditAnywhere)
	bool isTraining = true;

	int32 maxSelfPlayLoop = 2000;

	int32 curSelfPlayLoop = 0;

	UPROPERTY()
	int32 curSimulationMove = 0;

	UPROPERTY()
	int32 expandSimulationMoves = 1;

	UPROPERTY()
	FBoardInfo realBoard;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMctsTreeNode> mctsTreeNodeBPClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTritonHttpClient> tritonHttpClientBPClass;

	UPROPERTY()
	UMctsTreeNode* veryFirstNode;

	UPROPERTY()
	TArray<UMctsTreeNode*> newAddNodes;

	UPROPERTY()
	UMctsTreeNode* treeRoot;

	UPROPERTY()
	TArray<UMctsTreeNode*> finishSelfPlayGameTreeRoots;

	UPROPERTY()
	UTritonHttpClient* tritonHttpClient;

	int32 curTritonRequestID = 0;

	bool receivedTritonResponse = false;

	UPROPERTY()
	FTritonResponseData tritonResponseData;

	FTrainingData curTrainingData;

	//UPROPERTY()
	//TArray<FTrainingData> trainingDatas;


	void InitMcts();

	void ResetMcts();

	void UpdateCurSearchNode(int32 targetMove);

	int32 GetCurTritonRequestID();

	bool CheckTritonReponseAll();

	void SendTritonRequest();

	void SendTestTritonRequest();

	void GetTritonAction(
		int32& actionId, 
		ActionType& outActionType, 
		uint8& outSectionNb,
		TMap<int32, float>& trainDataActionProbsMap);

	void SaveTrainingData(const TArray<FTrainingData>& trainingDatas, 
		int32 channelNb,
		int32 height,
		int32 width);

	void SaveTestStateData(int32* stateCoding);

	void AddTrainingData(const FTrainingData& trainingData,
		int32 channelNb,
		int32 height,
		int32 width);

	void ClearTree(UMctsTreeNode* curTreeNode);

	/**
	* Testing part
	*/
};
