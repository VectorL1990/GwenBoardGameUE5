// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BattleBoard.h"
#include "CheckTargetGeoRuleLibrary.h"
#include "CheckPrereqTagFunctionLibrary.h"
#include "PassiveEffectFunctionLibrary.h"
#include "CheckPrereqFunctionLibrary.h"

// Sets default values
ABattleBoard::ABattleBoard()
{
				// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleBoard::BeginPlay()
{
				Super::BeginPlay();
				
}

// Called every frame
void ABattleBoard::Tick(float DeltaTime)
{
				Super::Tick(DeltaTime);

}

bool ABattleBoard::CheckGameEnd()
{
				return false;
}

void ABattleBoard::GetLatestSimulationBoard()
{
				simulationBoard.boardRows = realBoard.boardRows;
				simulationBoard.allInstanceCardInfo = realBoard.allInstanceCardInfo;
				simulationBoard.playSectionRows = realBoard.playSectionRows;
				simulationBoard.graveSectionRows = realBoard.graveSectionRows;
}

void ABattleBoard::GetLegalMoves(FBoardInfo& targetBoard, TArray<int32>& legalMoves)
{
				for (int32 row = 0; row < UGlobalConstFunctionLibrary::maxRow; row++)
				{
								for (int32 col = 0; col < UGlobalConstFunctionLibrary::maxCol; col++)
								{
												if (targetBoard.boardRows[row].colCardInfos[col] == -1)
												{
																// which means this grid is empty, we should consider about play action
																if ((curPlayerTurn == 0 && row < UGlobalConstFunctionLibrary::maxRow / 2) ||
																				(curPlayerTurn == 1 && row >= UGlobalConstFunctionLibrary::maxRow / 2))
																{
																				for (int32 playCardRow = 0; playCardRow < UGlobalConstFunctionLibrary::playCardSectionRow; playCardRow++)
																				{
																								for (int32 playCardCol = 0; playCardCol < UGlobalConstFunctionLibrary::maxCol; playCardCol++)
																								{
																												if (targetBoard.playSectionRows[playCardRow].colCardInfos[playCardCol] == -1)
																												{
																																continue;
																												}

																												int32 actionId = UCoreGameBlueprintFunctionLibrary::GetActionId(playCardCol, playCardRow, col, row, ActionType::PlayCard);
																												legalMoves.Add(actionId);
																								}
																				}
																}

																
												}
												else
												{
																// which means this grid is not empty, we could launch skill or move card


																// check possible skills
																FInstanceCardInfo cardInfo = targetBoard.allInstanceCardInfo[targetBoard.boardRows[row].colCardInfos[col]];
																if ((cardInfo.curAvailableTimes == -1 || cardInfo.curAvailableTimes > 0) &&
																				(cardInfo.curCoolDown == -1 || cardInfo.curCoolDown == 0))
																{
																				FEffectInfo effectInfo;
																				TArray<FGridXY> possibleGrids;
																				if (effectInfo.targetGeoType.Contains("&"))
																				{
																								TArray<FString> targetGeoTypes;
																								effectInfo.targetGeoType.ParseIntoArray(targetGeoTypes, TEXT("&"), true);
																								possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
																												targetGeoTypes[0],
																												targetBoard.allInstanceCardInfo,
																												targetBoard.boardRows,
																												effectInfo,
																												col,
																												row,
																												cardInfo.originCardInfo.attackDistance);

																								UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateGeoGrids(
																												targetGeoTypes[1],
																												targetBoard.allInstanceCardInfo,
																												targetBoard.boardRows,
																												possibleGrids);
																				}
																				else
																				{
																								possibleGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(
																												effectInfo.targetGeoType,
																												targetBoard.allInstanceCardInfo,
																												targetBoard.boardRows,
																												effectInfo,
																												col,
																												row,
																												cardInfo.originCardInfo.attackDistance);
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
																																row,
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
																												if (!UCheckPrereqTagFunctionLibrary::CheckPrereqTagRule(targetBoard.allInstanceCardInfo, targetBoard.boardRows, effectInfo, col, row))
																												{
																																possibleGrids.RemoveAt(checkGridNb);
																																continue;
																												}
																								}

																								checkGridNb += 1;
																				}

																				for (int32 i = 0; i < possibleGrids.Num(); i++)
																				{
																								int32 actionId = UCoreGameBlueprintFunctionLibrary::GetActionId(row, col, possibleGrids[i].x, possibleGrids[i].y, ActionType::LaunchSkill);
																								legalMoves.Add(actionId);
																				}
																}


																// check move action
																TArray<FGridXY> possibleMoveGrids = UCheckTargetGeoRuleLibrary::GetPossibleMoveGrids(
																				cardInfo.originCardInfo.moveType,
																				targetBoard.allInstanceCardInfo,
																				targetBoard.boardRows,
																				col,
																				row,
																				cardInfo.originCardInfo.moveDistance);
																
																for (int32 i = 0; i < possibleMoveGrids.Num(); i++)
																{
																				int32 actionId = UCoreGameBlueprintFunctionLibrary::GetActionId(
																								possibleMoveGrids[i].x,
																								possibleMoveGrids[i].y,
																								col,
																								row,
																								ActionType::Move);

																				legalMoves.Add(actionId);
																}
												}
								}
				}
}

void ABattleBoard::GetLegalActionProbsBoardValue(uint8* boardState, TMap<int32, float>& legalActionProbs, float& boardValue)
{
				TArray<float> actionProbs;
				float evaValue;
				UCoreGameBlueprintFunctionLibrary::QueryRemotePolicyValue(boardState, actionProbs, evaValue);
				boardValue = evaValue;

				TArray<int32> legalActionIds;
				GetLegalMoves(simulationBoard, legalActionIds);

				for (int32 i = 0; i < legalActionIds.Num(); i++)
				{
								legalActionProbs.Add(legalActionIds[i], actionProbs[legalActionIds[i]]);
				}
}

void ABattleBoard::TriggerAction(int32 actionId, bool simulateFlag)
{
				int32 launchX = 0;
				int32 launchY = 0;
				int32 targetX = 0;
				int32 targetY = 0;
				ActionType actionType = ActionType::EndRound;
				ActionDecoding(actionId, launchX, launchY, targetX, targetY, actionType);
				if (simulateFlag)
				{
								// which means it's only simulation, modify simulated board info instead of true board
								if (actionType == ActionType::LaunchSkill)
								{
												TriggerManualSkill(
																simulationBoard,
																launchX,
																launchY,
																targetX,
																targetY);
								}
								else if (actionType == ActionType::PlayCard)
								{
												TriggerPlayCard(
																simulationBoard,
																launchX,
																launchY,
																targetX,
																targetY);
								}
				}
				else
				{
								if (actionType == ActionType::PlayCard)
								{
												TriggerPlayCard(
																realBoard,
																launchX,
																launchY,
																targetX,
																targetY);
								}
								else if (actionType == ActionType::LaunchSkill)
								{
												TriggerManualSkill(
																realBoard,
																launchX,
																launchY,
																targetX,
																targetY);
								}
				}
}

void ABattleBoard::TriggerPlayCard(
				FBoardInfo& targetBoard,
				int32 playSectionX,
				int32 playSectionY,
				int32 targetX,
				int32 targetY)
{
				int32 playCardUid = targetBoard.playSectionRows[playSectionY].colCardInfos[playSectionX];
				targetBoard.boardRows[targetY].colCardInfos[targetX] = playCardUid;
				targetBoard.playSectionRows[playSectionY].colCardInfos[playSectionX] = -1;
				// Trigger play card skill
				if (targetBoard.allInstanceCardInfo[playCardUid].originCardInfo.launchType == "auto")
				{
								TriggerPlayCardSkill(targetBoard, targetX, targetY);
				}
}

void ABattleBoard::TriggerPlayCardSkill(
				FBoardInfo& targetBoard,
				int32 launchX,
				int32 launchY)
{
				int32 launchUid = targetBoard.boardRows[launchY].colCardInfos[launchX];
				FEffectInfo effectInfo;
				effectInfo.launchType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.launchType;
				effectInfo.coolDown = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.coolDown;
				effectInfo.availableTimes = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
				effectInfo.launchGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
				effectInfo.autoSkillTargetGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.autoSkillTargetGeoType;
				effectInfo.targetGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.targetGeoType;
				effectInfo.aoeType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.aoeType;
				effectInfo.targetCamp = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.targetCamp;
				effectInfo.effectType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectType;
				effectInfo.effectAffix = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectAffix;
				effectInfo.effectAffixCamp = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectAffixCamp;
				effectInfo.prereqTagCondition = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqTagCondition;
				effectInfo.prereqTag = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqTag;
				effectInfo.prereqCampType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqCampType;
				effectInfo.prereqType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqType;
				effectInfo.passivePrereqType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
				effectInfo.values = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.values;

				FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchPlayCardSkillDict(targetBoard.allInstanceCardInfo, targetBoard.boardRows, effectInfo, launchX, launchY);
				if (effectResultInfo.success)
				{
								TriggerPassiveEffect(targetBoard, effectResultInfo);
				}
}

void ABattleBoard::TriggerRoundEndSkill(
				FBoardInfo& targetBoard,
				int32 launchX,
				int32 launchY,
				int32 targetX,
				int32 targetY)
{

}

void ABattleBoard::TriggerManualSkill(
				FBoardInfo& targetBoard,
				int32 launchX, 
				int32 launchY, 
				int32 targetX, 
				int32 targetY)
{
				int32 launchUid = targetBoard.boardRows[launchY].colCardInfos[launchX];
				FEffectInfo effectInfo;
				effectInfo.launchType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.launchType;
				effectInfo.coolDown = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.coolDown;
				effectInfo.availableTimes = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.availableTimes;
				effectInfo.launchGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.launchGeoType;
				effectInfo.autoSkillTargetGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.autoSkillTargetGeoType;
				effectInfo.targetGeoType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.targetGeoType;
				effectInfo.aoeType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.aoeType;
				effectInfo.targetCamp = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.targetCamp;
				effectInfo.effectType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectType;
				effectInfo.effectAffix = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectAffix;
				effectInfo.effectAffixCamp = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.effectAffixCamp;
				effectInfo.prereqTagCondition = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqTagCondition;
				effectInfo.prereqTag = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqTag;
				effectInfo.prereqCampType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqCampType;
				effectInfo.prereqType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.prereqType;
				effectInfo.passivePrereqType = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.passivePrereqType;
				effectInfo.values = targetBoard.allInstanceCardInfo[launchUid].originCardInfo.values;

				FEffectResultDict effectResultInfo = UCoreGameBlueprintFunctionLibrary::LaunchSkillDict(targetBoard.allInstanceCardInfo, targetBoard.boardRows, effectInfo, launchX, launchY, targetX, targetY);
				if (effectResultInfo.success)
				{
								TriggerPassiveEffect(targetBoard, effectResultInfo);
				}
}

void ABattleBoard::TriggerPassiveEffect(FBoardInfo& targetBoard, FEffectResultDict effectResultDict)
{
				FEffectResultDict secondaryEffectResult;
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
												targetBoard.allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.launchType == "passive" &&
												effectResultDict.modifyType == targetBoard.allInstanceCardInfo[effectResultDict.modifyUids[i]].originCardInfo.passivePrereqType)
								{
												FEffectInfo secondaryEffectInfo;
												secondaryEffectResult = UPassiveEffectFunctionLibrary::GetPassiveEffect(
																targetBoard.allInstanceCardInfo,
																targetBoard.boardRows,
																secondaryEffectInfo,
																modifyGridX,
																modifyGridY,
																effectResultDict.triggerGridX,
																effectResultDict.triggerGridY,
																curRoundPassiveEffectTriggeredUids
												);

												curRoundPassiveEffectTriggeredUids.Add(effectResultDict.modifyUids[i]);

												int32 checkSecondaryEffectResultNb = 0;
												while (checkSecondaryEffectResultNb < secondaryEffectResult.modifyUids.Num())
												{
																if (curRoundPassiveEffectTriggeredUids.Contains(secondaryEffectResult.modifyUids[checkSecondaryEffectResultNb]))
																{
																				secondaryEffectResult.modifyUids.RemoveAt(checkSecondaryEffectResultNb);
																				secondaryEffectResult.modifyValues.RemoveAt(checkSecondaryEffectResultNb);
																}
																else
																{
																				checkSecondaryEffectResultNb += 1;
																}
												}
								}
				}


				if (secondaryEffectResult.modifyUids.Num() > 0)
				{
								TriggerPassiveEffect(targetBoard, secondaryEffectResult);
				}
}

uint8* ABattleBoard::StateCoding(FBoardInfo& targetBoard)
{
				for (TMap<int32, FBoardRow>::TConstIterator rowIter = targetBoard.boardRows.CreateConstIterator(); rowIter; ++rowIter)
				{
								for (TMap<int32, int32>::TConstIterator colIter = rowIter->Value.colCardInfos.CreateConstIterator(); colIter; ++colIter)
								{
												int32 row = rowIter->Key;
												int32 col = colIter->Key;
												int32 uid = colIter->Value;
												
												uint8 coding[200] = { 0 };
												// 6
												uint8* skillLaunchTypeCoding = GetSkillLaunchTypeCoding(targetBoard.allInstanceCardInfo[uid].originCardInfo.launchType);
												// 10
												uint8* skillLaunchGeoCoding = GetSkillLaunchGeoCoding(targetBoard.allInstanceCardInfo[uid].originCardInfo.launchGeoType);
												// 6
												uint8* skillTargetGeoCoding = GetSkillTargetGeoCoding(targetBoard.allInstanceCardInfo[uid].originCardInfo.targetGeoType);
												// 9
												//uint8* skillTargetLocateGeoCoding = GetSkillTargetLocateGeoCoding(allInstanceCardInfo[uid].originCardInfo.target);
												// 6
												uint8* skillAoeCoding = GetSkillAoeCoding(targetBoard.allInstanceCardInfo[uid].originCardInfo.aoeType);
												// 3

												
								}
				}

				return skillLaunchTypeCoding;
}


void ABattleBoard::ActionDecoding(int32 actionId, int32& launchX, int32& launchY, int32& targetX, int32& targetY, ActionType& actionType)
{
				int32 totalGridNb = UGlobalConstFunctionLibrary::maxRow * UGlobalConstFunctionLibrary::maxCol;
				int32 totalPlayCardActions = UGlobalConstFunctionLibrary::playCardSectionRow * UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::maxRow * UGlobalConstFunctionLibrary::maxCol;
				int32 totalLaunchSkillActions = UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::maxRow;
				int32 totalMoveActions = UGlobalConstFunctionLibrary::maxCol * UGlobalConstFunctionLibrary::maxRow;
				if (actionId < totalPlayCardActions)
				{
								// which means it's play card action
								
								int32 playCardSectionGridNb = FMath::FloorToInt((float)actionId / (float)totalGridNb);
								int32 targetCardGridNb = actionId % totalGridNb;

								int32 playCardSectionRow = FMath::FloorToInt((float)playCardSectionGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 playCardSectionCol = playCardSectionGridNb % UGlobalConstFunctionLibrary::maxCol;

								int32 targetSectionRow = FMath::FloorToInt((float)targetCardGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 targetSectionCol = targetCardGridNb % UGlobalConstFunctionLibrary::maxCol;

								launchX = playCardSectionCol;
								launchY = playCardSectionRow;
								targetX = targetSectionCol;
								targetY = targetSectionRow;
								actionType = ActionType::PlayCard;
				}
				else if (actionId < totalPlayCardActions + totalLaunchSkillActions)
				{
								// which means it's launch skill action
								int32 launchSkillActionId = actionId - totalPlayCardActions;

								int32 launchGridNb = FMath::FloorToInt((float)launchSkillActionId / (float)totalGridNb);
								int32 targetGridNb = launchSkillActionId % totalGridNb;

								int32 launchRow = FMath::FloorToInt((float)launchGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 launchCol = launchGridNb % UGlobalConstFunctionLibrary::maxCol;

								int32 targetRow = FMath::FloorToInt((float)targetGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 targetCol = targetGridNb % UGlobalConstFunctionLibrary::maxCol;

								launchX = launchCol;
								launchY = launchRow;
								targetX = targetCol;
								targetY = targetRow;
								actionType = ActionType::LaunchSkill;
				}
				else if (actionId < totalPlayCardActions + totalLaunchSkillActions + totalMoveActions)
				{
								// which means it's move action
								int32 moveActionId = actionId - totalPlayCardActions - totalLaunchSkillActions;

								int32 launchGridNb = FMath::FloorToInt((float)moveActionId / (float)totalGridNb);
								int32 targetGridNb = moveActionId % totalGridNb;

								int32 launchRow = FMath::FloorToInt((float)launchGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 launchCol = launchGridNb % UGlobalConstFunctionLibrary::maxCol;

								int32 targetRow = FMath::FloorToInt((float)targetGridNb / (float)UGlobalConstFunctionLibrary::maxCol);
								int32 targetCol = targetGridNb % UGlobalConstFunctionLibrary::maxCol;

								launchX = launchCol;
								launchY = launchRow;
								targetX = targetCol;
								targetY = targetRow;
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


uint8* ABattleBoard::GetSkillLaunchTypeCoding(FString launchType)
{
				uint8* coding = skillLaunchTypeCoding;
				if (launchType == "auto")																			coding[0] = 1;
				else if (launchType == "autoRoundEnd")						coding[1] = 1;
				else if (launchType == "manual")												coding[2] = 1;
				else if (launchType == "manualImmediate")			coding[3] = 1;
				else if (launchType == "passive")											coding[4] = 1;
				else if (launchType == "playCard")										coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetAutoSkillGeoTargetCoding(FString geoTargetType)
{
				uint8* coding = autoSkillGeoTargetTypeCoding;
				if (geoTargetType == "self")																																coding[0] = 1;
				else if (geoTargetType == "left")																											coding[1] = 1;
				else if (geoTargetType == "right")																										coding[2] = 1;
				else if (geoTargetType == "up")																													coding[3] = 1;
				else if (geoTargetType == "down")																											coding[4] = 1;
				else if (geoTargetType == "leftLine")																							coding[5] = 1;
				else if (geoTargetType == "rightLine")																						coding[6] = 1;
				else if (geoTargetType == "upLine")																									coding[7] = 1;
				else if (geoTargetType == "downLine")																							coding[8] = 1;
				else if (geoTargetType == "upLeft")																									coding[9] = 1;
				else if (geoTargetType == "upRight")																								coding[10] = 1;
				else if (geoTargetType == "downLeft")																							coding[11] = 1;
				else if (geoTargetType == "downRight")																						coding[12] = 1;
				else if (geoTargetType == "upLeftDiagonal")																	coding[13] = 1;
				else if (geoTargetType == "upRightDiagonal")																coding[14] = 1;
				else if (geoTargetType == "downLeftDiagonal")															coding[15] = 1;
				else if (geoTargetType == "downRightDiagonal")														coding[16] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillLaunchGeoCoding(FString launchGeoType)
{
				uint8* coding = skillLaunchGeoCoding;
				if (launchGeoType == "point")															coding[0] = 1;
				else if (launchGeoType == "three")										coding[1] = 1;
				else if (launchGeoType == "horn")											coding[2] = 1;
				else if (launchGeoType == "triangle")							coding[3] = 1;
				else if (launchGeoType == "obliqueCross")			coding[4] = 1;
				else if (launchGeoType == "normalCross")				coding[5] = 1;
				else if (launchGeoType == "selfSection")				coding[6] = 1;
				else if (launchGeoType == "selfFrontLine")		coding[7] = 1;
				else if (launchGeoType == "oppoSection")				coding[8] = 1;
				else if (launchGeoType == "oppoFrontLine")		coding[9] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetGeoCoding(FString targetGeoType)
{
				uint8* coding = skillTargetGeoCoding;
				if (targetGeoType == "line")																			coding[0] = 1;
				else if (targetGeoType == "seperated")									coding[1] = 1;
				else if (targetGeoType == "diagonal")										coding[2] = 1;
				else if (targetGeoType == "diagonalSeperated") coding[3] = 1;
				else if (targetGeoType == "connect")											coding[4] = 1;
				else if (targetGeoType == "arbitrary")									coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetLocateGeoCoding(FString targetLocateGeoType)
{
				uint8* coding = skillTargetLocateGeoCoding;
				if (targetLocateGeoType == "three")																	coding[0] = 1;
				else if (targetLocateGeoType == "horn")													coding[1] = 1;
				else if (targetLocateGeoType == "triangle")									coding[2] = 1;
				else if (targetLocateGeoType == "obliqueCross")					coding[3] = 1;
				else if (targetLocateGeoType == "normalCross")						coding[4] = 1;
				else if (targetLocateGeoType == "selfSection")						coding[5] = 1;
				else if (targetLocateGeoType == "selfFrontLine")				coding[6] = 1;
				else if (targetLocateGeoType == "oppoSection")						coding[7] = 1;
				else if (targetLocateGeoType == "oppoFrontLine")				coding[8] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillAoeCoding(FString aoeType)
{
				uint8* coding = skillAoeCoding;
				if (aoeType == "point")																	coding[0] = 1;
				else if (aoeType == "sweep")												coding[1] = 1;
				else if (aoeType == "H3")															coding[2] = 1;
				else if (aoeType == "V3")															coding[3] = 1;
				else if (aoeType == "normalCross")						coding[4] = 1;
				else if (aoeType == "obliqueCross")					coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetCampCoding(FString targetCampType)
{
				uint8* coding = skillTargetCampCoding;
				if (targetCampType == "self")							coding[0] = 1;
				else if (targetCampType == "oppo")		coding[1] = 1;
				else if (targetCampType == "none")		coding[2] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillEffectCoding(FString effectType)
{
				uint8* coding = skillEffectCoding;
				if (effectType == "hurt")																											coding[0] = 1;
				else if (effectType == "heal")																						coding[1] = 1;
				else if (effectType == "increaseDefence")											coding[2] = 1;
				else if (effectType == "replaceDefence")												coding[3] = 1;
				else if (effectType == "increaseSelfDefence")							coding[4] = 1;
				else if (effectType == "giveTempArmor")													coding[5] = 1;
				else if (effectType == "giveArmor")																	coding[6] = 1;
				else if (effectType == "tempArmor")																	coding[7] = 1;
				else if (effectType == "defenceHurt")															coding[8] = 1;
				else if (effectType == "defenceHeal")															coding[9] = 1;
				else if (effectType == "defenceDetonate")											coding[10] = 1;
				else if (effectType == "useArmorHurt")														coding[11] = 1;
				else if (effectType == "useArmorHeal")														coding[12] = 1;
				else if (effectType == "armorDetonate")													coding[13] = 1;
				else if (effectType == "armorDetonateSilence")						coding[14] = 1;
				else if (effectType == "armorDetonateWound")								coding[15] = 1;
				else if (effectType == "armorDetonatePoison")							coding[16] = 1;
				else if (effectType == "armorDetonateSublime")						coding[17] = 1;
				else if (effectType == "switchCamp")																coding[18] = 1;
				else if (effectType == "capture")																			coding[19] = 1;
				else if (effectType == "hurtTransfer")														coding[20] = 1;
				else if (effectType == "hurtLink")																		coding[21] = 1;
				else if (effectType == "healTransfer")														coding[22] = 1;
				else if (effectType == "healLink")																		coding[23] = 1;
				else if (effectType == "sublimeTransfer")											coding[24] = 1;
				else if (effectType == "silenceTransfer")											coding[25] = 1;
				else if (effectType == "woundTransfer")													coding[26] = 1;
				else if (effectType == "wound")																					coding[27] = 1;
				else if (effectType == "convertSublimeToWound")					coding[28] = 1;
				else if (effectType == "transferWound")													coding[29] = 1;
				else if (effectType == "revenge")																			coding[30] = 1;
				else if (effectType == "revengeWound")														coding[31] = 1;
				else if (effectType == "repayHeal")																	coding[32] = 1;
				else if (effectType == "repaySublime")														coding[33] = 1;
				else if (effectType == "devour")																				coding[34] = 1;
				else if (effectType == "devourSublime")													coding[35] = 1;
				else if (effectType == "devourButWound")												coding[36] = 1;
				else if (effectType == "deadWishConvert")											coding[37] = 1;
				else if (effectType == "deadWishHurt")														coding[38] = 1;
				else if (effectType == "deadWishSpawnUp")											coding[39] = 1;
				else if (effectType == "deadWishSpawnDown")									coding[40] = 1;
				else if (effectType == "deadWishSpawnRight")								coding[41] = 1;
				else if (effectType == "deadWishSpawnLeft")									coding[42] = 1;
				else if (effectType == "deadWishDestroyUp")									coding[43] = 1;
				else if (effectType == "deadWishDestroyDown")							coding[44] = 1;
				else if (effectType == "deadWishDestroyRight")						coding[45] = 1;
				else if (effectType == "deadWishDestroyLeft")							coding[46] = 1;
				else if (effectType == "deadWishHeal")														coding[47] = 1;
				else if (effectType == "deadWishHealUp")												coding[48] = 1;
				else if (effectType == "deadWishHealDown")										coding[49] = 1;
				else if (effectType == "deadWishHealRight")									coding[50] = 1;
				else if (effectType == "deadWishHealLeft")										coding[51] = 1;
				else if (effectType == "deadWishSublime")											coding[52] = 1;
				else if (effectType == "deadWishTackle")												coding[53] = 1;
				else if (effectType == "deadWishWound")													coding[54] = 1;
				else if (effectType == "explode")																			coding[55] = 1;
				else if (effectType == "explodeToPoison")											coding[56] = 1;
				else if (effectType == "explodeToWound")												coding[57] = 1;
				else if (effectType == "exchange")																		coding[58] = 1;
				else if (effectType == "drag")																						coding[59] = 1;
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

				return coding;
}

uint8* ABattleBoard::GetSkillAffixCampCoding(FString affixCampType)
{
				uint8* coding = skillAffixCampCoding;
				if (affixCampType == "self") coding[0] = 1;
				else if (affixCampType == "oppo") coding[1] = 1;
				else if (affixCampType == "none") coding[2] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillAffixCoding(FString affixType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillTagConditionCoding(FString tagCondition)
{
				return 0;
}

uint8* ABattleBoard::GetCardTagCoding(FString tag)
{
				return 0;
}

uint8* ABattleBoard::GetPrereqTagCoding(FString prereqTag)
{
				return 0;
}

uint8* ABattleBoard::GetPrereqCampCoding(FString prereqCampType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillPrereqCoding(FString skillPrereq)
{
				return 0;
}

uint8* ABattleBoard::GetPassiveSkillPrereqTypeCoding(FString passiveSkillPrereq)
{
				return 0;
}

