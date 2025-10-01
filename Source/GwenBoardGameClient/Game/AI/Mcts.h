// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GlobalConstFunctionLibrary.h"
#include "../CheckLaunchGeoLibrary.h"
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

	ActionType lastStepActionType = ActionType::EndRound;
	int32 lastActionId;

	void ResetBoard()
	{
		boardRows.Empty();
		allInstanceCardInfo.Empty();
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

		//GetPotentialEffectCoding(boardCoding, channelLen, curSectionNb);
		for (int32 i = 0; i < boardRows.Num(); i++)
		{
			for (int32 j = 0; j < boardRows[i].colCardInfos.Num(); j++)
			{
				int32 posInChannel = i * UGlobalConstFunctionLibrary::maxCol + j;

				int32 sectionTagStartChannelNb = 85;
				if (curSectionNb == 0)
				{
					boardCoding[channelLen * sectionTagStartChannelNb + posInChannel] = 1;
				}
				else
				{
					boardCoding[channelLen * sectionTagStartChannelNb + posInChannel] = -1;
				}

				int32 hpDiffStartChannelNb = 86;
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
				int32 launchGeoType[10] = { 0 };
				int32 targetGeoType[12] = { 0 };
				int32 moveType[3] = { 0 };
				int32 aoeType[4] = { 0 };
				int32 skillType[22] = { 0 };
				int32 prereqType[23] = { 0 };
				int32 affixType[6] = { 0 };


				GetSkillLaunchGeoCoding(allInstanceCardInfo[uid].originCardInfo.launchGeoType, launchGeoType);
				GetSkillTargetGeoCoding(allInstanceCardInfo[uid].originCardInfo.targetGeoType, targetGeoType);
				GetMoveTypeCoding(allInstanceCardInfo[uid].originCardInfo.moveType, moveType);
				GetSkillAoeCoding(allInstanceCardInfo[uid].originCardInfo.aoeType, aoeType);
				GetSkillEffectCoding(allInstanceCardInfo[uid].originCardInfo.effectType, skillType);
				GetPrereqTypeCoding(allInstanceCardInfo[uid].originCardInfo.prereqType, prereqType);
				GetAffixTypeCoding(allInstanceCardInfo[uid].originCardInfo.effectAffix, affixType);

				// There are 10 channels, which means there are 10 images
				// Every image size is W x H


				int32 graveStartChannelNb = 0;
				int32 playSectionChannelNb = 1;
				int32 boardSectionChannelNb = 2;
				if (i < UGlobalConstFunctionLibrary::graveCardSectionRow ||
					i >= UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow*2+
					UGlobalConstFunctionLibrary::boardSectionRow)
				{
					// which means it's a grave card, put it into grave channel
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * graveStartChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * graveStartChannelNb + posInChannel] = -1;
					}
				}
				else if ((i >= UGlobalConstFunctionLibrary::graveCardSectionRow &&
					i < UGlobalConstFunctionLibrary::graveCardSectionRow + 
					UGlobalConstFunctionLibrary::playCardSectionRow) ||
					(i >= UGlobalConstFunctionLibrary::graveCardSectionRow + 
						UGlobalConstFunctionLibrary::playCardSectionRow +
						UGlobalConstFunctionLibrary::boardSectionRow &&
						i < UGlobalConstFunctionLibrary::graveCardSectionRow + 
						UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
						UGlobalConstFunctionLibrary::boardSectionRow))
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * playSectionChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * playSectionChannelNb + posInChannel] = -1;
					}
				}
				else
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * boardSectionChannelNb + posInChannel] = 1;
					}
					else
					{
						boardCoding[channelLen * boardSectionChannelNb + posInChannel] = -1;
					}
				}

				int32 hpStartChannelNb = 3;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * hpStartChannelNb + posInChannel] = allInstanceCardInfo[uid].curHp;
				}
				else
				{
					boardCoding[channelLen * hpStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].curHp;
				}

				int32 defenceStartChannelNb = 4;
				if (allInstanceCardInfo[uid].camp == curSectionNb)
				{
					boardCoding[channelLen * defenceStartChannelNb + posInChannel] = allInstanceCardInfo[uid].curDefence;
				}
				else
				{
					boardCoding[channelLen * defenceStartChannelNb + posInChannel] = -allInstanceCardInfo[uid].curDefence;
				}


				int32 launchGeoTypeStartChannelNb = 5;
				for (int32 k = 0; k < 10; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + launchGeoTypeStartChannelNb) + posInChannel] = launchGeoType[k];
					}
					else
					{
						boardCoding[channelLen * (k + launchGeoTypeStartChannelNb) + posInChannel] = -launchGeoType[k];
					}
				}

				int32 targetGeoTypeStartChannelNb = 15;
				for (int32 k = 0; k < 11; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + targetGeoTypeStartChannelNb) + posInChannel] = targetGeoType[k];
					}
					else
					{
						boardCoding[channelLen * (k + targetGeoTypeStartChannelNb) + posInChannel] = -targetGeoType[k];
					}
				}

				int32 moveTypeStartChannelNb = 27;
				for (int32 k = 0; k < 3; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + moveTypeStartChannelNb) + posInChannel] =  moveType[k];
					}
					else
					{
						boardCoding[channelLen * (k + moveTypeStartChannelNb) + posInChannel] = -moveType[k];
					}
				}

				int32 aoeTypeStartChannelNb = 30;
				for (int32 k = 0; k < 4; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + aoeTypeStartChannelNb) + posInChannel] = aoeType[k];
					}
					else
					{
						boardCoding[channelLen * (k + aoeTypeStartChannelNb) + posInChannel] = -aoeType[k];
					}
				}

				int32 skillTypeStartChannelNb = 34;
				for (int32 k = 0; k < 22; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + skillTypeStartChannelNb) + posInChannel] = skillType[k];
					}
					else
					{
						boardCoding[channelLen * (k + skillTypeStartChannelNb) + posInChannel] = -skillType[k];
					}
				}

				int32 prereqTypeStartChannelNb = 56;
				for (int32 k = 0; k < 23; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + prereqTypeStartChannelNb) + posInChannel] = prereqType[k];
					}
					else
					{
						boardCoding[channelLen * (k + prereqTypeStartChannelNb) + posInChannel] = -prereqType[k];
					}
				}

				int32 affixTypeStartChannelNb = 79;
				for (int32 k = 0; k < 6; k++)
				{
					if (allInstanceCardInfo[uid].camp == curSectionNb)
					{
						boardCoding[channelLen * (k + prereqTypeStartChannelNb) + posInChannel] = affixType[k];
					}
					else
					{
						boardCoding[channelLen * (k + prereqTypeStartChannelNb) + posInChannel] = -affixType[k];
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

	bool CheckLaunchSkillLegality(int32 launchX, int32 launchY, int32 targetX, int32 targetY)
	{
		int32 launchId = boardRows[launchY].colCardInfos[launchX];
		if (launchId == -1)
		{
			return false;
		}

		FInstanceCardInfo launchCardInfo = allInstanceCardInfo[boardRows[launchY].colCardInfos[launchX]];

		bool manualSkillAvailable = false;
		if (launchCardInfo.originCardInfo.launchType == "manual")
		{
			if (launchCardInfo.originCardInfo.coolDown == -1)
			{
				if (launchCardInfo.originCardInfo.availableTimes == -1)
				{
					manualSkillAvailable = true;
				}
				else
				{
					if (launchCardInfo.curAvailableTimes > 0)
					{
						manualSkillAvailable = true;
					}
				}
			}
			else
			{
				if (launchCardInfo.curCoolDown == 0 &&
					(launchCardInfo.originCardInfo.availableTimes == -1 ||
						launchCardInfo.curAvailableTimes > 0))
				{
					manualSkillAvailable = true;
				}
			}
		}

		bool extraTagPrereq = true;
		if (launchCardInfo.curExtraTags.Contains("silent"))
		{
			extraTagPrereq = false;
		}

		if (manualSkillAvailable && extraTagPrereq)
		{
			FEffectInfo effectInfo;
			effectInfo.aoeType = launchCardInfo.originCardInfo.aoeType;
			effectInfo.availableTimes = launchCardInfo.originCardInfo.availableTimes;
			effectInfo.coolDown = launchCardInfo.originCardInfo.coolDown;
			effectInfo.effectAffix = launchCardInfo.originCardInfo.effectAffix;
			effectInfo.effectAffixCamp = launchCardInfo.originCardInfo.effectAffixCamp;
			effectInfo.effectType = launchCardInfo.originCardInfo.effectType;
			effectInfo.launchGeoType = launchCardInfo.originCardInfo.launchGeoType;
			effectInfo.launchType = launchCardInfo.originCardInfo.launchType;
			effectInfo.passivePrereqType = launchCardInfo.originCardInfo.passivePrereqType;
			effectInfo.prereqCampType = launchCardInfo.originCardInfo.prereqCampType;
			effectInfo.prereqTag = launchCardInfo.originCardInfo.prereqTag;
			effectInfo.prereqTagCondition = launchCardInfo.originCardInfo.prereqTagCondition;
			effectInfo.prereqType = launchCardInfo.originCardInfo.prereqType;
			effectInfo.prereqValue = launchCardInfo.originCardInfo.prereqValue;
			effectInfo.targetCamp = launchCardInfo.originCardInfo.targetCamp;
			effectInfo.targetGeoType = launchCardInfo.originCardInfo.targetGeoType;
			effectInfo.values = launchCardInfo.originCardInfo.values;
			effectInfo.renderEffectType = launchCardInfo.originCardInfo.renderEffectType;
			effectInfo.renderEffectTime = launchCardInfo.originCardInfo.renderEffectTime;

			bool prereqPass = false;
			if (UCheckLaunchGeoLibrary::CheckLaunchGeoType(effectInfo.launchGeoType,
				allInstanceCardInfo,
				boardRows,
				launchX,
				launchY))
			{
				prereqPass = true;
			}

			if (effectInfo.prereqType == "none" ||
				UCheckPrereqFunctionLibrary::CheckPrereqRule(
					allInstanceCardInfo,
					boardRows,
					effectInfo.prereqType,
					launchX,
					launchY,
					launchCardInfo.camp,
					effectInfo.prereqCampType,
					effectInfo.prereqValue))
			{
				prereqPass = true;
			}

			if (prereqPass &&
				(effectInfo.prereqTagCondition == "none" ||
					UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
						allInstanceCardInfo,
						boardRows,
						effectInfo,
						launchX,
						launchY)))
			{
				prereqPass = true;
			}

			if (prereqPass 
				&& 
				UCheckPrereqTagFunctionLibrary::CheckTargetPrereqTagRule(allInstanceCardInfo,
				boardRows,
				effectInfo,
				targetX,
				targetY)
				&&
				UCheckTargetGeoRuleLibrary::CheckTargetSkillGeoLegality(effectInfo,
					allInstanceCardInfo,
					boardRows,
					launchX,
					launchY,
					targetX,
					targetY))
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
			return false;
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

					bool extraTagPrereq = true;
					if (cardInfo.curExtraTags.Contains("silent"))
					{
						extraTagPrereq = false;
					}

					if (manualSkillAvailable && extraTagPrereq)
					{
						FEffectInfo effectInfo;
						effectInfo.aoeType = cardInfo.originCardInfo.aoeType;
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
						effectInfo.prereqValue = cardInfo.originCardInfo.prereqValue;
						effectInfo.targetCamp = cardInfo.originCardInfo.targetCamp;
						effectInfo.targetGeoType = cardInfo.originCardInfo.targetGeoType;
						effectInfo.values = cardInfo.originCardInfo.values;
						effectInfo.renderEffectType = cardInfo.originCardInfo.renderEffectType;
						effectInfo.renderEffectTime = cardInfo.originCardInfo.renderEffectTime;

						bool prereqPass = false;
						if (UCheckLaunchGeoLibrary::CheckLaunchGeoType(effectInfo.launchGeoType,
							allInstanceCardInfo,
							boardRows,
							col,
							checkRow))
						{
							prereqPass = true;
						}

						if (effectInfo.prereqType == "none" || 
							UCheckPrereqFunctionLibrary::CheckPrereqRule(
								allInstanceCardInfo,
								boardRows,
								effectInfo.prereqType,
								col,
								checkRow,
								cardInfo.camp,
								effectInfo.prereqCampType,
								effectInfo.prereqValue))
						{
							prereqPass = true;
						}

						if (prereqPass && 
							(effectInfo.prereqTagCondition == "none" ||
							UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
								allInstanceCardInfo,
								boardRows,
								effectInfo,
								col,
								checkRow)))
						{
							prereqPass = true;
						}

						if (prereqPass)
						{
							TArray<FGridXY> possibleGrids;
							possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
								effectInfo.targetGeoType,
								allInstanceCardInfo,
								boardRows,
								effectInfo,
								col,
								checkRow,
								cardInfo.originCardInfo.attackDistanceType);

							for (int32 i = 0; i < possibleGrids.Num(); i++)
							{
								if (UCheckPrereqTagFunctionLibrary::CheckTargetPrereqTagRule(
									allInstanceCardInfo,
									boardRows,
									effectInfo,
									possibleGrids[i].x,
									possibleGrids[i].y) 
									&&
									UCheckTargetGeoRuleLibrary::CheckTargetSkillGeoLegality(
										effectInfo,
										allInstanceCardInfo,
										boardRows,
										col,
										checkRow,
										possibleGrids[i].x,
										possibleGrids[i].y))
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
		FRenderActionNode& renderParentNode)
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
				renderParentNode);
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
				renderParentNode);
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
			TriggerMove(launchX, launchY, targetX, targetY, renderParentNode);
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
			TriggerRoundEndSkill(curPlayingSectionNb, renderParentNode);
			UpdateCardAttachInfos();
			UpdateCardAttachEffects();
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

	void UpdateCardAttachEffects()
	{
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow;
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow;
			i++)
		{
			for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
			{
				int32 uid = boardRows[i].colCardInfos[j];
				if (uid == -1)
				{
					continue;
				}
				if (allInstanceCardInfo[uid].curExtraTags.Contains("wound"))
				{
					allInstanceCardInfo[uid].curHp = allInstanceCardInfo[uid].curHp - 1;
					if (allInstanceCardInfo[uid].camp == 0)
					{
						sectionZeroScores -= 1;
					}
					else
					{
						sectionOneScores -= 1;
					}

					if (allInstanceCardInfo[uid].curHp <= 0.0)
					{
						// move this card to grave
						MoveCard2Grave(allInstanceCardInfo[uid].camp, j, i, uid);
					}
				}
			}
		}
	}


	void UpdateCardAttachInfos()
	{
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow;
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow;
			i++)
		{
			for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
			{
				int32 uid = boardRows[i].colCardInfos[j];
				if (uid == -1)
				{
					continue;
				}
				if (allInstanceCardInfo[uid].curCoolDown > 0)
				{
					allInstanceCardInfo[uid].curCoolDown -= 1;
					if (allInstanceCardInfo[uid].curCoolDown < 0)
					{
						allInstanceCardInfo[uid].curCoolDown = allInstanceCardInfo[uid].originCardInfo.coolDown;
					}
				}

				TArray<FString> deleteTags;
				for (TMap<FString, int32>::TConstIterator iter = allInstanceCardInfo[uid].curExtraTags.CreateConstIterator(); iter; ++iter)
				{
					if (iter->Value > 0)
					{
						allInstanceCardInfo[uid].curExtraTags[iter->Key] = iter->Value - 1;
					}
					else
					{
						deleteTags.Add(iter->Key);
					}
				}

				for (int32 k = 0; k < deleteTags.Num(); k++)
				{
					allInstanceCardInfo[uid].curExtraTags.Remove(deleteTags[k]);
				}
			}
		}
	}

	void RefreshPassiveEffectTriggerTags()
	{
		for (TMap<int32, FInstanceCardInfo>::TConstIterator iter = allInstanceCardInfo.CreateConstIterator(); iter; ++iter)
		{
			if (iter->Value.passiveEffectTriggerThisRound)
			{
				allInstanceCardInfo[iter->Key].passiveEffectTriggerThisRound = false;
			}
		}
	}



	void TriggerPlayCard(
		bool testInference,
		uint8 launchCampNb,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		FRenderActionNode& renderParentNode)
	{
		int32 triggerCardId = boardRows[launchY].colCardInfos[launchX];
		renderParentNode.actionType = ActionType::PlayCard;
		renderParentNode.renderTime = defaultRenderPlayCardInterval;
		renderParentNode.renderRound = 0;
		renderParentNode.triggerCardId = triggerCardId;
		renderParentNode.triggerGridX = launchX;
		renderParentNode.triggerGridY = launchY;
		renderParentNode.targetGridXs.Add(targetX);
		renderParentNode.targetGridYs.Add(targetY);
		

		int32 playCardUid = boardRows[launchY].colCardInfos[launchX];
		// Trigger play card skill
		if (allInstanceCardInfo[playCardUid].originCardInfo.launchType == "auto")
		{
			TriggerPlayCardSkill(launchCampNb, launchX, launchY, targetX, targetY, renderParentNode);
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
		FRenderActionNode& renderParentNode)
	{
		int32 launchUid = boardRows[launchY].colCardInfos[launchX];
		FEffectInfo effectInfo;
		effectInfo.launchType = allInstanceCardInfo[launchUid].originCardInfo.launchType;
		effectInfo.coolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
		effectInfo.availableTimes = allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
		effectInfo.launchGeoType = allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
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
		effectInfo.prereqValue = allInstanceCardInfo[launchUid].originCardInfo.prereqValue;
		effectInfo.passivePrereqType = allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
		effectInfo.values = allInstanceCardInfo[launchUid].originCardInfo.values;
		effectInfo.renderEffectType = allInstanceCardInfo[launchUid].originCardInfo.renderEffectType;
		effectInfo.renderEffectTime = allInstanceCardInfo[launchUid].originCardInfo.renderEffectTime;

		bool prereqPass = false;
		if (UCheckLaunchGeoLibrary::CheckLaunchGeoType(effectInfo.launchGeoType,
			allInstanceCardInfo,
			boardRows,
			launchX,
			launchY))
		{
			prereqPass = true;
		}


		if (effectInfo.prereqType == "none" ||
			UCheckPrereqFunctionLibrary::CheckPrereqRule(
				allInstanceCardInfo,
				boardRows,
				effectInfo.prereqType,
				launchX,
				launchY,
				launchCamp,
				effectInfo.prereqCampType,
				effectInfo.prereqValue))
		{
			prereqPass = true;
		}

		if (prereqPass &&
			(effectInfo.prereqTagCondition == "none" ||
				UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
					allInstanceCardInfo,
					boardRows,
					effectInfo,
					launchX,
					launchY)))
		{
			prereqPass = true;
		}

		if (prereqPass)
		{
			FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchPlayCardSkillDict(
				launchCamp, allInstanceCardInfo, boardRows, effectInfo, launchX, launchY, targetX, targetY, false, sectionZeroScores, sectionOneScores);

			if (effectResultInfo.success)
			{
				int32 triggerGridId = boardRows[effectResultInfo.triggerGridY].colCardInfos[effectResultInfo.triggerGridX];
				FRenderActionNode renderActionNode;
				renderActionNode.actionType = ActionType::LaunchSkill;
				renderActionNode.renderTime = effectResultInfo.renderTime;
				renderActionNode.renderRound = renderParentNode.renderRound + 1;
				renderActionNode.renderEffectType = effectResultInfo.renderEffectType;
				renderActionNode.triggerCardId = triggerGridId;
				renderActionNode.triggerGridX = effectResultInfo.triggerGridX;
				renderActionNode.triggerGridY = effectResultInfo.triggerGridY;
				renderActionNode.modifyUids = effectResultInfo.modifyUids;
				renderActionNode.modifyValues = effectResultInfo.modifyValues;
				for (int32 i = 0; i < effectResultInfo.modifyGrids.Num(); i++)
				{
					renderActionNode.targetGridXs.Add(effectResultInfo.modifyGrids[i].x);
					renderActionNode.targetGridYs.Add(effectResultInfo.modifyGrids[i].y);
				}

				
				TriggerPassiveEffect(launchX, launchY, effectResultInfo, renderActionNode);
				renderParentNode.children.Add(renderActionNode);
				RefreshPassiveEffectTriggerTags();

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
				
				
			}
		}
		
	}

	void TriggerRoundEndSkill(uint8 curSection, FRenderActionNode& renderParentNode)
	{
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow;
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow;
			i++)
		{
			for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
			{
				int32 launchUid = boardRows[i].colCardInfos[j];
				if (launchUid == -1)
				{
					continue;
				}

				if (allInstanceCardInfo[launchUid].curExtraTags.Contains("silent"))
				{
					continue;
				}

				if (allInstanceCardInfo[launchUid].originCardInfo.launchType == "autoRoundEnd")
				{
					FEffectInfo effectInfo;
					effectInfo.launchType = allInstanceCardInfo[launchUid].originCardInfo.launchType;
					effectInfo.coolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
					effectInfo.availableTimes = allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
					effectInfo.launchGeoType = allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
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
					effectInfo.prereqValue = allInstanceCardInfo[launchUid].originCardInfo.prereqValue;
					effectInfo.passivePrereqType = allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
					effectInfo.values = allInstanceCardInfo[launchUid].originCardInfo.values;
					effectInfo.renderEffectType = allInstanceCardInfo[launchUid].originCardInfo.renderEffectType;
					effectInfo.renderEffectTime = allInstanceCardInfo[launchUid].originCardInfo.renderEffectTime;

					bool prereqPass = false;
					if (UCheckLaunchGeoLibrary::CheckLaunchGeoType(effectInfo.launchGeoType,
						allInstanceCardInfo,
						boardRows,
						j,
						i))
					{
						prereqPass = true;
					}


					if (effectInfo.prereqType == "none" ||
						UCheckPrereqFunctionLibrary::CheckPrereqRule(
							allInstanceCardInfo,
							boardRows,
							effectInfo.prereqType,
							j,
							i,
							allInstanceCardInfo[launchUid].camp,
							effectInfo.prereqCampType,
							effectInfo.prereqValue))
					{
						prereqPass = true;
					}

					if (prereqPass &&
						(effectInfo.prereqTagCondition == "none" ||
							UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
								allInstanceCardInfo,
								boardRows,
								effectInfo,
								j,
								i)))
					{
						prereqPass = true;
					}

					if (prereqPass)
					{
						FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchRoundEndSkillDict(
							allInstanceCardInfo[launchUid].camp, allInstanceCardInfo, boardRows, effectInfo, j, i, false, sectionZeroScores, sectionOneScores);
						if (effectResultInfo.success)
						{
							int32 triggerGridId = boardRows[i].colCardInfos[j];
							FRenderActionNode renderActionNode;
							renderActionNode.actionType = ActionType::LaunchSkill;
							renderActionNode.renderTime = effectResultInfo.renderTime;
							renderActionNode.renderRound = renderParentNode.renderRound + 1;
							renderActionNode.renderEffectType = effectResultInfo.renderEffectType;
							renderActionNode.triggerCardId = triggerGridId;
							renderActionNode.triggerGridX = j;
							renderActionNode.triggerGridY = i;
							renderActionNode.modifyUids = effectResultInfo.modifyUids;
							renderActionNode.modifyValues = effectResultInfo.modifyValues;
							for (int32 k = 0; k < effectResultInfo.modifyGrids.Num(); k++)
							{
								renderActionNode.targetGridXs.Add(effectResultInfo.modifyGrids[k].x);
								renderActionNode.targetGridYs.Add(effectResultInfo.modifyGrids[k].y);
							}


							if (allInstanceCardInfo[launchUid].curAvailableTimes > 0)
							{
								allInstanceCardInfo[launchUid].curAvailableTimes -= 1;
							}
							if (allInstanceCardInfo[launchUid].curCoolDown == 0)
							{
								allInstanceCardInfo[launchUid].curCoolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
							}

							
							TriggerPassiveEffect(j, i, effectResultInfo, renderActionNode);
							renderParentNode.children.Add(renderActionNode);
							RefreshPassiveEffectTriggerTags();

							for (int32 k = 0; k < effectResultInfo.modifyGrids.Num(); k++)
							{
								if (allInstanceCardInfo[effectResultInfo.modifyUids[k]].curHp <= 0.0)
								{
									// move this card to grave
									MoveCard2Grave(allInstanceCardInfo[effectResultInfo.modifyUids[k]].camp,
										effectResultInfo.modifyGrids[k].x,
										effectResultInfo.modifyGrids[k].y,
										effectResultInfo.modifyUids[k]);
								}
							}
							
							
						}
					}
				}
			}
		}
	}

	void TriggerManualSkill(
		uint8 launchCampNb,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		FRenderActionNode& renderParentNode)
	{
		int32 launchUid = boardRows[launchY].colCardInfos[launchX];

		if (allInstanceCardInfo[launchUid].curExtraTags.Contains("silent"))
		{
			return;
		}

		FEffectInfo effectInfo;
		effectInfo.launchType = allInstanceCardInfo[launchUid].originCardInfo.launchType;
		effectInfo.coolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
		effectInfo.availableTimes = allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
		effectInfo.launchGeoType = allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
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
		effectInfo.prereqValue = allInstanceCardInfo[launchUid].originCardInfo.prereqValue;
		effectInfo.passivePrereqType = allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
		effectInfo.values = allInstanceCardInfo[launchUid].originCardInfo.values;
		effectInfo.renderEffectType = allInstanceCardInfo[launchUid].originCardInfo.renderEffectType;
		effectInfo.renderEffectTime = allInstanceCardInfo[launchUid].originCardInfo.renderEffectTime;

		bool prereqPass = false;
		if (UCheckLaunchGeoLibrary::CheckLaunchGeoType(effectInfo.launchGeoType,
			allInstanceCardInfo,
			boardRows,
			launchX,
			launchY))
		{
			prereqPass = true;
		}


		if (effectInfo.prereqType == "none" ||
			UCheckPrereqFunctionLibrary::CheckPrereqRule(
				allInstanceCardInfo,
				boardRows,
				effectInfo.prereqType,
				launchX,
				launchY,
				allInstanceCardInfo[launchUid].camp,
				effectInfo.prereqCampType,
				effectInfo.prereqValue))
		{
			prereqPass = true;
		}

		if (prereqPass &&
			(effectInfo.prereqTagCondition == "none" ||
				UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
					allInstanceCardInfo,
					boardRows,
					effectInfo,
					launchX,
					launchY)))
		{
			prereqPass = true;
		}

		if (prereqPass)
		{
			FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchSkillDict(
				allInstanceCardInfo, boardRows, effectInfo, launchX, launchY, targetX, targetY, false, sectionZeroScores, sectionOneScores);
			if (effectResultInfo.success)
			{
				int32 triggerGridId = boardRows[effectResultInfo.triggerGridY].colCardInfos[effectResultInfo.triggerGridX];
				FRenderActionNode renderActionNode;
				renderActionNode.actionType = ActionType::LaunchSkill;
				renderActionNode.renderTime = effectResultInfo.renderTime;
				renderActionNode.renderRound = 0;
				renderActionNode.renderEffectType = effectResultInfo.renderEffectType;
				renderActionNode.triggerCardId = triggerGridId;
				renderActionNode.triggerGridX = effectResultInfo.triggerGridX;
				renderActionNode.triggerGridY = effectResultInfo.triggerGridY;
				renderActionNode.modifyUids = effectResultInfo.modifyUids;
				renderActionNode.modifyValues = effectResultInfo.modifyValues;
				for (int32 i = 0; i < effectResultInfo.modifyGrids.Num(); i++)
				{
					renderActionNode.targetGridXs.Add(effectResultInfo.modifyGrids[i].x);
					renderActionNode.targetGridYs.Add(effectResultInfo.modifyGrids[i].y);
				}
				renderParentNode = renderActionNode;


				if (allInstanceCardInfo[launchUid].curAvailableTimes > 0)
				{
					allInstanceCardInfo[launchUid].curAvailableTimes -= 1;
				}
				if (allInstanceCardInfo[launchUid].curCoolDown == 0)
				{
					allInstanceCardInfo[launchUid].curCoolDown = allInstanceCardInfo[launchUid].originCardInfo.coolDown;
				}

				
				TriggerPassiveEffect(launchX, launchY, effectResultInfo, renderParentNode);
				RefreshPassiveEffectTriggerTags();

				for (int32 i = 0; i < effectResultInfo.modifyGrids.Num(); i++)
				{
					if (effectResultInfo.modifyUids[i] != -1 &&
						allInstanceCardInfo[effectResultInfo.modifyUids[i]].curHp <= 0.0)
					{
						// move this card to grave
						MoveCard2Grave(allInstanceCardInfo[effectResultInfo.modifyUids[i]].camp,
							effectResultInfo.modifyGrids[i].x,
							effectResultInfo.modifyGrids[i].y,
							effectResultInfo.modifyUids[i]);
					}
				}
				
				
			}
		}
		
	}

	/*
	void TriggerPassivePotentialCoding(
		int32* boardCoding,
		int32 channelLen)
	{
		for (int32 i = 0; i < boardRows.Num(); i++)
		{
			for (int32 j = 0; j < boardRows[i].colCardInfos.Num(); j++)
			{
				int32 uid = boardRows[i].colCardInfos[j];
				if (uid == -1 || allInstanceCardInfo[uid].originCardInfo.launchType != "passive")
				{
					continue;
				}

				FEffectInfo passiveEffectInfo;
				passiveEffectInfo.launchType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType;
				passiveEffectInfo.coolDown = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.coolDown;
				passiveEffectInfo.availableTimes = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.availableTimes;
				passiveEffectInfo.launchGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchGeoType;
				passiveEffectInfo.targetGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetGeoType;
				passiveEffectInfo.aoeType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.aoeType;
				passiveEffectInfo.targetCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetCamp;
				passiveEffectInfo.effectType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectType;
				passiveEffectInfo.effectAffix = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffix;
				passiveEffectInfo.effectAffixCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffixCamp;
				passiveEffectInfo.prereqTagCondition = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTagCondition;
				passiveEffectInfo.prereqTag = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTag;
				passiveEffectInfo.prereqCampType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqCampType;
				passiveEffectInfo.prereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqType;
				passiveEffectInfo.prereqValue = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqValue;
				passiveEffectInfo.passivePrereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType;
				passiveEffectInfo.values = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.values;

				TArray<FGridXY> possibleGrids = UCheckTargetGeoRuleLibrary::GetPassiveSkillTargetGrids(allInstanceCardInfo[uid].camp,
					allInstanceCardInfo,
					boardRows,
					j,
					i,
					passiveEffectInfo.targetGeoType,
					passiveEffectInfo.targetCamp);
				
				for (int32 k = 0; k < possibleGrids.Num(); k++)
				{
					int32 possiblePassiveTargetGridPosInChannel = possibleGrids[k].y * UGlobalConstFunctionLibrary::maxCol + 
						possibleGrids[k].x;
					if (passiveEffectInfo.effectType == "revenge")
					{
						boardCoding[channelLen * 49 + possiblePassiveTargetGridPosInChannel] += 1;
					}
					else if ()
				}
			}
		}
	}*/

	void TriggerPotentialPassiveEffectCoding(
		int32* boardCoding,
		int32 channelLen,
		int32 triggerX,
		int32 triggerY,
		FEffectResultDict effectResultDict,
		TArray<int32>& alreadyTriggerPassiveUids)
	{
		for (int32 i = 0; i < effectResultDict.modifyUids.Num(); i++)
		{
			if (alreadyTriggerPassiveUids.Contains(effectResultDict.modifyUids[i]))
			{
				continue;
			}
			alreadyTriggerPassiveUids.Add(effectResultDict.modifyUids[i]);

			int32 modifyGridX = effectResultDict.modifyGrids[i].x;
			int32 modifyGridY = effectResultDict.modifyGrids[i].y;
			if (effectResultDict.modifyUids[i] != -1 &&
				allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType == "passive" &&
				effectResultDict.modifyType == allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType)
			{
				FEffectInfo passiveEffectInfo;
				passiveEffectInfo.launchType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType;
				passiveEffectInfo.coolDown = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.coolDown;
				passiveEffectInfo.availableTimes = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.availableTimes;
				passiveEffectInfo.launchGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchGeoType;
				passiveEffectInfo.targetGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetGeoType;
				passiveEffectInfo.aoeType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.aoeType;
				passiveEffectInfo.targetCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetCamp;
				passiveEffectInfo.effectType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectType;
				passiveEffectInfo.effectAffix = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffix;
				passiveEffectInfo.effectAffixCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffixCamp;
				passiveEffectInfo.prereqTagCondition = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTagCondition;
				passiveEffectInfo.prereqTag = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTag;
				passiveEffectInfo.prereqCampType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqCampType;
				passiveEffectInfo.prereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqType;
				passiveEffectInfo.prereqValue = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqValue;
				passiveEffectInfo.passivePrereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType;
				passiveEffectInfo.values = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.values;

				FEffectResultDict secondaryEffectResult = UCoreGameBlueprintFunctionLibrary::LaunchPassiveSkillDict(allInstanceCardInfo[effectResultDict.modifyUids[i]].camp,
					allInstanceCardInfo,
					boardRows,
					effectResultDict,
					passiveEffectInfo,
					effectResultDict.modifyType,
					effectResultDict.modifyGrids[i].x,
					effectResultDict.modifyGrids[i].y,
					triggerX,
					triggerY,
					true,
					sectionZeroScores,
					sectionOneScores);

				for (int32 j = 0; j < secondaryEffectResult.modifyValues.Num(); j++)
				{
					int32 possiblePassiveTargetGridPosInChannel = secondaryEffectResult.modifyGrids[j].y * UGlobalConstFunctionLibrary::maxCol + secondaryEffectResult.modifyGrids[j].x;
					int32 possiblePassiveTargetUid = boardRows[secondaryEffectResult.modifyGrids[j].y].colCardInfos[secondaryEffectResult.modifyGrids[j].x];

					if (secondaryEffectResult.modifyType == "hurt")
					{
						if (possiblePassiveTargetUid != -1)
						{
							if (allInstanceCardInfo[possiblePassiveTargetUid].camp != curPlayingSectionNb)
							{
								boardCoding[channelLen * 51 + possiblePassiveTargetGridPosInChannel] += secondaryEffectResult.modifyValues[j];
							}
							else
							{
								boardCoding[channelLen * 52 + possiblePassiveTargetGridPosInChannel] += -secondaryEffectResult.modifyValues[j];
							}
						}
					}
					else if (secondaryEffectResult.modifyType == "heal")
					{
						if (possiblePassiveTargetUid != -1)
						{
							if (allInstanceCardInfo[possiblePassiveTargetUid].camp == curPlayingSectionNb)
							{
								boardCoding[channelLen * 53 + possiblePassiveTargetGridPosInChannel] += secondaryEffectResult.modifyValues[j];
							}
							else
							{
								boardCoding[channelLen * 54 + possiblePassiveTargetGridPosInChannel] += -secondaryEffectResult.modifyValues[j];
							}
						}
					}
					else if (secondaryEffectResult.modifyType == "wound")
					{
						if (possiblePassiveTargetUid != -1)
						{
							if (allInstanceCardInfo[possiblePassiveTargetUid].camp != curPlayingSectionNb)
							{
								boardCoding[channelLen * 55 + possiblePassiveTargetGridPosInChannel] += secondaryEffectResult.modifyValues[j];
							}
							else
							{
								boardCoding[channelLen * 56 + possiblePassiveTargetGridPosInChannel] += -secondaryEffectResult.modifyValues[j];
							}
						}
					}
				}


				if (secondaryEffectResult.modifyUids.Num() > 0)
				{
					TriggerPotentialPassiveEffectCoding(
						boardCoding, 
						channelLen, 
						effectResultDict.modifyGrids[i].x,
						effectResultDict.modifyGrids[i].y,
						secondaryEffectResult, 
						alreadyTriggerPassiveUids);
				}
			}
		}
	}

	void TriggerPassiveEffect(
		int32 triggerX,
		int32 triggerY,
		FEffectResultDict effectResultDict,
		FRenderActionNode& renderParentNode)
	{
		// Traverse all cards that are modified, try to trigger their passive effects
		for (int32 i = 0; i < effectResultDict.modifyUids.Num(); i++)
		{
			if (effectResultDict.modifyUids[i] == -1)
			{
				continue;
			}
			if (allInstanceCardInfo[effectResultDict.modifyUids[i]].passiveEffectTriggerThisRound)
			{
				continue;
			}
			allInstanceCardInfo[effectResultDict.modifyUids[i]].passiveEffectTriggerThisRound = true;

			int32 modifyGridX = effectResultDict.modifyGrids[i].x;
			int32 modifyGridY = effectResultDict.modifyGrids[i].y;
			if (effectResultDict.modifyUids[i] != -1 &&
				allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType == "passive" &&
				effectResultDict.modifyType == allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType)
			{
				FEffectInfo passiveEffectInfo;
				passiveEffectInfo.launchType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType;
				passiveEffectInfo.coolDown = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.coolDown;
				passiveEffectInfo.availableTimes = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.availableTimes;
				passiveEffectInfo.launchGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchGeoType;
				passiveEffectInfo.targetGeoType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetGeoType;
				passiveEffectInfo.aoeType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.aoeType;
				passiveEffectInfo.targetCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.targetCamp;
				passiveEffectInfo.effectType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectType;
				passiveEffectInfo.effectAffix = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffix;
				passiveEffectInfo.effectAffixCamp = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.effectAffixCamp;
				passiveEffectInfo.prereqTagCondition = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTagCondition;
				passiveEffectInfo.prereqTag = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqTag;
				passiveEffectInfo.prereqCampType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqCampType;
				passiveEffectInfo.prereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqType;
				passiveEffectInfo.prereqValue = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.prereqValue;
				passiveEffectInfo.passivePrereqType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType;
				passiveEffectInfo.values = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.values;
				passiveEffectInfo.renderEffectType = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.renderEffectType;
				passiveEffectInfo.renderEffectTime = allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.renderEffectTime;

				FEffectResultDict secondaryEffectResult = UCoreGameBlueprintFunctionLibrary::LaunchPassiveSkillDict(allInstanceCardInfo[effectResultDict.modifyUids[i]].camp,
					allInstanceCardInfo,
					boardRows,
					effectResultDict,
					passiveEffectInfo,
					effectResultDict.modifyType,
					effectResultDict.modifyGrids[i].x,
					effectResultDict.modifyGrids[i].y,
					triggerX,
					triggerY,
					false,
					sectionZeroScores,
					sectionOneScores);


				if (secondaryEffectResult.modifyUids.Num() > 0)
				{
					int32 triggerCardId = boardRows[secondaryEffectResult.triggerGridY].colCardInfos[secondaryEffectResult.triggerGridX];
					FRenderActionNode renderActionNode;
					renderActionNode.actionType = ActionType::LaunchSkill;
					renderActionNode.renderRound = renderParentNode.renderRound + 1;
					renderActionNode.renderTime = secondaryEffectResult.renderTime;
					renderActionNode.renderEffectType = secondaryEffectResult.renderEffectType;
					renderActionNode.triggerCardId = triggerCardId;
					renderActionNode.triggerGridX = secondaryEffectResult.triggerGridX;
					renderActionNode.triggerGridY = secondaryEffectResult.triggerGridY;
					for (int32 j = 0; j < secondaryEffectResult.modifyGrids.Num(); j++)
					{
						renderActionNode.targetGridXs.Add(secondaryEffectResult.modifyGrids[j].x);
						renderActionNode.targetGridYs.Add(secondaryEffectResult.modifyGrids[j].y);
					}
					renderActionNode.modifyUids = secondaryEffectResult.modifyUids;
					renderActionNode.modifyValues = secondaryEffectResult.modifyValues;

					TriggerPassiveEffect(effectResultDict.modifyGrids[i].x,
						effectResultDict.modifyGrids[i].y, secondaryEffectResult, renderActionNode);


					renderParentNode.children.Add(renderActionNode);


					for (int32 j = 0; j < secondaryEffectResult.modifyGrids.Num(); j++)
					{
						if (allInstanceCardInfo[secondaryEffectResult.modifyUids[j]].curHp <= 0.0)
						{
							// move this card to grave
							MoveCard2Grave(allInstanceCardInfo[secondaryEffectResult.modifyUids[j]].camp,
								secondaryEffectResult.modifyGrids[j].x,
								secondaryEffectResult.modifyGrids[j].y,
								secondaryEffectResult.modifyUids[j]);
						}
					}
				}
			}
		}
	}

	void TriggerMove(
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		FRenderActionNode& renderParentNode)
	{
		int32 triggerCardId = boardRows[launchY].colCardInfos[launchX];
		renderParentNode.actionType = ActionType::Move;
		renderParentNode.triggerCardId = triggerCardId;
		renderParentNode.triggerGridX = launchX;
		renderParentNode.triggerGridY = launchY;
		renderParentNode.targetGridXs.Add(targetX);
		renderParentNode.targetGridYs.Add(targetY);
		renderParentNode.renderTime = defaultRenderMoveCardInterval;

		int32 moveCardUid = boardRows[launchY].colCardInfos[launchX];
		boardRows[targetY].colCardInfos[targetX] = moveCardUid;
		boardRows[launchY].colCardInfos[launchX] = -1;
		allInstanceCardInfo[moveCardUid].curCol = targetX;
		allInstanceCardInfo[moveCardUid].curRow = targetY;
	}

	void MoveCard2Grave(uint8 section, int32 deadGridX, int32 deadGridY, int32 deadUid)
	{
		if (boardRows[deadGridY].colCardInfos[deadGridX] == -1)
		{
			// which means this card has already been moved to grave
			return;
		}

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

	void GetMoveTypeCoding(FString moveType, int32* coding)
	{
		if (moveType == "line") coding[0] = 1;
		else if (moveType == "diagonal") coding[1] = 1;
		else if (moveType == "seperate") coding[2] = 1;
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
		else if (launchGeoType == "selfFrontRow")		coding[7] = 1;
		else if (launchGeoType == "oppoSection")	coding[8] = 1;
		else if (launchGeoType == "oppoFrontRow")		coding[9] = 1;
	}
	void GetSkillTargetGeoCoding(FString targetGeoType, int32* coding)
	{
		if (targetGeoType == "line")							coding[0] = 1;
		else if (targetGeoType == "seperate")			coding[1] = 1;
		else if (targetGeoType == "diagonal")				coding[2] = 1;
		else if (targetGeoType == "diagonalSeperated") coding[3] = 1;
		else if (targetGeoType == "arbitrary")			coding[4] = 1;
		else if (targetGeoType == "forward") coding[5] = 1;
		else if (targetGeoType == "backward") coding[6] = 1;
		else if (targetGeoType == "right") coding[7] = 1;
		else if (targetGeoType == "left") coding[8] = 1;
		else if (targetGeoType == "self") coding[9] = 1;
		else if (targetGeoType == "reflect") coding[10] = 1;
		else if (targetGeoType == "next") coding[11] = 1;
	}

	void GetSkillAoeCoding(FString aoeType, int32* coding)
	{
		if (aoeType == "point")					coding[0] = 1;
		else if (aoeType == "sweep")			coding[1] = 1;
		else if (aoeType == "H3")						coding[2] = 1;
		else if (aoeType == "V3")						coding[3] = 1;
	}

	void GetSkillEffectCoding(FString effectType, int32* coding)
	{
		if (effectType == "hurt")									coding[0] = 1;
		else if (effectType == "heal")							coding[1] = 1;
		else if (effectType == "increaseDefence")					coding[2] = 1;
		else if (effectType == "giveArmor")					coding[3] = 1;
		else if (effectType == "switchCamp")				coding[4] = 1;
		else if (effectType == "capture")							coding[5] = 1;
		else if (effectType == "wound")						coding[6] = 1;
		else if (effectType == "revenge")							coding[7] = 1;
		else if (effectType == "revengeWound")					coding[8] = 1;
		else if (effectType == "repayHeal")					coding[9] = 1;
		else if (effectType == "devour")					coding[10] = 1;
		else if (effectType == "explode")							coding[11] = 1;
		else if (effectType == "sublime") coding[12] = 1;
		else if (effectType == "spawn") coding[13] = 1;
		else if (effectType == "clone") coding[14] = 1;
		else if (effectType == "silence") coding[15] = 1;
		else if (effectType == "summonFirstDeath") coding[16] = 1;
		else if (effectType == "exchangeFirstDeath") coding[17] = 1;
		else if (effectType == "duel") coding[18] = 1;
		else if (effectType == "recover") coding[19] = 1;

		else if (effectType == "pull") coding[20] = 1;
		else if (effectType == "switchOppoPos") coding[21] = 1;
	}

	void GetAffixTypeCoding(FString affixType, int32* coding)
	{
		if (affixType == "sameLaunchRowNb") coding[0] = 1;
		else if (affixType == "sameRowNb") coding[1] = 1;
		else if (affixType == "sameColNb") coding[2] = 1;
		else if (affixType == "useSelfDefence_2") coding[3] = 1;
		else if (affixType == "graveCardNb") coding[4] = 1;
		else if (affixType == "useTargetArmor") coding[5] = 1;

	}

	void GetPrereqTypeCoding(FString prereqType, int32* coding)
	{
		if (prereqType == "selfSameRowDefenceMore") coding[0] = 1;
		else if (prereqType == "sameRowDefenceMore") coding[1] = 1;
		else if (prereqType == "selfDefenceMore") coding[2] = 1;
		else if (prereqType == "maxDefenceIsSelf") coding[3] = 1;
		else if (prereqType == "defenceDiffMore") coding[4] = 1;
		else if (prereqType == "threeDefenceMore") coding[5] = 1;
		else if (prereqType == "sameRowSelfDefenceMoreOppo") coding[6] = 1;
		else if (prereqType == "sameColSelfDefenceMoreOppo") coding[7] = 1;

		else if (prereqType == "hasArmor") coding[8] = 1;

		else if (prereqType == "handCardMore") coding[9] = 1;
		else if (prereqType == "handCardLess") coding[10] = 1;
		else if (prereqType == "selfGraveMore") coding[11] = 1;

		else if (prereqType == "sameRowMore") coding[12] = 1;
		else if (prereqType == "sameColMore") coding[13] = 1;
		else if (prereqType == "sameColSelfCardMoreThanOppo") coding[14] = 1;
		else if (prereqType == "selfHealCardMore") coding[15] = 1;
		
		else if (prereqType == "sameHp") coding[16] = 1;
		else if (prereqType == "hpDiffMore") coding[17] = 1;
		else if (prereqType == "maxHpIsSelf") coding[18] = 1;
		else if (prereqType == "targetHpLess") coding[19] = 1;
		else if (prereqType == "selfHurt") coding[20] = 1;
		else if (prereqType == "hurtCardMore") coding[21] = 1;
		else if (prereqType == "nextToHurt") coding[22] = 1;
	}



	void GetPotentialEffectCoding(int32* boardCoding, int32 channelLen, uint8 curSectionNb)
	{
		// There are 3 cases included:
		// 1. It contains affix of this card
		// 2. It contains pre reqs of this card
		// 3. It contains tag reqs of this card
		// or it contains combination of 2 or 3 of the previous
		// so we should coding all 3 channels instead of the mask combination of these 3
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow; 
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow; i++)
		{
			for (int32 j = 0; j < boardRows[i].colCardInfos.Num(); j++)
			{
				if (boardRows[i].colCardInfos[j] == -1)
				{
					continue;
				}

				FInstanceCardInfo launchCardInfo = allInstanceCardInfo[boardRows[i].colCardInfos[j]];

				if (launchCardInfo.curExtraTags.Contains("silent"))
				{
					continue;
				}

				FEffectInfo effectInfo;
				effectInfo.aoeType = launchCardInfo.originCardInfo.aoeType;
				effectInfo.availableTimes = launchCardInfo.originCardInfo.availableTimes;
				effectInfo.coolDown = launchCardInfo.originCardInfo.coolDown;
				effectInfo.effectAffix = launchCardInfo.originCardInfo.effectAffix;
				effectInfo.effectAffixCamp = launchCardInfo.originCardInfo.effectAffixCamp;
				effectInfo.effectType = launchCardInfo.originCardInfo.effectType;
				effectInfo.launchGeoType = launchCardInfo.originCardInfo.launchGeoType;
				effectInfo.launchType = launchCardInfo.originCardInfo.launchType;
				effectInfo.passivePrereqType = launchCardInfo.originCardInfo.passivePrereqType;
				effectInfo.prereqCampType = launchCardInfo.originCardInfo.prereqCampType;
				effectInfo.prereqTag = launchCardInfo.originCardInfo.prereqTag;
				effectInfo.prereqTagCondition = launchCardInfo.originCardInfo.prereqTagCondition;
				effectInfo.prereqType = launchCardInfo.originCardInfo.prereqType;
				effectInfo.prereqValue = launchCardInfo.originCardInfo.prereqValue;
				effectInfo.targetCamp = launchCardInfo.originCardInfo.targetCamp;
				effectInfo.targetGeoType = launchCardInfo.originCardInfo.targetGeoType;
				effectInfo.values = launchCardInfo.originCardInfo.values;
				effectInfo.renderEffectType = launchCardInfo.originCardInfo.renderEffectType;
				effectInfo.renderEffectTime = launchCardInfo.originCardInfo.renderEffectTime;

				bool prereqPass = true;
				if (!UCheckPrereqFunctionLibrary::CheckPrereqRule(
					allInstanceCardInfo,
					boardRows,
					effectInfo.prereqType,
					j,
					i,
					launchCardInfo.camp,
					effectInfo.prereqCampType,
					effectInfo.prereqValue))
				{
					prereqPass = false;
				}

				if (!prereqPass)
				{
					continue;
				}

				if (!UCheckPrereqTagFunctionLibrary::CheckLaunchPrereqTagRule(
					allInstanceCardInfo,
					boardRows,
					effectInfo,
					j,
					i))
				{
					prereqPass = false;
				}

				if (!prereqPass)
				{
					continue;
				}

				// It could be skill auto triggered, round end auto triggered, manual triggered
				TArray<FGridXY> possibleGrids;
				possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
					effectInfo.targetGeoType,
					allInstanceCardInfo,
					boardRows,
					effectInfo,
					j,
					i,
					launchCardInfo.originCardInfo.attackDistanceType);

				
				int32 virtualSectionZeroScores = 0;
				int32 virtualSectionOneScores = 0;
				for (int32 k = 0; k < possibleGrids.Num(); k++)
				{
					FEffectResultDict effectResultDict;
					if (effectInfo.launchType == "manual")
					{
						effectResultDict = UCoreGameBlueprintFunctionLibrary::LaunchSkillDict(allInstanceCardInfo,
							boardRows,
							effectInfo,
							j,
							i,
							possibleGrids[k].x,
							possibleGrids[k].y,
							true,
							virtualSectionZeroScores,
							virtualSectionOneScores);
					}
					else if (effectInfo.launchType == "autoRoundEnd")
					{
						effectResultDict = UCoreGameBlueprintFunctionLibrary::LaunchRoundEndSkillDict(launchCardInfo.camp,
							allInstanceCardInfo,
							boardRows,
							effectInfo,
							j,
							i,
							true,
							virtualSectionZeroScores,
							virtualSectionOneScores);
					}

					int32 possibleTargetGridPosInChannel = possibleGrids[k].y * UGlobalConstFunctionLibrary::maxCol + possibleGrids[k].x;
					if (effectInfo.effectType == "hurt")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp != curSectionNb)
							{
								// This channel records current player hurt oppo units
								boardCoding[channelLen * 33 + possibleTargetGridPosInChannel] += effectResultDict.modifyValues[n];
							}
							else
							{
								// This channel records current player being hurt 
								boardCoding[channelLen * 34 + possibleTargetGridPosInChannel] += -effectResultDict.modifyValues[n];
							}
						}
					}
					else if (effectInfo.effectType == "heal")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp == curSectionNb)
							{
								boardCoding[channelLen * 35 + possibleTargetGridPosInChannel] += effectResultDict.modifyValues[n];
							}
							else
							{
								boardCoding[channelLen * 36 + possibleTargetGridPosInChannel] += -effectResultDict.modifyValues[n];
							}
						}
					}
					else if (effectInfo.effectType == "wound")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp != curSectionNb)
							{
								boardCoding[channelLen * 37 + possibleTargetGridPosInChannel] += effectResultDict.modifyValues[n];
							}
							else
							{
								boardCoding[channelLen * 38 + possibleTargetGridPosInChannel] += -effectResultDict.modifyValues[n];
							}
						}
					}
					else if (effectInfo.effectType == "increaseDefence")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp == curSectionNb)
							{
								boardCoding[channelLen * 39 + possibleTargetGridPosInChannel] += effectResultDict.modifyValues[n];
							}
							else
							{
								boardCoding[channelLen * 40 + possibleTargetGridPosInChannel] += -effectResultDict.modifyValues[n];
							}
						}
					}
					else if (effectInfo.effectType == "giveArmor")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp == curSectionNb)
							{
								boardCoding[channelLen * 41 + possibleTargetGridPosInChannel] += 1;
							}
							else
							{
								boardCoding[channelLen * 42 + possibleTargetGridPosInChannel] += -1;
							}
						}
					}
					else if (effectInfo.effectType == "devour")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp == curSectionNb)
							{
								// which means this unit is being potentialy devoured by self
								boardCoding[channelLen * 43 + possibleTargetGridPosInChannel] += 1;
							}
							else
							{
								boardCoding[channelLen * 44 + possibleTargetGridPosInChannel] += -1;
							}
						}
					}
					else if (effectInfo.effectType == "spawn")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (launchCardInfo.camp == curSectionNb)
							{
								// which means this grid may potentialy be occupied by current player spawned unit
								boardCoding[channelLen * 45 + possibleTargetGridPosInChannel] += 1;
							}
							else
							{
								boardCoding[channelLen * 46 + possibleTargetGridPosInChannel] += -1;
							}
						}
					}
					else if (effectInfo.effectType == "switchCamp")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp != curSectionNb)
							{
								boardCoding[channelLen * 47 + possibleTargetGridPosInChannel] += 1;
							}
							else
							{
								boardCoding[channelLen * 48 + possibleTargetGridPosInChannel] += -1;
							}
						}
					}
					else if (effectInfo.effectType == "capture")
					{
						for (int32 n = 0; n < effectResultDict.modifyGrids.Num(); n++)
						{
							if (allInstanceCardInfo[effectResultDict.modifyUids[n]].camp != curSectionNb)
							{
								boardCoding[channelLen * 49 + possibleTargetGridPosInChannel] += 1;
							}
							else
							{
								boardCoding[channelLen * 50 + possibleTargetGridPosInChannel] += -1;
							}
						}
					}

					TArray<int32> alreadyTriggerPassiveUids;
					TriggerPotentialPassiveEffectCoding(
						boardCoding, 
						channelLen, 
						j, 
						i, 
						effectResultDict, 
						alreadyTriggerPassiveUids);
				}
			}
		}
	}
	void GetSkillAffixCoding(FString affixType, int32* coding)
	{
		if (affixType == "sameRowNb") coding[0] = 1;
		else if (affixType == "sameColNb") coding[1] = 1;
		else if (affixType == "useSelfDefence_2") coding[2] = 1;
		else if (affixType == "useTargetDefence_2") coding[3] = 1; // we should check the cards being hurt in this channel
		else if (affixType == "useTargetArmor") coding[4] = 1; // we should check the cards being hurt in this channel
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
		if (skillPrereq == "selfSameRowDefenceMore") coding[0] = 1;
		else if (skillPrereq == "selfDefenceMore") coding[1] = 1;
		else if (skillPrereq == "handCardMore") coding[2] = 1;
		else if (skillPrereq == "handCardLess") coding[2] = 1;
		else if (skillPrereq == "sameRowMore") coding[3] = 1;
		else if (skillPrereq == "sameColMore") coding[4] = 1;
		else if (skillPrereq == "selfGraveMore") coding[5] = 1;
		else if (skillPrereq == "targetHpLess") coding[6] = 1;
		else if (skillPrereq == "sameColSelfCardMoreThanOppo") coding[7] = 1;
		else if (skillPrereq == "selfHurt") coding[8] = 1;
		else if (skillPrereq == "hasArmor") coding[9] = 1;
		else if (skillPrereq == "maxHpIsSelf") coding[9] = 1;
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
	FString curMctsTreeNodeUid;

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

	int32 maxSelfPlayLoop = 10000;

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
	FMctsNode veryFirstNode;
	//UMctsTreeNode* veryFirstNode;

	UPROPERTY()
	TMap<FString, FMctsNode> allMctsNodes;


	UPROPERTY()
	FString treeRootUid;
	//UMctsTreeNode* treeRoot;

	//UPROPERTY()
	//TArray<FString> finishSelfPlayGameTreeRoots;

	UPROPERTY()
	TArray<FMctsNodeTree> finishSelfPlayGameTrees;

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


	/**
	* Testing part
	*/
};
