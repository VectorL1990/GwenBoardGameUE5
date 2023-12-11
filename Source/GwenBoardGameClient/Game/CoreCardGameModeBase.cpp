// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCardGameModeBase.h"
#include "Scripts/BattleEvents.h"

void ACoreCardGameModeBase::BeginPlay()
{
				InitEvents();
				InitKBEMain();
				InitDone();
				GetAllPresetObjects();
}

void ACoreCardGameModeBase::InitEvents()
{
				Super::InitEvents();
				KBENGINE_REGISTER_EVENT("onStopCardSelection", onStopCardSelection);
				KBENGINE_REGISTER_EVENT("onSyncBattleResult", onSyncBattleResult);
				KBENGINE_REGISTER_EVENT("onSyncChangeHandCardSuccess", onSyncChangeHandCardSuccess);
				KBENGINE_REGISTER_EVENT("onSyncExhaustCardReplacement", onSyncExhaustCardReplacement);
				KBENGINE_REGISTER_EVENT("onSyncHeartBeat", onSyncHeartBeat);
				KBENGINE_REGISTER_EVENT("onSyncLatestBattleState", onSyncLatestBattleState);
				KBENGINE_REGISTER_EVENT("onSyncPlayerBattleInfo", onSyncPlayerBattleInfo);
				KBENGINE_REGISTER_EVENT("onSyncResumeBattle", onSyncResumeBattle);
				KBENGINE_REGISTER_EVENT("onSyncRoomStartBattle", onSyncRoomStartBattle);
				KBENGINE_REGISTER_EVENT("onSyncSelectCardInterlude", onSyncSelectCardInterlude);
				KBENGINE_REGISTER_EVENT("onSyncSwitchController", onSyncSwitchController);
				KBENGINE_REGISTER_EVENT("onSyncTimeInterval", onSyncTimeInterval);
				KBENGINE_REGISTER_EVENT("onSyncUpdateSelectedCards", onSyncUpdateSelectedCards);
}

void ACoreCardGameModeBase::Tick(float deltaTime)
{
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

				if (clientBattleState == ClientBattleState::SyncHeartBeat)
				{
								if (curCountingTick >= battleStateTicksMap["SyncHeartBeatInterval"])
								{
												ReqSyncHeartBeat();
								}
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

void ACoreCardGameModeBase::ReqEnterRoom()
{
				UKBEventData* eventData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqEnterRoom", eventData);
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

void ACoreCardGameModeBase::onStopCardSelection(const UKBEventData* eventData)
{}

void ACoreCardGameModeBase::onSyncBattleResult(const UKBEventData* eventData)
{}

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
																handCardMap[onSyncChangeHandCardSuccessData->changeHandCardKey]->Destroy();
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

void ACoreCardGameModeBase::onSyncExhaustCardReplacement(const UKBEventData* eventData)
{
				// which means this player lost some information
				// player should ask server to supplement all informations
				UKBEventData* reqUpdateSelectCardData = NewObject<UKBEventData>();
				KBENGINE_EVENT_FIRE("ReqUpdateSelectedCard", reqUpdateSelectCardData);
}

void ACoreCardGameModeBase::onSyncHeartBeat(const UKBEventData* eventData)
{}

void ACoreCardGameModeBase::onSyncLatestBattleState(const UKBEventData* eventData)
{}

void ACoreCardGameModeBase::onSyncResumeBattle(const UKBEventData* eventData)
{}

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

}

void ACoreCardGameModeBase::onSyncSelectCardInterlude(const UKBEventData* eventData)
{
				// which means all players have already finished card selection
				// play interlude and setup cards on board
				curCountingTick = 0.0;
				interludeState = InterludeState::SelectCardDemoPauseInterlude;
}

void ACoreCardGameModeBase::onSyncSwitchController(const UKBEventData* eventData)
{}

void ACoreCardGameModeBase::onSyncTimeInterval(const UKBEventData* eventData)
{}

void ACoreCardGameModeBase::InitPlayerBattleInfoDone(TArray<FString> cardList)
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

