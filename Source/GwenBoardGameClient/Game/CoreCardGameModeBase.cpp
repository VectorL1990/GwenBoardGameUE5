// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "CoreCardGamePC.h"
#include "Engine/KBEngine.h"
#include "Engine/Entity.h"
#include "Scripts/BattleEvents.h"

void ACoreCardGameModeBase::BeginPlay()
{
				InitEvents();
				InitKBEMain();
				InitDone();
				GetAllPresetObjects();
				InitPreBattle();
				CheckEntitiesCreated();
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
				if (isSinglePlay)
				{
								
				}
				else
				{

				}

				if (interludeState == InterludeState::SelectCardDemoPauseInterlude)
				{
								// Pause for a while for final select cards demonstration
								if (curCountingTick >= interludeStateTicksMap["SelectCardDemoPauseInterlude"])
								{
												// delete all demonstrated hand cards fist
												for (TMap<FString, ACard*>::TConstIterator iter = handCardMap.CreateConstIterator(); iter; ++iter)
												{
																if (iter->Value->IsValidLowLevel())
																{
																				iter->Value->Destroy();
																}
												}
												handCardMap.Empty();

												// move camera to specific location
												APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
												AActor* targetCamera = camerasMap[CameraType::SelectCardCamera];
												playerController->SetViewTargetWithBlend(targetCamera, 2.0);


												// which means selected cards demonstration ends
												curCountingTick = 0.0;
												interludeState = InterludeState::MoveCameraCardSelectionToBattle;
								}
								else
								{
												curCountingTick += deltaTime;
								}
				}
				else if (interludeState == InterludeState::MoveCameraCardSelectionToBattle)
				{
								if (curCountingTick >= interludeStateTicksMap["MoveCameraCardSelectionToBattle"])
								{
												// when camera motion done, we should demonstrate our board right now
												for (TMap<int32, ABoardGrid*>::TConstIterator iter = boardGrids.CreateConstIterator(); iter; ++iter)
												{
																iter->Value->DemonstrateInitEffect();
												}

												curCountingTick = 0.0;
												interludeState = InterludeState::DemoBoardInterlude;
								}
								else
								{
												curCountingTick += deltaTime;
								}
				}
				else if (interludeState == InterludeState::DemoBoardInterlude)
				{
								if (curCountingTick >= interludeStateTicksMap["DemoBoardInterlude"])
								{
												// when battle board init effect demonstration is done, start actual battle

												curCountingTick = 0.0;
												// which means stop interlude counting, begin battle counting
												interludeState = InterludeState::Default;
								}
								else
								{
												curCountingTick += deltaTime;
								}
				}

				if (clientBattleState == ClientBattleState::ReqEnterRoom)
				{
								if (curReqEnterRoomTick >= battleStateTicksMap["ReqEnterRoom"])
								{
												// which means account has already entered world
												// trigger reqEnterRoom so that server gives side gives client to avatar
												ReqEnterRoom();
												curReqEnterRoomTick = 0.0;
								}
								else
								{
												curReqEnterRoomTick += deltaTime;
								}
				}
				else if (clientBattleState == ClientBattleState::SelectCard)
				{
								SpawnSelectCard();
				}
				else if (clientBattleState == ClientBattleState::InBattle)
				{
								// we should always keep sending sync information to server
								if (curBattleStateTick >= battleStateTicksMap["SyncHeartBeatInterval"])
								{
												ReqSyncHeartBeat();
								}

								if (curBattleStateTick >= battleStateTicksMap["SyncBattleInterval"])
								{
												ReqLatestBattleInfo();
								}
				}
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

void ACoreCardGameModeBase::ReqPlayCard(int32 targetGridNb, int32 playCardUid)
{
				UKBEventData_reqSendAction* eventData = NewObject<UKBEventData_reqSendAction>();
				eventData->actionType = "playCard";
				FPLAY_CARD_KBE_INFO playCardInfo;
				playCardInfo.playCardId = playCardUid;
				playCardInfo.targetGridNb = targetGridNb;
				eventData->playCardInfo = playCardInfo;
				KBENGINE_EVENT_FIRE("ReqSendAction", eventData);
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

void ACoreCardGameModeBase::ReqPlayCardAction(int32 actionSequence, FString cardUid, int32 gridNb)
{
				UKBEventData_reqPlayCardAction* eventData = NewObject<UKBEventData_reqPlayCardAction>();
				eventData->actionSequence = actionSequence;
				eventData->cardUid = cardUid;
				eventData->gridNb = gridNb;
				KBENGINE_EVENT_FIRE("ReqPlayCardAction", eventData);
}

void ACoreCardGameModeBase::ReqLaunchCardSkill(int32 actionSequence, FString cardUid, FString skillName, int32 launchGridNb, int32 targetGridNb)
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
				const UKBEventData_onSyncChangeHandCardSuccess* onSyncChangeHandCardSuccessData = Cast<UKBEventData_onSyncChangeHandCardSuccess>(eventData);
				if (onSyncChangeHandCardSuccessData->changeSelectCardNb - curChangeSelectCardNb == 1)
				{
								if (handCardKeyList.Contains(onSyncChangeHandCardSuccessData->changeHandCardKey) &&
												pileCardKeyList.Contains(onSyncChangeHandCardSuccessData->changePileCardKey))
								{
												// which means information is complete, nothing missed
												curChangeSelectCardNb = onSyncChangeHandCardSuccessData->changeSelectCardNb;

												// delete choosen hand card and replace it with card in card pile
												int32 changeHandCardKeyLoc = handCardKeyList.Find(onSyncChangeHandCardSuccessData->changeHandCardKey);
												handCardKeyList[changeHandCardKeyLoc] = onSyncChangeHandCardSuccessData->changePileCardKey;
												// remove target pile card key from pileCardKeyList
												pileCardKeyList.RemoveAt(0);
												pileCardKeyList.Add(onSyncChangeHandCardSuccessData->changeHandCardKey);
												if (!handCardMap.Contains(onSyncChangeHandCardSuccessData->changeHandCardKey))
												{
																// which means 3d card error, regenerate all 3d cards!
												}
												else
												{
																FVector changeHandCardLoc = handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->GetActorLocation();
																FRotator changeHandCardRot = handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->GetActorRotation();
																if (handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->IsValidLowLevel())
																{
																				handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->Destroy();
																}
																handCardMap.Remove(onSyncChangeHandCardSuccessData->changeHandCardKey);
																ACard* changeHandCard = GetWorld()->SpawnActor<ACard>(cardBPClass, changeHandCardLoc, changeHandCardRot);
																changeHandCard->cardStatus = BattleCardStatus::Select;
																changeHandCard->InitCard(allCardInfoMap[onSyncChangeHandCardSuccessData->changeHandCardKey].cardName);
																handCardMap.Add(onSyncChangeHandCardSuccessData->changeHandCardKey, changeHandCard);
												}
												if (maxChangeSelectCardNb - curChangeSelectCardNb <= 1)
												{
																// which means all opportunity exhausted
																// tell server card selection done
																ReqFinishSelectCards();
												}
								}
								else
								{
												// which means information loss, require latest select hand cards!
												ReqUpdateSelectedCard();
								}
				}
				else
				{
								// which means information loss, require latest select hand cards!
								ReqUpdateSelectedCard();
				}
}

void ACoreCardGameModeBase::onSyncPlayerBattleInfo(const UKBEventData* eventData)
{
				// when client receive this message, it means avatar is ready both on server and client sides
				const UKBEventData_onSyncPlayerBattleInfo* onSyncPlayerBattleInfoData = Cast<UKBEventData_onSyncPlayerBattleInfo>(eventData);

				for (int32 i = 0; i < onSyncPlayerBattleInfoData->cardList.Num(); i++)
				{
								if (!onSyncPlayerBattleInfoData->handCardList.Contains(onSyncPlayerBattleInfoData->cardList[i].cardKey))
								{
												pileCardKeyList.Add(onSyncPlayerBattleInfoData->cardList[i].cardKey);
								}
								allCardInfoMap.Add(onSyncPlayerBattleInfoData->cardList[i].cardKey, onSyncPlayerBattleInfoData->cardList[i]);
				}
				handCardKeyList = onSyncPlayerBattleInfoData->handCardList;
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
				const UKBEventData_onSyncLatestBattleState* latestBattleStateData = Cast<UKBEventData_onSyncLatestBattleState>(eventData);
				if (receiveActionSequence < latestBattleStateData->curActionSequence ||
								receiveSwitchControllerSequence != latestBattleStateData->curSwitchControllerSequence ||
								receiveControllerNb != latestBattleStateData->curControllerNb)
				{
								// which means client got information latency or information loss
								// we should use sync information for battle recovering
								CalibrateGridInfos(latestBattleStateData->updateGridInfos);
								CalibratePlayerCardInfos(latestBattleStateData->cardList, latestBattleStateData->handCardList);
								CalibrateCurrentGlobalInfo(latestBattleStateData->curActionSequence, latestBattleStateData->curSwitchControllerSequence, latestBattleStateData->curControllerNb);
				}
}

void ACoreCardGameModeBase::onSyncResumeBattle(const UKBEventData* eventData)
{
				// which means server has finished switch controller interlude
				// restart counting again
}

void ACoreCardGameModeBase::onSyncUpdateSelectedCards(const UKBEventData* eventData)
{
				const UKBEventData_onSyncUpdateSelectedCards* onSyncUpdateSelectedCardsData = Cast<UKBEventData_onSyncUpdateSelectedCards>(eventData);
				// delete all existing hand cards and replace them with data from server
				for (TMap<FString, ACard*>::TConstIterator iter = handCardMap.CreateConstIterator(); iter; ++iter)
				{
								if (iter->Value->IsValidLowLevel())
								{
												iter->Value->Destroy();
								}
				}
				handCardMap.Empty();

				allCardInfoMap.Empty();
				pileCardKeyList.Empty();
				for (int32 i = 0; i < onSyncUpdateSelectedCardsData->cardList.Num(); i++)
				{
								if (!onSyncUpdateSelectedCardsData->handCardList.Contains(onSyncUpdateSelectedCardsData->cardList[i].cardKey))
								{
												pileCardKeyList.Add(onSyncUpdateSelectedCardsData->cardList[i].cardKey);
								}
								allCardInfoMap.Add(onSyncUpdateSelectedCardsData->cardList[i].cardKey, onSyncUpdateSelectedCardsData->cardList[i]);
				}
				handCardKeyList = onSyncUpdateSelectedCardsData->handCardList;

				FRotator spawnRot = FRotator::ZeroRotator;
				for (int32 i = 0; i < handCardKeyList.Num(); i++)
				{
								if (i >= selectCardSpawnPts.Num())
								{
												break;
								}
								FVector spawnLoc = selectCardSpawnPts[i];
								ACard* handCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
								handCard->cardStatus = BattleCardStatus::Select;
								handCard->InitCard(allCardInfoMap[handCardKeyList[i]].cardName);
								handCardMap.Add(handCardKeyList[i], handCard);
				}

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
				FRotator spawnRot = FRotator::ZeroRotator;
				for (int32 i = 0; i < handCardKeyList.Num(); i++)
				{
								if (i >= selectCardSpawnPts.Num())
								{
												break;
								}
								FVector spawnLoc = selectCardSpawnPts[i];
								ACard* handCard = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, spawnRot);
								handCard->cardStatus = BattleCardStatus::Select;
								handCard->InitCard(allCardInfoMap[handCardKeyList[i]].cardName);
								handCardMap.Add(handCardKeyList[i], handCard);
				}
}

