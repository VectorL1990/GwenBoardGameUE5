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
	//InitEvents();
	//InitKBEMain();
	InitDone();
	GetAllPresetObjects();
	InitPreBattle();
	//CheckEntitiesCreated();

	CalculateCardSpread();
	for (int32 i = 0; i < sectionZeroHandBattleCards.Num(); i++)
	{
		sectionZeroHandBattleCards[i]->SetActorLocation(sectionZeroCardLocations[i]);
		sectionZeroHandBattleCards[i]->SetActorRotation(sectionZeroCardRots[i]);
	}
	for (int32 i = 0; i < sectionOneHandBattleCards.Num(); i++)
	{
		sectionOneHandBattleCards[i]->SetActorLocation(sectionOneCardLocations[i]);
		sectionOneHandBattleCards[i]->SetActorRotation(sectionOneCardRots[i]);
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

	//TrainPlayGameLoop(deltaTime);

	if (isSinglePlay)
	{
		
	}
	else
	{

	}


	if (aiRunnable->aiRunnableState == EAIRunnableState::NewState)
	{
		TArray<int32> allCardIds;
		TArray<int32> allSectionZeroHandCardIds;
		TArray<int32> allSectionOneHandCardIds;
		TArray<int32> allSectionZeroGraveCardIds;
		TArray<int32> allSectionOneGraveCardIds;
		for (int32 i = 0; i < aiRunnable->mcts->realBoard.boardRows.Num(); i++)
		{
			for (int32 j = 0; j < aiRunnable->mcts->realBoard.boardRows[i].colCardInfos.Num(); j++)
			{
				int32 cardId = aiRunnable->mcts->realBoard.boardRows[i].colCardInfos[j];
				if (cardId == -1)
				{

				}
				else
				{
					if (!allBattleCards.Contains(cardId))
					{
						// we should spawn card here
					}
					else
					{
						allBattleCards[cardId]->gridX = j;
						allBattleCards[cardId]->gridY = i;
						allBattleCards[cardId]->camp = aiRunnable->mcts->realBoard.allInstanceCardInfo[cardId].camp;
						if (i < UGlobalConstFunctionLibrary::graveCardSectionRow ||
							i >= (UGlobalConstFunctionLibrary::graveCardSectionRow +
								UGlobalConstFunctionLibrary::playCardSectionRow +
								UGlobalConstFunctionLibrary::boardSectionRow +
								UGlobalConstFunctionLibrary::playCardSectionRow))
						{
							// which means this card goes to grave area
							UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
							UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
							if (gwenGI->playerCambNb == allBattleCards[cardId]->camp)
							{
								allSectionZeroGraveCardIds.Add(cardId);
							}
							else
							{
								allSectionOneGraveCardIds.Add(cardId);
							}
						}
						else if ((i >= UGlobalConstFunctionLibrary::graveCardSectionRow &&
							i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow) ||
							(i >= UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow &&
								i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow * 2 + UGlobalConstFunctionLibrary::boardSectionRow))
						{
							// which means this card belongs to hand card area
							UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
							UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
							if (gwenGI->playerCambNb == allBattleCards[cardId]->camp)
							{
								allSectionZeroHandCardIds.Add(cardId);
							}
							else
							{
								allSectionOneHandCardIds.Add(cardId);
							}
						}
					}
				}
				
				allCardIds.Add(cardId);
			}
		}

		// delete cards not existed first
		for (TMap<int32, ACard*>::TConstIterator iter = allBattleCards.CreateConstIterator(); iter; ++iter)
		{
			if (!allCardIds.Contains(iter->Key))
			{
				// which means this card doesn't exist, we should delete it
			}
			else
			{
				int32 curHp = aiRunnable->mcts->realBoard.allInstanceCardInfo[iter->Key].curHp;
				int32 curDefence = aiRunnable->mcts->realBoard.allInstanceCardInfo[iter->Key].curDefence;
				iter->Value->UpdateCard(curHp, curDefence);
			}
		}

		// move grave cards to grave
		for (int32 i = 0; i < allSectionZeroGraveCardIds.Num(); i++)
		{
			if (allBattleCards[allSectionZeroGraveCardIds[i]]->cardStatus != BattleCardStatus::InGrave)
			{
				// move this card to grave
				FVector cardGraveOffset = allBattleCards[allSectionZeroGraveCardIds[i]]->GetActorLocation() - sectionZeroGraveAreaLocation;
				if (cardGraveOffset.Length() > 100.0)
				{
					allBattleCards[allSectionZeroGraveCardIds[i]]->SetActorLocation(sectionZeroGraveAreaLocation);
				}
				allBattleCards[allSectionZeroGraveCardIds[i]]->cardStatus = BattleCardStatus::InGrave;
			}
		}
		for (int32 i = 0; i < allSectionOneGraveCardIds.Num(); i++)
		{
			if (allBattleCards[allSectionZeroGraveCardIds[i]]->cardStatus != BattleCardStatus::InGrave)
			{
				// move this card to grave
				FVector cardGraveOffset = allBattleCards[allSectionZeroGraveCardIds[i]]->GetActorLocation() - sectionOneGraveAreaLocation;
				if (cardGraveOffset.Length() > 100.0)
				{
					allBattleCards[allSectionZeroGraveCardIds[i]]->SetActorLocation(sectionOneGraveAreaLocation);
				}
				allBattleCards[allSectionZeroGraveCardIds[i]]->SetActorLocation(sectionOneGraveAreaLocation);
			}
		}


		// move cards from pile to hand
		for (int32 i = 0; i < allSectionZeroHandCardIds.Num(); i++)
		{
			bool findRenderHandCard = false;
			for (int32 j = 0; j < sectionZeroHandBattleCards.Num(); j++)
			{
				if (sectionZeroHandBattleCards[j]->cardId == allSectionZeroHandCardIds[i])
				{
					findRenderHandCard = true;
					break;
				}
			}
			if (!findRenderHandCard)
			{
				
			}
		}
		for (int32 i = 0; i < allSectionOneHandCardIds.Num(); i++)
		{
			bool findRenderHandCard = false;
			for (int32 j = 0; j < sectionOneHandBattleCards.Num(); j++)
			{
				if (sectionOneHandBattleCards[j]->cardId == allSectionOneHandCardIds[i])
				{
					findRenderHandCard = true;
					break;
				}
			}
			if (!findRenderHandCard)
			{

			}
		}


		// move cards from hand to board or pile
		int32 sectionZeroHandCardNb = 0;
		while (sectionZeroHandCardNb < sectionZeroHandBattleCards.Num())
		{
			if (!allSectionZeroHandCardIds.Contains(sectionZeroHandBattleCards[sectionZeroHandCardNb]->cardId))
			{
				int32 boardGridX = sectionZeroHandBattleCards[sectionZeroHandCardNb]->gridX;
				int32 boardGridY = sectionZeroHandBattleCards[sectionZeroHandCardNb]->gridY - 
					UGlobalConstFunctionLibrary::graveCardSectionRow - 
					UGlobalConstFunctionLibrary::playCardSectionRow;
				int32 boardGridId = boardGridY * UGlobalConstFunctionLibrary::maxCol + boardGridX;
				
				FVector destLocation = boardGrids[boardGridId]->GetActorLocation() + gridCardVerticalOffset;
				sectionZeroHandBattleCards[sectionZeroHandCardNb]->SetActorLocation(destLocation);
				sectionZeroHandBattleCards[sectionZeroHandCardNb]->SetActorRotation(FRotator::ZeroRotator);
				sectionZeroHandBattleCards.RemoveAt(sectionZeroHandCardNb);
			}
			else
			{
				sectionZeroHandCardNb += 1;
			}
		}
		int32 sectionOneHandCardNb = 0;
		while (sectionOneHandCardNb < sectionOneHandBattleCards.Num())
		{
			if (!allSectionOneHandCardIds.Contains(sectionOneHandBattleCards[sectionOneHandCardNb]->cardId))
			{
				int32 boardGridX = sectionOneHandBattleCards[sectionOneHandCardNb]->gridX;
				int32 boardGridY = sectionOneHandBattleCards[sectionOneHandCardNb]->gridY -
					UGlobalConstFunctionLibrary::graveCardSectionRow -
					UGlobalConstFunctionLibrary::playCardSectionRow;
				int32 boardGridId = boardGridY * UGlobalConstFunctionLibrary::maxCol + boardGridX;

				FVector destLocation = boardGrids[boardGridId]->GetActorLocation() + gridCardVerticalOffset;
				sectionOneHandBattleCards[sectionOneHandCardNb]->SetActorLocation(destLocation);
				sectionOneHandBattleCards[sectionOneHandCardNb]->SetActorRotation(FRotator::ZeroRotator);
				sectionOneHandBattleCards.RemoveAt(sectionOneHandCardNb);
			}
			else
			{
				sectionOneHandCardNb += 1;
			}
		}



		// We should refresh all nodes to Mct widget here.
		//aiRunnable->mcts->treeRoot
	}
	else if (aiRunnable->aiRunnableState == EAIRunnableState::)
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(playerController);
		coreCardPC->mctReplayMenu->RefreshMctsMenu();
		aiRunnable->aiRunnableState = EAIRunnableState::Default;
	}
}

void ACoreCardGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (aiRunnable)
	{
		aiRunnable->Stop();
	}
}





void ACoreCardGameModeBase::FinishCardSelection()
{
	// Trigger by Select Card widget, next operation depends on single or multi players game mode
}

void ACoreCardGameModeBase::TestTriggerSimulation()
{
	aiRunnable->TriggerTestGetAction();
}

void ACoreCardGameModeBase::TestTriggerAction(uint8 campNb, int32 launchX, int32 launchY, int32 targetX, int32 targetY, ActionType actionType)
{
	aiRunnable->TriggerAssignAction(campNb, launchX, launchY, targetX, targetY, actionType);
}

void ACoreCardGameModeBase::DemonstrateMctsTreeNode(UMctsTreeNode* node)
{
	for (TMap<int32, AReplayCard*>::TConstIterator iter = allReplayCards.CreateConstIterator(); iter; ++iter)
	{
		iter->Value->ConditionalBeginDestroy();
	}
	allReplayCards.Empty();


	for (int32 i = 0; i < node->replayBoardRows.Num(); i++)
	{
		for (int32 j = 0; j < node->replayBoardRows[i].colCardInfos.Num(); j++)
		{
			int32 cardId = node->replayBoardRows[i].colCardInfos[j];
			if (cardId != -1)
			{
				if (!allReplayCards.Contains(cardId))
				{
					// we should spawn card here
					if (i < UGlobalConstFunctionLibrary::graveCardSectionRow ||
						i >= (UGlobalConstFunctionLibrary::graveCardSectionRow +
							UGlobalConstFunctionLibrary::playCardSectionRow +
							UGlobalConstFunctionLibrary::boardSectionRow +
							UGlobalConstFunctionLibrary::playCardSectionRow))
					{
						// which means it's a grave card
					}
					else if ((i >= UGlobalConstFunctionLibrary::graveCardSectionRow &&
						i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow) ||
						(i >= UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow + UGlobalConstFunctionLibrary::boardSectionRow &&
							i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow * 2 + UGlobalConstFunctionLibrary::boardSectionRow))
					{
						// which means it's in play card section

						FVector spawnLocation;
						if (i < UGlobalConstFunctionLibrary::graveCardSectionRow + UGlobalConstFunctionLibrary::playCardSectionRow)
						{
							FVector cardOffset(replayHandCardOffset * j, 0.0, 0.0);
							spawnLocation = replaySectionZeroFirstHandCardLoc + cardOffset;
						}
						else
						{
							FVector cardOffset(-replayHandCardOffset * j, 0.0, 0.0);
							spawnLocation = replaySectionOneFirstHandCardLoc + cardOffset;
						}
						AReplayCard* replayCard = GetWorld()->SpawnActor<AReplayCard>(replayCardBPClass, spawnLocation, FRotator::ZeroRotator);
						replayCard->Init(node->allReplayInstanceCardInfo[cardId].originCardInfo.cardName,
							node->allReplayInstanceCardInfo[cardId].curHp,
							node->allReplayInstanceCardInfo[cardId].curDefence);
						allReplayCards.Add(cardId, replayCard);
					}
					else
					{
						// which means it's in board section
						int32 boardGridX = j;
						int32 boardGridY = i - UGlobalConstFunctionLibrary::graveCardSectionRow - UGlobalConstFunctionLibrary::playCardSectionRow;
						int32 boardGridId = boardGridY * UGlobalConstFunctionLibrary::maxCol + boardGridX;

						FVector destLocation = boardGrids[boardGridId]->GetActorLocation() + replayGridCardVerticalOffset;
						AReplayCard* replayCard = GetWorld()->SpawnActor<AReplayCard>(replayCardBPClass, destLocation, FRotator::ZeroRotator);
						replayCard->Init(node->allReplayInstanceCardInfo[cardId].originCardInfo.cardName,
							node->allReplayInstanceCardInfo[cardId].curHp,
							node->allReplayInstanceCardInfo[cardId].curDefence);
						allReplayCards.Add(cardId, replayCard);
					}
				}
			}
		}
	}



	int32 launchX = -1;
	int32 launchY = -1;
	int32 targetX = -1;
	int32 targetY = -1;
	ActionType actionType;
	aiRunnable->mcts->realBoard.ActionDecoding(node->actionId, launchX, launchY, targetX, targetY, actionType);
	if (actionType == ActionType::EndRound)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "This is an end round action");
	}
	else
	{
		if (actionType == ActionType::PlayCard)
		{

		}
		else if (actionType == ActionType::LaunchSkill)
		{
			int32 launchCardUid = aiRunnable->mcts->realBoard.boardRows[launchY].colCardInfos[launchX];
			int32 targetCardUid = aiRunnable->mcts->realBoard.boardRows[targetY].colCardInfos[targetX];
			FVector launchCardLoc = allBattleCards[launchCardUid]->GetActorLocation();
			FVector targetCardLoc = allBattleCards[targetCardUid]->GetActorLocation();
			UKismetSystemLibrary::DrawDebugLine(this, launchCardLoc, targetCardLoc, FLinearColor::Green, 1.0, 10.0);
		}
	}
}

void ACoreCardGameModeBase::SimulateTrainAction(float dT)
{
	if (aiTrainPlayerActionCount >= aiTrainPlayerActionInterval)
	{
		// get train simulation action and launch
		int32 actionId = -1;
		if (curSectionNb == 0)
		{
			aiRunnable->TriggerMctsGetAction(0);
			//sectionZeroMctsPlayer->mcts->GetAction(sectionZeroMctsPlayer->sectionNb, battleBoard, actionId);
		}
		else
		{
			aiRunnable->TriggerMctsGetAction(1);
			//sectionOneMctsPlayer->mcts->GetAction(sectionOneMctsPlayer->sectionNb, battleBoard, actionId);
		}

		//curActionType = battleBoard->TriggerAction(actionId, true, curActionRenderEffectRoundList);
		singleBattleState = SingleBattleState::ActionInterlude;

		aiTrainPlayerActionCount = 0.0;
	}
	else
	{
		aiTrainPlayerActionCount += dT;
	}
}

void ACoreCardGameModeBase::TrainPlayGameLoop(float dT)
{
	if (singleBattleState == SingleBattleState::Battle)
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
			if (curSectionNb == 0)
			{
				curSectionNb = 1;
			}
			else
			{
				curSectionNb = 0;
			}
			curCountingTick += dT;
		}
	}
	else if (singleBattleState == SingleBattleState::Default)
	{
		if (curCountingTick >= battleStateTicksMap["BeforeSelectCard"])
		{
			singleBattleState = SingleBattleState::SelectCard;
			APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
			ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(playerController);
			coreCardPC->SwitchMenu("SelectCardMenu");
			curCountingTick = 0.0;
		}
		else
		{
			curCountingTick += dT;
		}
	}
	else if (singleBattleState == SingleBattleState::SelectCard)
	{
		if (gamingType == EGamingType::Training)
		{
			if (curCountingTick >= battleStateTicksMap["TrainSelectCardInterval"])
			{
				// switch to select card interlude
				singleBattleState = SingleBattleState::AfterSelectCardInterlude;
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(playerController);
				coreCardPC->SwitchMenu("BattleMenu");
				battleBoard->InitBattleBoard();
				curCountingTick = 0.0;
			}
			else
			{
				curCountingTick += dT;
			}
		}
		else
		{
			if (curCountingTick >= battleStateTicksMap["MaxSelectCardInterval"])
			{
				// switch to select card interlude
				singleBattleState = SingleBattleState::AfterSelectCardInterlude;
				APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(playerController);
				coreCardPC->SwitchMenu("BattleMenu");
				battleBoard->InitBattleBoard();
				curCountingTick = 0.0;
			}
			else
			{
				curCountingTick += dT;
			}
		}
	}
	else if (singleBattleState == SingleBattleState::AfterSelectCardInterlude)
	{
		if (curCountingTick >= battleStateTicksMap["AfterSelectCardInterludeInterval"])
		{
			singleBattleState = SingleBattleState::SelectCardAnimInterlude;
			curCountingTick = 0.0;
		}
		else
		{
			curCountingTick += dT;
		}
	}
	else if (singleBattleState == SingleBattleState::SelectCardAnimInterlude)
	{
		if (curCountingTick >= battleStateTicksMap["SelectCardAnimInterval"])
		{
			singleBattleState = SingleBattleState::Battle;
			curCountingTick = 0.0;
		}
		else
		{
			curCountingTick += dT;
		}
	}
	else if (singleBattleState == SingleBattleState::ActionInterlude)
	{
		float actionInterlude = 0.0;
		if (curActionType == ActionType::PlayCard)
		{
			actionInterlude = battleStateTicksMap["PlayCardInterlude"];
		}
		else if (curActionType == ActionType::LaunchSkill)
		{
			actionInterlude = battleStateTicksMap["LaunchSkillInterlude"];
		}
		else if (curActionType == ActionType::Move)
		{
			actionInterlude = battleStateTicksMap["MoveInterlude"];
		}
		else if (curActionType == ActionType::EndRound)
		{
			actionInterlude = battleStateTicksMap["EndRoundInterlude"];
		}

		if (curCountingTick >= actionInterlude)
		{
			singleBattleState = SingleBattleState::RenderEffectInterlude;
			curCountingTick = 0.0;
			TriggerRenderEffect();
		}
		else
		{
			curCountingTick += dT;
		}
	}
	else if (singleBattleState == SingleBattleState::RenderEffectInterlude)
	{
		float curRenderEffectInterval = 0.0;
		if (curActionEffectRound < curActionRenderEffectRoundList.Num())
		{
			curRenderEffectInterval = curActionRenderEffectRoundList[curActionEffectRound].renderTime;
		}
		else
		{
			// which means rendering is finished
			singleBattleState = SingleBattleState::Battle;
		}
		
		if (curCountingTick >= curRenderEffectInterval)
		{
			// let's switch to next effect rendering round
			curActionEffectRound += 1;
			curCountingTick = 0.0;
			TriggerRenderEffect();
		}
		else
		{
			curCountingTick += dT;
		}
	}
}

void ACoreCardGameModeBase::TriggerReadCardInfo_Implementation()
{

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


void ACoreCardGameModeBase::ReqLaunchCardSkill(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY)
{
	
}

void ACoreCardGameModeBase::TriggerRenderEffect()
{
	/*
	FRenderEffectRound renderEffectRound = curActionRenderEffectRoundList[curActionEffectRound];
	for (int32 i = 0; i < renderEffectRound.renderEffectList.Num(); i++)
	{
		int32 launchX = renderEffectRound.renderEffectList[i].triggerGridX;
		int32 launchY = renderEffectRound.renderEffectList[i].triggerGridY;
		int32 launchUid = battleBoard->realBoard.boardRows[launchY].colCardInfos[launchX];
		for (int32 j = 0; j < renderEffectRound.renderEffectList[i].modifyUids.Num(); j++)
		{
			TSubclassOf<AActor> particleActorClass = effectParticleActorMap[renderEffectRound.renderEffectList[i].renderEffectType];
			int32 targetX = battleBoard->realBoard.allInstanceCardInfo[renderEffectRound.renderEffectList[i].modifyUids[j]].curCol;
			int32 targetY = battleBoard->realBoard.allInstanceCardInfo[renderEffectRound.renderEffectList[i].modifyUids[j]].curRow;
			int32 targetUid = battleBoard->realBoard.boardRows[targetY].colCardInfos[targetX];
			
			FVector targetLaunchOffset = battleBoard->allCards[targetUid]->GetActorLocation() - battleBoard->allCards[launchUid]->GetActorLocation();
			targetLaunchOffset.Normalize();
			FRotator targetRot = targetLaunchOffset.Rotation();
			AActor* particleActor = GetWorld()->SpawnActor<AActor>(particleActorClass, battleBoard->allCards[launchUid]->GetActorLocation(), targetRot);
		}
	}*/
}

void ACoreCardGameModeBase::SpawnHandCard(FString cardName, uint8 sectionNb, int32 cardUid, int32 inCurHp, int32 inCurDefence, int32 handCardNb)
{
	FVector spawnLoc = FVector(0.0, 0.0, 0.0);
	ACard* card = GetWorld()->SpawnActor<ACard>(cardBPClass, spawnLoc, FRotator::ZeroRotator);
	card->cardId = cardUid;
	int32 playCardCol = handCardNb;
	if (handCardNb >= UGlobalConstFunctionLibrary::maxCol)
	{
		playCardCol -= UGlobalConstFunctionLibrary::maxCol;
	}
	card->gridX = playCardCol;

	int32 playCardRow = handCardNb / UGlobalConstFunctionLibrary::maxCol;
	if (sectionNb == 0)
	{
		card->gridY = UGlobalConstFunctionLibrary::graveCardSectionRow + playCardRow;
	}
	else
	{
		card->gridY = UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow +
			UGlobalConstFunctionLibrary::boardSectionRow +
			playCardRow;
	}

	card->camp = sectionNb;
	card->InitCard(cardName, inCurHp, inCurDefence);

	if (sectionNb == 0)
	{
		sectionZeroHandBattleCards.Add(card);
		allBattleCards.Add(cardUid, card);
	}
	else
	{
		sectionOneHandBattleCards.Add(card);
		allBattleCards.Add(cardUid, card);
	}
}

void ACoreCardGameModeBase::CalculateHoverCardLocations(uint8 campNb, int32 hoverCardNb)
{
	if (campNb == 0 && hoverCardNb >= sectionZeroHandBattleCards.Num())
	{
		return;
	}
	else if (campNb == 1 && hoverCardNb >= sectionOneHandBattleCards.Num())
	{
		return;
	}

	if (campNb == 0)
	{
		// Move cards on the right side of hovered card
		for (int32 i = hoverCardNb; i < sectionZeroHandBattleCards.Num(); i++)
		{
			FVector cardOffset = FVector(hoverMoveCardsHorizontalOffset, 0.0, 0.0);
			if (sectionZeroHandBattleCards[i] == selectPlayCard)
			{
				sectionZeroCardLocations[i] = sectionZeroOriginLocations[i] + FVector(0.0, -hoverCardUpOffset, 0.0) + cardOffset;
			}
			else
			{
				sectionZeroCardLocations[i] = sectionZeroOriginLocations[i] + cardOffset;
			}
		}

		FVector hoverCardOffset = FVector(0.0, -hoverCardUpOffset, 0.0);
		sectionZeroCardLocations[hoverCardNb] = sectionZeroOriginLocations[hoverCardNb] + hoverCardOffset;
	}
	else
	{
		// Move cards on the left side of hovered card
		for (int32 i = hoverCardNb; i >= 0; i--)
		{
			FVector cardOffset = FVector(-hoverMoveCardsHorizontalOffset, 0.0, 0.0);
			if (sectionOneHandBattleCards[i] == selectPlayCard)
			{
				sectionOneCardLocations[i] = sectionOneOriginLocations[i] + FVector(0.0, hoverCardUpOffset, 0.0) + cardOffset;
			}
			else
			{
				sectionOneCardLocations[i] = sectionOneOriginLocations[i] + cardOffset;
			}
		}

		FVector hoverCardOffset = FVector(0.0, hoverCardUpOffset, 0.0);
		sectionOneCardLocations[hoverCardNb] = sectionOneOriginLocations[hoverCardNb] + hoverCardOffset;
	}
}

void ACoreCardGameModeBase::RecoverHoverCardLocations()
{
	for (int32 i = 0; i < sectionZeroHandBattleCards.Num(); i++)
	{
		if (selectPlayCard == sectionZeroHandBattleCards[i])
		{
			sectionZeroCardLocations[i] = sectionZeroOriginLocations[i] + FVector(0.0, -hoverCardUpOffset, 0.0);
		}
		else
		{
			sectionZeroCardLocations[i] = sectionZeroOriginLocations[i];
		}
	}

	for (int32 i = 0; i < sectionOneHandBattleCards.Num(); i++)
	{
		if (selectPlayCard == sectionOneHandBattleCards[i])
		{
			sectionOneCardLocations[i] = sectionOneOriginLocations[i] + FVector(0.0, hoverCardUpOffset, 0.0);
		}
		else
		{
			sectionOneCardLocations[i] = sectionOneOriginLocations[i];
		}
	}
}

void ACoreCardGameModeBase::SetSelectPlayCard(uint8 campNb, ACard* inSelectCard)
{
	int32 selectCardNb = -1;
	if (campNb == 0)
	{
		sectionZeroHandBattleCards.Find(inSelectCard, selectCardNb);
		if (selectCardNb >= sectionZeroHandBattleCards.Num() || selectCardNb == -1)
		{
			return;
		}

		for (int32 i = selectCardNb; i < sectionZeroHandBattleCards.Num(); i++)
		{
			FVector cardOffset = FVector(hoverMoveRightCardsOffset, 0.0, 0.0);
			if (i == selectCardNb)
			{
				sectionZeroCardLocations[i] = sectionZeroOriginLocations[i] + FVector(0.0, -hoverCardUpOffset, 0.0);
			}
			else
			{
				sectionZeroCardLocations[i] = sectionZeroOriginLocations[i] + cardOffset;
			}
		}
	}
	else
	{
		sectionOneHandBattleCards.Find(inSelectCard, selectCardNb);
		if (selectCardNb >= sectionOneHandBattleCards.Num() || selectCardNb == -1)
		{
			return;
		}

		for (int32 i = selectCardNb; i < sectionOneHandBattleCards.Num(); i++)
		{
			FVector cardOffset = FVector(-hoverMoveCardsHorizontalOffset, 0.0, 0.0);
			if (i == selectCardNb)
			{
				sectionOneCardLocations[i] = sectionOneOriginLocations[i] + FVector(0.0, hoverCardUpOffset, 0.0);
			}
			else
			{
				sectionOneCardLocations[i] = sectionOneOriginLocations[i] + cardOffset;
			}
		}
	}
}

void ACoreCardGameModeBase::RecoverSelectPlayCard()
{
	sectionZeroCardLocations = sectionZeroOriginLocations;
	sectionOneCardLocations = sectionOneOriginLocations;
}

void ACoreCardGameModeBase::CalculateCardSpread()
{
	sectionZeroOriginLocations.Empty();
	sectionZeroCardLocations.Empty();
	sectionZeroCardRots.Empty();
	sectionOneOriginLocations.Empty();
	sectionOneCardLocations.Empty();
	sectionOneCardRots.Empty();
	for (int32 i = 0; i < sectionZeroHandBattleCards.Num(); i++)
	{
		float spreadCardRot = 0.0;
		float spreadCardPositionY = 0.0;
		if (sectionZeroHandBattleCards.Num() % 2 == 0)
		{
			int32 halfCardNum = sectionZeroHandBattleCards.Num() / 2;
			if (i <= halfCardNum)
			{
				spreadCardRot = -((float)halfCardNum - (float)i - 0.5) * spreadCardRotInterval;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
			else
			{
				spreadCardRot = ((float)i - (float)halfCardNum + 0.5) * spreadCardRotInterval;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
		}
		else
		{
			int32 halfCardNum = sectionZeroHandBattleCards.Num() / 2;
			if (i <= halfCardNum)
			{
				spreadCardRot = -((float)halfCardNum - (float)i) * spreadCardRotInterval;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
			else
			{
				spreadCardRot = ((float)i - (float)halfCardNum) * spreadCardRotInterval;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
		}
		float spreadCardPositionX = spreadCardRotRadius * FMath::Sin(spreadCardRot * PI / 180.0);

		FVector cardLocation = FVector(spreadCardPositionX, spreadCardPositionY, spreadCardHeight + spreadCardStepHeight * i);
		cardLocation -= sectionZeroSpreadCardOffset;
		FRotator cardRotation = FRotator(0.0, spreadCardRot, 0.0);

		sectionZeroCardRots.Add(cardRotation);
		sectionZeroCardLocations.Add(cardLocation);
		sectionZeroOriginLocations.Add(cardLocation);
	}


	for (int32 i = 0; i < sectionOneHandBattleCards.Num(); i++)
	{
		float spreadCardRot = 0.0;
		float spreadCardPositionY = 0.0;
		if (sectionOneHandBattleCards.Num() % 2 == 0)
		{
			int32 halfCardNum = sectionOneHandBattleCards.Num() / 2;
			if (i <= halfCardNum)
			{
				spreadCardRot = -((float)halfCardNum - (float)i - 0.5) * spreadCardRotInterval + 180.0;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
			else
			{
				spreadCardRot = ((float)i - (float)halfCardNum + 0.5) * spreadCardRotInterval + 180.0;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
		}
		else
		{
			int32 halfCardNum = sectionOneHandBattleCards.Num() / 2;
			if (i <= halfCardNum)
			{
				spreadCardRot = -((float)halfCardNum - (float)i) * spreadCardRotInterval + 180.0;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
			else
			{
				spreadCardRot = ((float)i - (float)halfCardNum) * spreadCardRotInterval + 180.0;
				spreadCardPositionY = -spreadCardRotRadius * FMath::Cos(spreadCardRot * PI / 180.0);
			}
		}
		float spreadCardPositionX = spreadCardRotRadius * FMath::Sin(spreadCardRot * PI / 180.0);

		FVector cardLocation = FVector(spreadCardPositionX, spreadCardPositionY, spreadCardHeight + spreadCardStepHeight * i);
		cardLocation -= sectionOneSpreadCardOffset;
		FRotator cardRotation = FRotator(0.0, spreadCardRot, 0.0);

		sectionOneCardRots.Add(cardRotation);
		sectionOneCardLocations.Add(cardLocation);
		sectionOneOriginLocations.Add(cardLocation);
	}
}

void ACoreCardGameModeBase::MoveRearrangeCards()
{
	for (int32 i = 0; i < sectionZeroHandBattleCards.Num(); i++)
	{
		FVector cardCurLocation = sectionZeroHandBattleCards[i]->GetActorLocation();
		FVector interpLocation = FMath::VInterpTo(cardCurLocation, sectionZeroCardLocations[i], hoverMoveCardInterpDeltaTime, hoverMoveCardInterpSpeed);
		sectionZeroHandBattleCards[i]->SetActorLocation(interpLocation);
	}

	for (int32 i = 0; i < sectionOneHandBattleCards.Num(); i++)
	{
		FVector cardCurLocation = sectionOneHandBattleCards[i]->GetActorLocation();
		FVector interpLocation = FMath::VInterpTo(cardCurLocation, sectionOneCardLocations[i], hoverMoveCardInterpDeltaTime, hoverMoveCardInterpSpeed);
		sectionOneHandBattleCards[i]->SetActorLocation(interpLocation);
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
		boardGrid->gridId = boardGrid->gridY * UGlobalConstFunctionLibrary::maxCol + boardGrid->gridX;
		if (!boardGrids.Contains(boardGrid->gridId) && boardGrid->available)
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

	if (isTrain)
	{
		sectionZeroMctsPlayer = GetWorld()->SpawnActor<AMctsPlayer>(mctsPlayerBPClass,
			FVector::Zero(), FRotator::ZeroRotator);

		sectionZeroMctsPlayer->InitMctsPlayer(0);

		sectionOneMctsPlayer = GetWorld()->SpawnActor<AMctsPlayer>(mctsPlayerBPClass,
			FVector::Zero(), FRotator::ZeroRotator);

		sectionOneMctsPlayer->InitMctsPlayer(1);
	}
	else
	{
		sectionOneMctsPlayer = GetWorld()->SpawnActor<AMctsPlayer>(mctsPlayerBPClass,
			FVector::Zero(), FRotator::ZeroRotator);

		sectionOneMctsPlayer->InitMctsPlayer(1);
	}

	battleBoard = GetWorld()->SpawnActor<ABattleBoard>(battleBoardBPClass,
		FVector::Zero(), FRotator::ZeroRotator);

	mcts = NewObject<UMcts>(this, mctsBPClass);
	mcts->InitMcts(2);
	aiRunnable = new FAIRunnable(this);
	aiRunnable->Start(mcts);
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

