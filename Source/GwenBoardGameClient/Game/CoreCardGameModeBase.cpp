// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameModeBase.h"
#include "Base/GwenBoardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CoreCardGamePC.h"
#include "Engine/KBEngine.h"
#include "Engine/Entity.h"
#include "CheckTargetGeoRuleLibrary.h"
#include "Scripts/BattleEvents.h"

void ACoreCardGameModeBase::BeginPlay()
{
				InitEvents();
				InitKBEMain();
				InitDone();
				GetAllPresetObjects();
				InitPreBattle();
				CheckEntitiesCreated();

				SpawnTestCards();
				CalculateCardSpread();
				for (int32 i = 0; i < testCards.Num(); i++)
				{
								testCards[i]->SetActorLocation(testCardLocations[i]);
								testCards[i]->SetActorRotation(testCardRots[i]);
				}
}

void ACoreCardGameModeBase::InitEvents()
{
				Super::InitEvents();
				KBENGINE_REGISTER_EVENT("onEnterWorld", onEnterWorld);
				KBENGINE_REGISTER_EVENT("onSyncBattleResult", onSyncBattleResult);
				KBENGINE_REGISTER_EVENT("onSyncChangeHandCardSuccess", onSyncChangeHandCardSuccess);
				KBENGINE_REGISTER_EVENT("onSyncExhaustCardReplacement", onSyncExhaustCardReplacement);
				KBENGINE_REGISTER_EVENT("onSyncHeartBeat", onSyncHeartBeat);
				KBENGINE_REGISTER_EVENT("onSyncLatestBattleState", onSyncLatestBattleState);
				KBENGINE_REGISTER_EVENT("onSyncPlayerBattleInfo", onSyncPlayerBattleInfo);
				KBENGINE_REGISTER_EVENT("onSyncReceiveEnterRoom", onSyncReceiveEnterRoom);
				KBENGINE_REGISTER_EVENT("onSyncReceiveFinishCardSelection", onSyncReceiveFinishCardSelection);
				KBENGINE_REGISTER_EVENT("onSyncResumeBattle", onSyncResumeBattle);
				KBENGINE_REGISTER_EVENT("onSyncRoomStartBattle", onSyncRoomStartBattle);
				KBENGINE_REGISTER_EVENT("onSyncSelectCardInterlude", onSyncSelectCardInterlude);
				KBENGINE_REGISTER_EVENT("onSyncSwitchController", onSyncSwitchController);
				KBENGINE_REGISTER_EVENT("onSyncTimeInterval", onSyncTimeInterval);
				KBENGINE_REGISTER_EVENT("onSyncUpdateSelectedCards", onSyncUpdateSelectedCards);
}

void ACoreCardGameModeBase::Tick(float deltaTime)
{
				MoveRearrangeCards();

				if (isSinglePlay)
				{
								
				}
				else
				{

				}
}



void ACoreCardGameModeBase::RandSelectCards()
{
				UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
				UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
				
				TArray<int32> cardNbList;
				for (int32 i = 0; i < gwenGI->selectCardList.Num(); i++)
				{
								cardNbList.Add(i);
				}

				TArray<int32> selectCardNbList;
				for (int32 i = 0; i < startingSelectCardNb; i++)
				{
								int32 randCardNb = FMath::RandRange(0, cardNbList.Num() - 1);
								selectCardNbList.Add(cardNbList[randCardNb]);
								cardNbList.RemoveAt(randCardNb);
				}

				for (int32 i = 0; i < selectCardNbList.Num(); i++)
				{

				}
}

void ACoreCardGameModeBase::FinishCardSelection()
{
				// Trigger by Select Card widget, next operation depends on single or multi players game mode
}

void ACoreCardGameModeBase::SimulateTrainAction(float dT)
{
				if (aiTrainPlayerActionCount >= aiTrainPlayerActionInterval)
				{
								// get train simulation action and launch
								int32 actionId = -1;
								mctsPlayer->mcts->GetAction(battleBoard, actionId);

								battleBoard->TriggerAction(actionId, true);

								aiTrainPlayerActionCount = 0.0;
				}
				else
				{
								aiTrainPlayerActionCount += dT;
				}


				
}

void ACoreCardGameModeBase::TrainPlayGameLoop(float dT)
{
				if (singleBattleState == SingleBattleState::Default)
				{
								return;
				}
				else if (singleBattleState == SingleBattleState::Battle)
				{
								if (curCountingTick >= battleStateTicksMap["MaxLaunchActionTimeInterval"])
								{
												singleBattleState = SingleBattleState::BattleInterlude;
												curCountingTick = 0.0;
								}
								else
								{
												// keep trying to trigger train action
												SimulateTrainAction(dT);
												curCountingTick += dT;
								}
				}
}

void ACoreCardGameModeBase::TriggerReadCardInfo_Implementation()
{

}

void ACoreCardGameModeBase::SetCardInfo(FString cardName,
				FString aliasCN,
				FString aliasEN,
				FString cardFunctionCN,
				FString cardFunctionEN,
				FString despCN,
				FString despEN,
				int32 hp,
				int32 defence,
				int32 agility,
				TArray<FString> cardTags,
				FString launchType,
				int32 coolDown,
				int32 availableTimes,
				FString launchGeoType,
				FString autoSkillTargetGeoType,
				FString targetGeoType,
				FString aoeType,
				FString targetCamp,
				FString effectType,
				FString effectAffix,
				FString effectAffixCamp,
				FString prereqTagCondition,
				FString prereqTag,
				FString prereqCampType,
				FString prereqType,
				FString passivePrereqType,
				TArray<int32> values,
				UTexture* texture,
				FString moveType,
				FString cardCategory)
{
				FCardInfo cardInfo;
				cardInfo.cardName = cardName;
				cardInfo.aliasCN = aliasCN;
				cardInfo.aliasEN = aliasEN;
				cardInfo.cardFunctionCN = cardFunctionCN;
				cardInfo.cardFunctionEN = cardFunctionEN;
				cardInfo.despCN = despCN;
				cardInfo.despEN = despEN;
				cardInfo.hp = hp;
				cardInfo.defence = defence;
				cardInfo.agility = agility;
				cardInfo.cardTags = cardTags;
				cardInfo.launchType = launchType;
				cardInfo.coolDown = coolDown;
				cardInfo.availableTimes = availableTimes;
				cardInfo.launchGeoType = launchGeoType;
				cardInfo.autoSkillTargetGeoType = autoSkillTargetGeoType;
				cardInfo.targetGeoType = targetGeoType;
				cardInfo.aoeType = aoeType;
				cardInfo.targetCamp = targetCamp;
				cardInfo.effectType = effectType;
				cardInfo.effectAffix = effectAffix;
				cardInfo.effectAffixCamp = effectAffixCamp;
				cardInfo.prereqTagCondition = prereqTagCondition;
				cardInfo.prereqTag = prereqTag;
				cardInfo.prereqCampType = prereqCampType;
				cardInfo.prereqType = prereqType;
				cardInfo.passivePrereqType = passivePrereqType;
				cardInfo.values = values;
				cardInfo.texture = texture;
				cardInfo.moveType = moveType;
				cardInfo.cardCategory = cardCategory;
				allCardInfos.Add(cardName, cardInfo);
}


void ACoreCardGameModeBase::SinglePlayerGameLoop(float dT)
{
				if (singleBattleState == SingleBattleState::Default)
				{
								return;
				}
				else if (singleBattleState == SingleBattleState::SelectCard)
				{
								if (curCountingTick >= battleStateTicksMap["MaxSelectCardInterval"])
								{
												// switch to select card interlude
												singleBattleState = SingleBattleState::SelectCardInterlude;
												curCountingTick = 0.0;
								}
								else
								{
												curCountingTick += dT;
								}
				}
				else if (singleBattleState == SingleBattleState::SelectCardInterlude)
				{
								if (curCountingTick >= battleStateTicksMap["MaxSelectCardInterludeInterval"])
								{
												singleBattleState = SingleBattleState::Battle;
												curCountingTick = 0.0;
								}
								else
								{
												curCountingTick += dT;
								}
				}
				else if (singleBattleState == SingleBattleState::Battle)
				{
								if (curCountingTick >= battleStateTicksMap["MaxLaunchActionTimeInterval"])
								{
												singleBattleState = SingleBattleState::BattleInterlude;
												curCountingTick = 0.0;
								}
								else
								{
												if (!isHumanTurn)
												{
																// Get action from neural network

												}
												curCountingTick += dT;
								}
				}
}

void ACoreCardGameModeBase::GetLegalLaunchSkillAction(TMap<int32, FBoardRow>& boardCardInfo, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY)
{
				/*int32 uid = boardCardInfo[launchY].colCardInfos[launchX];
				if ((effectInfo.launchType == "manual" || effectInfo.launchType == "manualImmediate") &&
								(effectInfo.coolDown == 0 || effectInfo.coolDown == -1) &&
								(effectInfo.availableTimes > 0 || effectInfo.availableTimes == -1))
				{
								if (effectInfo.targetGeoType.Contains("&"))
								{
												TArray<FString> splitStrs;
												effectInfo.targetGeoType.ParseIntoArray(splitStrs, TEXT("&"), true);
												TArray<FGridXY> possibleGeoGrids = UCheckTargetGeoRuleLibrary::GetPossibleTargetGeoGrids(splitStrs[0], allInstanceCardInfo, boardCardInfo, effectInfo, launchX, launchY);
												UCheckTargetGeoRuleLibrary::CheckPossibleTargetLocateGeoGrids(splitStrs[1], allInstanceCardInfo, boardCardInfo, possibleGeoGrids);
								}
								else
								{

								}
				}*/
}

void ACoreCardGameModeBase::ReqPlayCard(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
				if (simulationFlag)
				{
								int32 playCardUid = battleBoard->simulationBoard.playSectionRows[launchY].colCardInfos[launchX];
								FInstanceCardInfo playCardInfo = battleBoard->simulationBoard.allInstanceCardInfo[playCardUid];
								// launch auto skill attached to card
								if (playCardInfo.originCardInfo.launchType == "auto")
								{
												
								}
				}
				else
				{
								int32 playCardUid = battleBoard->realBoard.playSectionRows[launchY].colCardInfos[launchX];
								FInstanceCardInfo playCardInfo = battleBoard->realBoard.allInstanceCardInfo[playCardUid];
								// launch auto skill attached to card
								if (playCardInfo.originCardInfo.launchType == "auto")
								{

								}
				}

				
}

void ACoreCardGameModeBase::ReqLaunchCardSkill(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
				
}

void ACoreCardGameModeBase::SpawnTestCards()
{
				FVector spawnTestLoc = FVector(0.0, 0.0, 0.0);
				testMoveCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnTestLoc, FRotator::ZeroRotator);
				for (int32 i = 0; i < 10; i++)
				{
								FVector spawnLoc = FVector(0.0, 0.0, 0.0);
								ACard* card = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, FRotator::ZeroRotator);
								testCards.Add(card);
				}
}

void ACoreCardGameModeBase::RearrangeCardLocations(int32 hoverCardNb)
{
				if (hoverCardNb >= testCards.Num())
				{
								return;
				}

				// Move cards on the right side of hovered card
				for (int32 i = hoverCardNb; i < testCards.Num(); i++)
				{
								FVector cardOffset = FVector(hoverMoveRightCardsOffset, 0.0, 0.0);
								testCardTempLocations[i] = testCardLocations[i] + cardOffset;
				}

				FVector hoverCardOffset = FVector(0.0, -hoverCardUpOffset, 0.0);
				testCardTempLocations[hoverCardNb] = testCardLocations[hoverCardNb] + hoverCardOffset;
}

void ACoreCardGameModeBase::RecoverCardLocations()
{
				testCardTempLocations = testCardLocations;
}

void ACoreCardGameModeBase::CalculateCardSpread()
{
				TArray<float> handCardYawList;
				for (int32 i = 0; i < testCards.Num(); i++)
				{
								float cardSelfRot = 0.0;
								float spreadCardRot = 0.0;
								float spreadCardPositionY = 0.0;
								if (testCards.Num() % 2 == 0)
								{
												int32 halfCardNum = testCards.Num() / 2;
												if (i <= halfCardNum)
												{
																cardSelfRot = -((float)halfCardNum - (float)i - 0.5) * cardSelfRotInterval;
																spreadCardRot = -((float)halfCardNum - (float)i - 0.5) * spreadCardRotInterval;
																spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
												}
												else
												{
																cardSelfRot = ((float)i - (float)halfCardNum + 0.5) * cardSelfRotInterval;
																spreadCardRot = ((float)i - (float)halfCardNum + 0.5) * spreadCardRotInterval;
																spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
												}
								}
								else
								{
												int32 halfCardNum = testCards.Num() / 2;
												if (i <= halfCardNum)
												{
																cardSelfRot = -((float)halfCardNum - (float)i) * cardSelfRotInterval;
																spreadCardRot = -((float)halfCardNum - (float)i) * spreadCardRotInterval;
																spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
												}
												else
												{
																cardSelfRot = ((float)i - (float)halfCardNum) * cardSelfRotInterval;
																spreadCardRot = ((float)i - (float)halfCardNum) * spreadCardRotInterval;
																spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
												}
								}
								float spreadCardPositionX = spreadCardRotRadius * FMath::Sin(spreadCardRot * PI / 180.0);
								
								FVector cardLocation = FVector(spreadCardPositionX, spreadCardPositionY, spreadCardHeight + spreadCardStepHeight * i);
								cardLocation -= spreadCardOffset;
								FRotator cardRotation = FRotator(0.0, spreadCardRot, 0.0);
								
								testCardRots.Add(cardRotation);
								testCardLocations.Add(cardLocation);
				}
				testCardTempLocations = testCardLocations;
}

void ACoreCardGameModeBase::MoveRearrangeCards()
{
				for (int32 i = 0; i < testCards.Num(); i++)
				{
								FVector cardCurLocation = testCards[i]->GetActorLocation();
								FVector interpLocation = FMath::VInterpTo(cardCurLocation, testCardTempLocations[i], hoverMoveCardInterpDeltaTime, hoverMoveCardInterpSpeed);
								testCards[i]->SetActorLocation(interpLocation);
				}
}

void ACoreCardGameModeBase::MoveCard(FVector originLoc, FVector targetLoc, float midHeight)
{
				testMoveCard->SetActorLocation(originLoc);
				FVector midPoint = (originLoc + targetLoc) / 2.0;
				midPoint.Z = midHeight;
}



void ACoreCardGameModeBase::CheckEntitiesCreated()
{
				KBEngine::KBEngineApp::ENTITIES_MAP& entities = KBEngine::KBEngineApp::getSingleton().entities();
				for (auto& item : entities)
				{
								KBEngine::Entity* entity = item.Value;
								UKBEventData_onEnterWorld* eventData = NewObject<UKBEventData_onEnterWorld>();
								eventData->entityID = entity->id();
								eventData->spaceID = KBEngine::KBEngineApp::getSingleton().spaceID();
								eventData->isPlayer = entity->isPlayer();
								KBENGINE_EVENT_FIRE(KBEngine::KBEventTypes::onEnterWorld, eventData);
				}
}

void ACoreCardGameModeBase::GetAllPresetObjects()
{
				TArray<AActor*> presetCameras;
				UGameplayStatics::GetAllActorsOfClass(this, battleCameraBPClass, presetCameras);
				for (int32 i = 0; i < presetCameras.Num(); i++)
				{
								ABattleCamera* battleCamera = Cast<ABattleCamera>(presetCameras[i]);
								if (!camerasMap.Contains(battleCamera->cameraType))
								{
												camerasMap.Add(battleCamera->cameraType, battleCamera);
								}
				}

				TArray<AActor*> presetBoardGrids;
				UGameplayStatics::GetAllActorsOfClass(this, boardGridBPClass, presetBoardGrids);
				for (int32 i = 0; i < presetBoardGrids.Num(); i++)
				{
								ABoardGrid* boardGrid = Cast<ABoardGrid>(presetBoardGrids[i]);
								if (!boardGrids.Contains(boardGrid->gridId))
								{
												boardGrids.Add(boardGrid->gridId, boardGrid);
								}
				}
}

void ACoreCardGameModeBase::InitPreBattle()
{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(playerController);
				coreCardGamePC->InitSelectCardCamera();
}

void ACoreCardGameModeBase::CalibrateGridInfos(TArray<FBATTLE_GRID_INFO> gridInfos)
{
				for (TMap<int32, ABoardGrid*>::TConstIterator iter = boardGrids.CreateConstIterator(); iter; ++iter)
				{
								iter->Value->DemonstrateInitEffect();
				}
				for (int32 i = 0; i < gridInfos.Num(); i++)
				{
								if (!boardGrids[gridInfos[i].gridNb]->card)
								{
												// which means card is missed in this grid, we should spawn a brand new card corresponding to supplement information from server
												FVector spawnLoc = boardGrids[gridInfos[i].gridNb]->GetActorLocation();
												spawnLoc.Z += gridSpawnCardOffset;
												FRotator spawnRot = FRotator::ZeroRotator;
												ACard* suplementCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
												suplementCard->InitCard(allCardInfoMap[gridInfos[i].cardUid].cardName);
												suplementCard->hp = gridInfos[i].hp;
												suplementCard->defence = gridInfos[i].defence;
												suplementCard->agility = gridInfos[i].agility;
												suplementCard->inherentTags = gridInfos[i].tags;
												suplementCard->stateTags = gridInfos[i].stateTags;
												allCardMap.Add(gridInfos[i].cardUid, suplementCard);

												boardGrids[gridInfos[i].gridNb]->card = suplementCard;
								}
								else
								{
												if (boardGrids[gridInfos[i].gridNb]->card->cardUid != gridInfos[i].cardUid)
												{
																// which means a wrong card is placed in this grid
																// delete occupied card first
																FString cardUid = boardGrids[gridInfos[i].gridNb]->card->cardUid;
																if (boardGrids[gridInfos[i].gridNb]->card->IsValidLowLevel())
																{
																				boardGrids[gridInfos[i].gridNb]->card->ConditionalBeginDestroy();
																}
																allCardMap.Remove(cardUid);

																// replace or spawn sync card for this grid
																if (allCardMap.Contains(gridInfos[i].cardUid))
																{
																				// which means a existing card which should be placed in this grid is located somewhere else
																				// in this case we should delete that card, it doesn't matter because that "cavity" will be make up
																				if (allCardMap[gridInfos[i].cardUid]->IsValidLowLevel())
																				{
																								allCardMap[gridInfos[i].cardUid]->Destroy();
																				}
																				allCardMap.Remove(gridInfos[i].cardUid);

																				for (TMap<int32, ABoardGrid*>::TConstIterator iter = boardGrids.CreateConstIterator(); iter; ++iter)
																				{
																								if (iter->Value->card && iter->Value->card->cardUid == gridInfos[i].cardUid)
																								{
																												iter->Value->card = NULL;
																												break;
																								}
																				}
																}

																FVector spawnLoc = boardGrids[gridInfos[i].gridNb]->GetActorLocation();
																spawnLoc.Z += gridSpawnCardOffset;
																FRotator spawnRot = FRotator::ZeroRotator;
																ACard* replacedCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
																replacedCard->InitCard(allCardInfoMap[gridInfos[i].cardUid].cardName);
																replacedCard->hp = gridInfos[i].hp;
																replacedCard->defence = gridInfos[i].defence;
																replacedCard->agility = gridInfos[i].agility;
																replacedCard->inherentTags = gridInfos[i].tags;
																replacedCard->stateTags = gridInfos[i].stateTags;
																allCardMap.Add(gridInfos[i].cardUid, replacedCard);

																boardGrids[gridInfos[i].gridNb]->card = replacedCard;

												}
												else
												{
																// which means card id is correct, calibrate card information
																if (boardGrids[gridInfos[i].gridNb]->card->hp != gridInfos[i].hp)
																{
																				boardGrids[gridInfos[i].gridNb]->card->hp = gridInfos[i].hp;
																}
																if (boardGrids[gridInfos[i].gridNb]->card->defence != gridInfos[i].defence)
																{
																				boardGrids[gridInfos[i].gridNb]->card->defence = gridInfos[i].defence;
																}
																if (boardGrids[gridInfos[i].gridNb]->card->agility != gridInfos[i].agility)
																{
																				boardGrids[gridInfos[i].gridNb]->card->agility = gridInfos[i].agility;
																}
																if (boardGrids[gridInfos[i].gridNb]->card->inherentTags.Num() != gridInfos[i].tags.Num())
																{
																				boardGrids[gridInfos[i].gridNb]->card->inherentTags = gridInfos[i].tags;
																}
																else
																{
																				for (int32 j = 0; j < gridInfos[j].tags.Num(); j++)
																				{
																								if (!boardGrids[gridInfos[i].gridNb]->card->inherentTags.Contains(gridInfos[i].tags[j]))
																								{
																												boardGrids[gridInfos[i].gridNb]->card->inherentTags.Add(gridInfos[i].tags[j]);
																												// let's replace all tags to board grid
																												boardGrids[gridInfos[i].gridNb]->card->inherentTags = gridInfos[i].tags;
																												// update actor demonstration and ui
																												break;

																								}
																				}
																}

																if (boardGrids[gridInfos[i].gridNb]->card->stateTags.Num() != gridInfos[i].stateTags.Num())
																{
																				boardGrids[gridInfos[i].gridNb]->card->stateTags = gridInfos[i].stateTags;
																}
																else
																{
																				bool reqCorrection = false;
																				for (int32 j = 0; j < gridInfos[i].stateTags.Num(); j++)
																				{
																								bool findCorrespondStateTag = false;
																								for (int32 k = 0; k < boardGrids[gridInfos[i].gridNb]->card->stateTags.Num(); k++)
																								{
																												if (gridInfos[i].stateTags[j].stateName == boardGrids[gridInfos[i].gridNb]->card->stateTags[k].stateName)
																												{
																																findCorrespondStateTag = true;
																																if (gridInfos[i].stateTags[j].curCount != boardGrids[gridInfos[i].gridNb]->card->stateTags[k].curCount ||
																																				gridInfos[i].stateTags[j].stipulation != boardGrids[gridInfos[i].gridNb]->card->stateTags[k].stipulation)
																																{
																																				boardGrids[gridInfos[i].gridNb]->card->stateTags = gridInfos[i].stateTags;
																																				reqCorrection = true;
																																				break;
																																}
																												}
																								}
																								if (reqCorrection)
																								{
																												break;
																								}
																				}
																}
												}
								}
				}
}

void ACoreCardGameModeBase::CalibratePlayerCardInfos(TArray<FSYNC_CARD_INFO> allCardInfoList, TArray<FString> handCardUidList)
{
				for (int32 i = 0; i < allCardInfoList.Num(); i++)
				{
								if (allCardInfoMap.Contains(allCardInfoList[i].cardKey))
								{
												if (allCardInfoMap[allCardInfoList[i].cardKey].hp != allCardInfoList[i].hp)
												{
																allCardInfoMap[allCardInfoList[i].cardKey].hp = allCardInfoList[i].hp;
												}
												if (allCardInfoMap[allCardInfoList[i].cardKey].defence != allCardInfoList[i].defence)
												{
																allCardInfoMap[allCardInfoList[i].cardKey].defence = allCardInfoList[i].defence;
												}
								}
				}
}

void ACoreCardGameModeBase::CalibrateCurrentGlobalInfo(int32 curActionSequence, int32 curSwitchControllerSequence, uint8 curControllerNb)
{
				if (curActionSequence != receiveActionSequence)
				{
								receiveActionSequence = curActionSequence;
				}
				if (curSwitchControllerSequence != receiveSwitchControllerSequence)
				{
								receiveSwitchControllerSequence = curSwitchControllerSequence;
				}
				if (curControllerNb != receiveControllerNb)
				{
								receiveControllerNb = receiveControllerNb;
				}
}


void ACoreCardGameModeBase::onEnterWorld(const UKBEventData* eventData)
{
				const UKBEventData_onEnterWorld* onEnterWorldData = Cast<UKBEventData_onEnterWorld>(eventData);
				if (onEnterWorldData->isPlayer)
				{
								clientBattleState = ClientBattleState::ReqEnterRoom;
				}
}

void ACoreCardGameModeBase::ReqChangeSelectCard(FString changeCardKey)
{
				UKBEventData_reqChangeSelectCard* eventData = NewObject<UKBEventData_reqChangeSelectCard>();
				eventData->changeCardKey = changeCardKey;
				KBENGINE_EVENT_FIRE("ReqChangeSelectCard", eventData);
}

void ACoreCardGameModeBase::ReqFinishSelectCards()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqFinishSelectCards", eventData);
}

void ACoreCardGameModeBase::ReqUpdateSelectedCard()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqUpdateSelectedCard", eventData);
}

void ACoreCardGameModeBase::ReqSyncHeartBeat()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqSyncHeartBeat", eventData);
}

void ACoreCardGameModeBase::ReqLatestBattleInfo()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqLatestBattleInfo", eventData);
}

void ACoreCardGameModeBase::MultiPlayerReqPlayCardAction(int32 actionSequence, FString cardUid, int32 gridNb)
{
				UKBEventData_reqPlayCardAction* eventData = NewObject<UKBEventData_reqPlayCardAction>();
				eventData->actionSequence = actionSequence;
				eventData->cardUid = cardUid;
				eventData->gridNb = gridNb;
				KBENGINE_EVENT_FIRE("ReqPlayCardAction", eventData);
}

void ACoreCardGameModeBase::MultiPlayerReqLaunchCardSkill(int32 actionSequence, FString cardUid, FString skillName, int32 launchGridNb, int32 targetGridNb)
{
				UKBEventData_reqLaunchCardSkill* eventData = NewObject<UKBEventData_reqLaunchCardSkill>();
				eventData->actionSequence = actionSequence;
				eventData->cardUid = cardUid;
				eventData->skillName = skillName;
				eventData->launchGridNb = launchGridNb;
				eventData->targetGridNb = targetGridNb;
				KBENGINE_EVENT_FIRE("ReqLaunchCardSkill", eventData);
}

void ACoreCardGameModeBase::onUpdateGridInfoList(const UKBEventData* eventData)
{
				/*auto updateGridInfoListData = Cast<UKBEventData_onUpdateGridInfoList>(eventData);
				if (updateGridInfoListData->updateInfoList.updateId - curReceiveUpdateId == 1)
				{
								// which means receives latest data
								curReceiveUpdateId = updateGridInfoListData->updateInfoList.updateId;
								for (int32 i = 0; i < updateGridInfoListData->updateInfoList.updateGridList.Num(); i++)
								{
												if (updateGridInfoListData->updateInfoList.updateGridList[i].updateType == "createCard")
												{
																// TODO: should consider cases which cards missed or grid occupied
																boardGrids[updateGridInfoListData->updateInfoList.updateGridList[i].gridNb]->card =
																				allCards[updateGridInfoListData->updateInfoList.updateGridList[i].cardUid];

																// and then play animation
												}
												else if (updateGridInfoListData->updateInfoList.updateGridList[i].updateType == "deleteCard")
												{

												}
								}
				}*/
}


void ACoreCardGameModeBase::onReceiveUpdateCoreGame(const UKBEventData* eventData)
{
				auto updateBattleEventData = Cast<UKBEventData_onUpdateBattle>(eventData);

				/*TArray<int32> latestOccupiedGrids;
				// update all grids
				for (int32 i = 0; i < updateBattleEventData->updateInfo.updateInfos.Num(); i++)
				{
								if (occupiedGridCardMap.Contains(updateBattleEventData->updateInfo.updateInfos[i].gridNb))
								{

								}
								else
								{
												// which means this is a createtion action
												occupiedGridCardMap.Add(updateBattleEventData->updateInfo.updateInfos[i].gridNb,
																allCards[updateBattleEventData->updateInfo.updateInfos[i].cardUid]);
								}
								latestOccupiedGrids.Add(updateBattleEventData->updateInfo.updateInfos[i].gridNb);
				}

				for (TPair<int32, ACard*>& element : occupiedGridCardMap)
				{
								if (!latestOccupiedGrids.Contains(element.Key))
								{
												// which means card in this grid is removed!
												// we should move this card to grave
								}
				}*/
}

void ACoreCardGameModeBase::InitDone_Implementation()
{

}

void ACoreCardGameModeBase::onSyncBattleResult(const UKBEventData* eventData)
{
				// which means battle ends
				const UKBEventData_onSyncBattleResult* syncBattleResultData = Cast<UKBEventData_onSyncBattleResult>(eventData);

}

void ACoreCardGameModeBase::onSyncChangeHandCardSuccess(const UKBEventData* eventData)
{
}

void ACoreCardGameModeBase::onSyncPlayerBattleInfo(const UKBEventData* eventData)
{
				// when client receive this message, it means avatar is ready both on server and client sides

}

void ACoreCardGameModeBase::onSyncReceiveEnterRoom(const UKBEventData* eventData)
{
				const UKBEventData_onSyncReceiveEnterRoom* onSyncEnterRoomData = Cast<UKBEventData_onSyncReceiveEnterRoom>(eventData);
				if (onSyncEnterRoomData->result == 1)
				{
								// which means server has received enter room message
								// no need to keep checking again
								clientBattleState = ClientBattleState::SelectCard;
				}
				else
				{
								// which means server has already received room entering message more than once
								// which implies that client maybe lose some messages
								clientBattleState = ClientBattleState::SelectCard;
				}
}

void ACoreCardGameModeBase::onSyncReceiveFinishCardSelection(const UKBEventData* eventData)
{
				// nofity battle widget
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(playerController);
				coreCardPC->ReceiveFinishCardSelection();
}

void ACoreCardGameModeBase::onSyncExhaustCardReplacement(const UKBEventData* eventData)
{
				// which means this player lost some information
				// player should ask server to supplement all informations
				UKBEventData* reqUpdateSelectCardData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqUpdateSelectedCard", reqUpdateSelectCardData);
}

void ACoreCardGameModeBase::onSyncHeartBeat(const UKBEventData* eventData)
{

}

void ACoreCardGameModeBase::onSyncLatestBattleState(const UKBEventData* eventData)
{
				// Compare current operation sequence

}

void ACoreCardGameModeBase::onSyncResumeBattle(const UKBEventData* eventData)
{
				// which means server has finished switch controller interlude
				// restart counting again
}

void ACoreCardGameModeBase::onSyncUpdateSelectedCards(const UKBEventData* eventData)
{


}

void ACoreCardGameModeBase::onSyncRoomStartBattle(const UKBEventData* eventData)
{
				// when this message is received, it means server passes card selection interlude
				// this only means that we could spawn time count down slider on UI menu
				interludeState = InterludeState::Default;
				clientBattleState = ClientBattleState::InBattle;
				curCountingTick = 0.0;
}

void ACoreCardGameModeBase::onSyncSelectCardInterlude(const UKBEventData* eventData)
{
				// which means all players have already finished card selection
				// play interlude and setup cards on board
				curCountingTick = 0.0;
				interludeState = InterludeState::SelectCardDemoPauseInterlude;
}

void ACoreCardGameModeBase::onSyncSwitchController(const UKBEventData* eventData)
{
				const UKBEventData_onSyncSwitchController* switchControllerData = Cast<UKBEventData_onSyncSwitchController>(eventData);

}

void ACoreCardGameModeBase::onSyncTimeInterval(const UKBEventData* eventData)
{
}

void ACoreCardGameModeBase::SpawnSelectCard()
{

}

