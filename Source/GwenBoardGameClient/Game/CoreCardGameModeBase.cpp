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
	curPlayerOperateLeftTime = playerOperateTime;

	FName testTag = "testMotionCard";
	TArray<AActor*> testTagCards;
	UGameplayStatics::GetAllActorsWithTag(this, testTag, testTagCards);

	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(pc);
	coreCardPC->InitMenu();

	gameModeRenderState = EGameModeRenderState::Default;
	UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
	UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
	if (gwenGI->playerCambNb == curSectionNb)
	{
		UpdateEndRoundButtonState(EEndRoundButtonState::SelfNotPlayCardYet);
	}
	else
	{
		UpdateEndRoundButtonState(EEndRoundButtonState::OppoNotPlayCardYet);
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

	if (gameModeRenderState == EGameModeRenderState::Default)
	{
		if (aiRunnable->aiRunnableState == EAIRunnableState::NewState)
		{
			aiRunnable->aiRunnableState = EAIRunnableState::Default;
			gameModeRenderState = EGameModeRenderState::RenderingStep;
			curRenderingActionNodes.Empty();
			curRenderingActionNodes.Add(aiRunnable->newStateRenderRoot);
		}
		else if (aiRunnable->aiRunnableState == EAIRunnableState::SelfPlayEnd)
		{
			APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
			ACoreCardGamePC* coreCardPC = Cast<ACoreCardGamePC>(pc);
			coreCardPC->mctReplayMenu->RefreshMctsMenu();
			aiRunnable->aiRunnableState = EAIRunnableState::Default;
		}
		else
		{
			OperateCountDown(deltaTime);
			UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
			UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
			if (gwenGI->playerCambNb != curSectionNb)
			{
				if (!isTrain)
				{
					if (aiRunnable->aiRunnableState == EAIRunnableState::Default)
					{
						AIMovementCountDown(deltaTime);
					}
					/*
					else if (aiRunnable->aiRunnableState == EAIRunnableState::WaitTritonResponse)
					{
						if (curAIWaitResponseTime >= aiWaitResponseTime)
						{
							aiRunnable->TriggerSuplementRequest();
							curAIWaitResponseTime = 0.0;
						}
						else
						{
							curAIWaitResponseTime += deltaTime;
						}
					}
					*/
				}
			}
		}
	}
	else if (gameModeRenderState == EGameModeRenderState::ActionTimeOut)
	{
		// if card is not played, trigger playing card action
		if (curSectionNb == 0)
		{
			if (aiRunnable->mcts->realBoard.sectionZeroPlayCardAvailable)
			{
				// trigger playing card action
				TimeoutTriggerPlayCardAction(0);
			}
			else
			{
				// trigger end round animation
				gameModeRenderState = EGameModeRenderState::EndRoundRender;
				curPlayerOperateLeftTime = playerOperateTime;
			}
		}
		else
		{
			if (aiRunnable->mcts->realBoard.sectionOnePlayCardAvailable)
			{
				TimeoutTriggerPlayCardAction(1);
			}
			else
			{
				// trigger end round animation
				gameModeRenderState = EGameModeRenderState::EndRoundRender;
				curPlayerOperateLeftTime = playerOperateTime;
			}
		}
	}
	else if (gameModeRenderState == EGameModeRenderState::ActionTimeOutWaitRender)
	{
		if (aiRunnable->aiRunnableState == EAIRunnableState::NewState)
		{
			aiRunnable->aiRunnableState = EAIRunnableState::Default;
			gameModeRenderState = EGameModeRenderState::ActionTimeOutRenderStep;
			curRenderingActionNodes.Empty();
			curRenderingActionNodes.Add(aiRunnable->newStateRenderRoot);
		}
	}
	else if (gameModeRenderState == EGameModeRenderState::EndRoundRender)
	{
		// update end round button state
		// demonstrate end round words
		if (curSectionNb == 0)
		{
			curSectionNb = 1;
		}
		else
		{
			curSectionNb = 0;
		}
		curRenderEndRoundTime = 0.0;
		gameModeRenderState = EGameModeRenderState::RenderingEndRound;
	}
	else if (gameModeRenderState == EGameModeRenderState::RenderingStep ||
		gameModeRenderState == EGameModeRenderState::ActionTimeOutRenderStep)
	{
		if (curRenderingActionNodes.Num() > 0)
		{
			TArray<FRenderActionNode> triggerRenderNodes;
			TArray<int32> finishedRenderNodeNbs;
			for (int32 i = 0; i < curRenderingActionNodes.Num(); i++)
			{
				if (!curRenderingActionNodes[i].triggerRender)
				{
					triggerRenderNodes.Add(curRenderingActionNodes[i]);
					curRenderingActionNodes[i].triggerRender = true;
				}
				else
				{
					if (curRenderingActionNodes[i].curRenderTime >= curRenderingActionNodes[i].renderTime)
					{
						// which means action node rendering is finished
						finishedRenderNodeNbs.Add(i);
						if (curRenderingActionNodes[i].actionType == ActionType::PlayCard)
						{
							// which means card has been played, show up end round button
							UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
							UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
							if (curSectionNb == gwenGI->playerCambNb)
							{
								if (gameModeRenderState == EGameModeRenderState::RenderingStep)
								{
									UpdateEndRoundButtonState(EEndRoundButtonState::SelfHasPlayCard);
								}
								else
								{
									UpdateEndRoundButtonState(EEndRoundButtonState::OppoNotPlayCardYet);
								}
							}
							else
							{
								if (gameModeRenderState == EGameModeRenderState::RenderingStep)
								{
									UpdateEndRoundButtonState(EEndRoundButtonState::OppoHasPlayCard);
								}
								else
								{
									UpdateEndRoundButtonState(EEndRoundButtonState::OppoHasPlayCard);
								}
							}
						}
					}
					else
					{
						curRenderingActionNodes[i].curRenderTime += deltaTime;
					}
				}
			}

			if (finishedRenderNodeNbs.Num() == curRenderingActionNodes.Num())
			{
				// finish all current rendering nodes
				for (int32 i = 0; i < curRenderingActionNodes.Num(); i++)
				{
					for (int32 j = 0; j < curRenderingActionNodes[i].children.Num(); j++)
					{
						triggerRenderNodes.Add(curRenderingActionNodes[i].children[j]);
					}
				}

				curRenderingActionNodes.Empty();
				for (int32 i = 0; i < triggerRenderNodes.Num(); i++)
				{
					triggerRenderNodes[i].triggerRender = true;
				}
				curRenderingActionNodes = triggerRenderNodes;
				if (curRenderingActionNodes.Num() == 0)
				{
					if (gameModeRenderState == EGameModeRenderState::RenderingStep)
					{
						gameModeRenderState = EGameModeRenderState::Default;
					}
					else
					{
						gameModeRenderState = EGameModeRenderState::EndRoundRender;
						curPlayerOperateLeftTime = playerOperateTime;
					}
				}
			}


			for (int32 i = 0; i < triggerRenderNodes.Num(); i++)
			{
				if (triggerRenderNodes[i].actionType == ActionType::LaunchSkill)
				{
					if (triggerRenderNodes[i].renderEffectType == "hurt")
					{
						// play hurt card effect
						int32 boardGridId = (triggerRenderNodes[i].triggerGridY -
							UGlobalConstFunctionLibrary::graveCardSectionRow -
							UGlobalConstFunctionLibrary::playCardSectionRow) *
							UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].triggerGridX;

						FVector triggerPt = FVector(
							boardGrids[boardGridId]->GetActorLocation().X,
							boardGrids[boardGridId]->GetActorLocation().Y,
							boardGrids[boardGridId]->GetActorLocation().Z + gridGuidePSOffset);

						TArray<FVector> targetPts;
						for (int32 j = 0; j < triggerRenderNodes[i].targetGridXs.Num(); j++)
						{
							int32 targetBoardGridId = (triggerRenderNodes[i].targetGridYs[j] -
								UGlobalConstFunctionLibrary::graveCardSectionRow -
								UGlobalConstFunctionLibrary::playCardSectionRow) *
								UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].targetGridXs[j];

							FVector targetPt = FVector(
								boardGrids[targetBoardGridId]->GetActorLocation().X,
								boardGrids[targetBoardGridId]->GetActorLocation().Y,
								boardGrids[targetBoardGridId]->GetActorLocation().Z + gridGuidePSOffset);

							targetPts.Add(targetPt);
						}
						TriggerSkillHurtRender(triggerPt, targetPts);

						for (int32 j = 0; j < triggerRenderNodes[i].modifyUids.Num(); j++)
						{
							if (allBattleCards[triggerRenderNodes[i].modifyUids[j]]->defence >= triggerRenderNodes[i].modifyValues[j])
							{
								int32 curDefence = allBattleCards[triggerRenderNodes[i].modifyUids[j]]->defence - triggerRenderNodes[i].modifyValues[j];
								int32 curHp = allBattleCards[triggerRenderNodes[i].modifyUids[j]]->hp;
								int32 curCd = aiRunnable->mcts->realBoard.allInstanceCardInfo[triggerRenderNodes[i].modifyUids[j]].curCoolDown;
								int32 curAvailableTimes = aiRunnable->mcts->realBoard.allInstanceCardInfo[triggerRenderNodes[i].modifyUids[j]].curAvailableTimes;
								allBattleCards[triggerRenderNodes[i].modifyUids[j]]->UpdateCard(curHp, curDefence, curCd, curAvailableTimes);
							}
							else
							{
								int32 curDefence = 0;
								int32 curHp = allBattleCards[triggerRenderNodes[i].modifyUids[j]]->hp -
									(triggerRenderNodes[i].modifyValues[j] - allBattleCards[triggerRenderNodes[i].modifyUids[j]]->defence);
								int32 curCd = aiRunnable->mcts->realBoard.allInstanceCardInfo[triggerRenderNodes[i].modifyUids[j]].curCoolDown;
								int32 curAvailableTimes = aiRunnable->mcts->realBoard.allInstanceCardInfo[triggerRenderNodes[i].modifyUids[j]].curAvailableTimes;
								allBattleCards[triggerRenderNodes[i].modifyUids[j]]->UpdateCard(curHp, curDefence, curCd, curAvailableTimes);
							}
						}
					}
					else if (triggerRenderNodes[i].renderEffectType == "move")
					{
						// play effect moving card vfx

						
					}
					else if (triggerRenderNodes[i].renderEffectType == "spawn")
					{
						// play spawn card vfx
					}
					else if (triggerRenderNodes[i].renderEffectType == "draw")
					{
						// play draw card vfx
					}
					else if (triggerRenderNodes[i].renderEffectType == "switchOppoPos")
					{
						if (triggerRenderNodes[i].modifyUids[0] == -1)
						{
							int32 boardGridId = (triggerRenderNodes[i].targetGridYs[0] -
								UGlobalConstFunctionLibrary::graveCardSectionRow -
								UGlobalConstFunctionLibrary::playCardSectionRow) *
								UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].targetGridXs[0];
							FVector targetGridLoc = FVector(
								boardGrids[boardGridId]->GetActorLocation().X,
								boardGrids[boardGridId]->GetActorLocation().Y,
								boardGrids[boardGridId]->GetActorLocation().Z) + gridCardVerticalOffset;

							FVector pos_1 = allBattleCards[triggerRenderNodes[i].modifyUids[1]]->GetActorLocation();
							
							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->SetActorLocation(targetGridLoc);
							int32 gridX_0 = triggerRenderNodes[i].targetGridXs[0];
							int32 gridY_0 = triggerRenderNodes[i].targetGridYs[0];

							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridX = gridX_0;
							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridY = gridY_0;
						}
						else if (triggerRenderNodes[i].modifyUids[1] == -1)
						{
							int32 boardGridId = (triggerRenderNodes[i].targetGridYs[1] -
								UGlobalConstFunctionLibrary::graveCardSectionRow -
								UGlobalConstFunctionLibrary::playCardSectionRow) *
								UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].targetGridXs[1];
							FVector targetGridLoc = FVector(
								boardGrids[boardGridId]->GetActorLocation().X,
								boardGrids[boardGridId]->GetActorLocation().Y,
								boardGrids[boardGridId]->GetActorLocation().Z) + gridCardVerticalOffset;

							FVector pos_0 = allBattleCards[triggerRenderNodes[i].modifyUids[0]]->GetActorLocation();

							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->SetActorLocation(targetGridLoc);
							int32 gridX_1 = triggerRenderNodes[i].targetGridXs[1];
							int32 gridY_1 = triggerRenderNodes[i].targetGridYs[1];

							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridX = gridX_1;
							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridY = gridY_1;
						}
						else
						{
							FVector pos_0 = allBattleCards[triggerRenderNodes[i].modifyUids[0]]->GetActorLocation();
							FVector pos_1 = allBattleCards[triggerRenderNodes[i].modifyUids[1]]->GetActorLocation();
							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->SetActorLocation(pos_1);
							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->SetActorLocation(pos_0);
							int32 gridX_0 = allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridX;
							int32 gridY_0 = allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridY;
							int32 gridX_1 = allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridX;
							int32 gridY_1 = allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridY;
							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridX = gridX_1;
							allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridY = gridY_1;
							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridX = gridX_0;
							allBattleCards[triggerRenderNodes[i].modifyUids[1]]->gridY = gridY_0;
						}
					}
					else if (triggerRenderNodes[i].renderEffectType == "pull")
					{
						FVector triggerLoc = allBattleCards[triggerRenderNodes[i].modifyUids[0]]->GetActorLocation();
						int32 boardGridId = (triggerRenderNodes[i].toGridYs[0] -
							UGlobalConstFunctionLibrary::graveCardSectionRow -
							UGlobalConstFunctionLibrary::playCardSectionRow) *
							UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].toGridXs[0];
						FVector targetGridLoc = FVector(
							boardGrids[boardGridId]->GetActorLocation().X,
							boardGrids[boardGridId]->GetActorLocation().Y,
							boardGrids[boardGridId]->GetActorLocation().Z) + gridCardVerticalOffset;
						allBattleCards[triggerRenderNodes[i].modifyUids[0]]->SetActorLocation(targetGridLoc);
						allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridX = triggerRenderNodes[i].toGridXs[0];
						allBattleCards[triggerRenderNodes[i].modifyUids[0]]->gridY = triggerRenderNodes[i].toGridYs[0];
					}
				}
				else if (triggerRenderNodes[i].actionType == ActionType::Move)
				{
					int32 targetGridId = (triggerRenderNodes[i].targetGridYs.Last() -
						UGlobalConstFunctionLibrary::graveCardSectionRow - UGlobalConstFunctionLibrary::playCardSectionRow) *
						UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].targetGridXs.Last();

					FVector2D target = FVector2D(boardGrids[targetGridId]->GetActorLocation().X,
						boardGrids[targetGridId]->GetActorLocation().Y);

					int32 launchCardId = triggerRenderNodes[i].triggerCardId;
					allBattleCards[launchCardId]->TriggerCardMotion(target);

					allBattleCards[launchCardId]->gridX = triggerRenderNodes[i].targetGridXs.Last();
					allBattleCards[launchCardId]->gridY = triggerRenderNodes[i].targetGridYs.Last();

					selectBoardCard = NULL;
				}
				else if (triggerRenderNodes[i].actionType == ActionType::PlayCard)
				{
					int32 triggerCardId = triggerRenderNodes[i].triggerCardId;

					if (allBattleCards[triggerCardId]->cardStatus != BattleCardStatus::InBattle)
					{
						allBattleCards[triggerCardId]->cardStatus = BattleCardStatus::InBattle;
					}

					allBattleCards[triggerCardId]->gridX = triggerRenderNodes[i].targetGridXs.Last();
					allBattleCards[triggerCardId]->gridY = triggerRenderNodes[i].targetGridYs.Last();


					int32 targetGridId = (triggerRenderNodes[i].targetGridYs.Last() -
						UGlobalConstFunctionLibrary::graveCardSectionRow - UGlobalConstFunctionLibrary::playCardSectionRow) *
						UGlobalConstFunctionLibrary::maxCol + triggerRenderNodes[i].targetGridXs.Last();

					FVector2D target = FVector2D(boardGrids[targetGridId]->GetActorLocation().X,
						boardGrids[targetGridId]->GetActorLocation().Y);

					// play card distributed animation
					allBattleCards[triggerCardId]->TriggerCardMotion(target);
					if (allBattleCards[triggerCardId]->camp == 0)
					{
						for (int32 j = 0; j < sectionZeroHandBattleCards.Num(); j++)
						{
							if (sectionZeroHandBattleCards[j] == allBattleCards[triggerCardId])
							{
								sectionZeroHandBattleCards.RemoveAt(j);
								break;
							}
						}
					}
					else
					{
						for (int32 j = 0; j < sectionOneHandBattleCards.Num(); j++)
						{
							if (sectionOneHandBattleCards[j] == allBattleCards[triggerCardId])
							{
								sectionOneHandBattleCards.RemoveAt(j);
								break;
							}
						}
					}
				}
				else if (triggerRenderNodes[i].actionType == ActionType::EndRound)
				{
					gameModeRenderState = EGameModeRenderState::EndRoundRender;
					curPlayerOperateLeftTime = playerOperateTime;
				}
			}
		}
	}
	else if (gameModeRenderState == EGameModeRenderState::RenderingEndRound)
	{
		if (curRenderEndRoundTime >= endRoundRenderInterval)
		{
			gameModeRenderState = EGameModeRenderState::Default;
			UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
			UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
			if (gwenGI->playerCambNb == curSectionNb)
			{
				UpdateEndRoundButtonState(EEndRoundButtonState::SelfNotPlayCardYet);
			}
			else
			{
				UpdateEndRoundButtonState(EEndRoundButtonState::OppoNotPlayCardYet);
			}
		}
		else
		{
			curRenderEndRoundTime += deltaTime;
		}
	}
}

void ACoreCardGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (aiRunnable)
	{
		aiRunnable->Stop();
	}
}

void ACoreCardGameModeBase::OperateCountDown(float dT)
{
	if (curPlayerOperateLeftTime > 0.0)
	{
		// update count down bar
		curPlayerOperateLeftTime -= dT;
		APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
		ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(pc);
		float progress = 1.0 - curPlayerOperateLeftTime / playerOperateTime;
		coreCardGamePC->UpdateActionCountDownBar(progress);
	}
	else
	{
		// force to stop current player action
		gameModeRenderState = EGameModeRenderState::ActionTimeOut;
	}
}

void ACoreCardGameModeBase::AIMovementCountDown(float dT)
{
	
	if (curAIMovementWaitTime >= aiMovementWaitTime)
	{
		curAIMovementWaitTime = 0.0;
		UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
		UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
		if (gwenGI->playerCambNb == 0)
		{
			TriggerAIAskAction(1);
		}
		else
		{
			TriggerAIAskAction(0);
		}
	}
	else
	{
		curAIMovementWaitTime += dT;
	}
	
}

void ACoreCardGameModeBase::TriggerAIAskAction(uint8 inTriggerSection)
{
	aiRunnable->TriggerAIAskAction(inTriggerSection);
}





void ACoreCardGameModeBase::FinishCardSelection()
{
	// Trigger by Select Card widget, next operation depends on single or multi players game mode
}

void ACoreCardGameModeBase::TestTriggerSimulation()
{
	aiRunnable->TriggerTestGetAction();
}

void ACoreCardGameModeBase::TestTriggerTritonInference()
{
	aiRunnable->TriggerTestTritonInference();
}

void ACoreCardGameModeBase::TestTriggerAction(uint8 campNb, int32 launchX, int32 launchY, int32 targetX, int32 targetY, ActionType actionType)
{
	aiRunnable->TriggerAssignAction(campNb, launchX, launchY, targetX, targetY, actionType);
}

void ACoreCardGameModeBase::TryTriggerEndRound()
{
	UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
	UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
	if (gwenGI->playerCambNb == 0)
	{
		if (aiRunnable->mcts->realBoard.curPlayingSectionNb == 0)
		{
			if (aiRunnable->mcts->realBoard.sectionZeroPlayCardAvailable)
			{
				// no, you can not end up

			}
			else
			{

			}
		}
		else
		{
			// you can not press it
		}
	}
	else
	{
		if (aiRunnable->mcts->realBoard.curLaunchPlaySectionNb == 0)
		{
			// you can not press it
		}
		else
		{
			if (aiRunnable->mcts->realBoard.sectionOnePlayCardAvailable)
			{
				// no, you can not end up

			}
			else
			{

			}
		}
	}
}

void ACoreCardGameModeBase::TimeoutTriggerPlayCardAction(uint8 campNb)
{
	if (campNb == 0)
	{
		int32 launchX = -1;
		int32 launchY = -1;
		// get first available playing card
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow; 
			i < UGlobalConstFunctionLibrary::graveCardSectionRow + 
			UGlobalConstFunctionLibrary::playCardSectionRow; 
			i++)
		{
			for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
			{
				if (aiRunnable->mcts->realBoard.boardRows[i].colCardInfos[j] != -1)
				{
					launchX = j;
					launchY = i;
					break;
				}
			}

			if (launchX != -1)
			{
				break;
			}
		}


		if (launchX != -1)
		{
			int32 targetX = 0;
			int32 targetY = 0;
			// find random available target grid
			for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow;
				i < UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow / 2;
				i++)
			{
				for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
				{
					if (aiRunnable->mcts->realBoard.boardRows[i].colCardInfos[j] == -1)
					{
						targetX = j;
						targetY = i;
						break;
					}
				}

				if (targetX != -1)
				{
					// trigger playing card
					aiRunnable->TriggerAssignAction(0, launchX, launchY, targetX, targetY, ActionType::PlayCard);
					break;
				}
			}
		}
	}
	else
	{
		int32 launchX = -1;
		int32 launchY = -1;
		// get first available playing card
		for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow + 
			UGlobalConstFunctionLibrary::playCardSectionRow + 
			UGlobalConstFunctionLibrary::boardSectionRow;
			i < UGlobalConstFunctionLibrary::graveCardSectionRow +
			UGlobalConstFunctionLibrary::playCardSectionRow*2 + 
			UGlobalConstFunctionLibrary::boardSectionRow;
			i++)
		{
			for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
			{
				if (aiRunnable->mcts->realBoard.boardRows[i].colCardInfos[j] != -1)
				{
					launchX = j;
					launchY = i;
					break;
				}
			}

			if (launchX != -1)
			{
				break;
			}
		}


		if (launchX != -1)
		{
			int32 targetX = 0;
			int32 targetY = 0;
			// find random available target grid
			for (int32 i = UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow / 2; i < UGlobalConstFunctionLibrary::graveCardSectionRow +
				UGlobalConstFunctionLibrary::playCardSectionRow +
				UGlobalConstFunctionLibrary::boardSectionRow;
				i++)
			{
				for (int32 j = 0; j < UGlobalConstFunctionLibrary::maxCol; j++)
				{
					if (aiRunnable->mcts->realBoard.boardRows[i].colCardInfos[j] == -1)
					{
						targetX = j;
						targetY = i;
						break;
					}
				}

				if (targetX != -1)
				{
					aiRunnable->TriggerAssignAction(1, launchX, launchY, targetX, targetY, ActionType::PlayCard);
					break;
				}
			}
		}
		
	}

	gameModeRenderState = EGameModeRenderState::ActionTimeOutWaitRender;
}

void ACoreCardGameModeBase::UpdateEndRoundButtonState(EEndRoundButtonState state)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(pc);
	coreCardGamePC->UpdateEndRoundButtonState(state);
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
							node->allReplayInstanceCardInfo[cardId].camp,
							node->allReplayInstanceCardInfo[cardId].curHp,
							node->allReplayInstanceCardInfo[cardId].curDefence,
							node->allReplayInstanceCardInfo[cardId].curCoolDown,
							node->allReplayInstanceCardInfo[cardId].curAvailableTimes);
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
							node->allReplayInstanceCardInfo[cardId].camp,
							node->allReplayInstanceCardInfo[cardId].curHp,
							node->allReplayInstanceCardInfo[cardId].curDefence,
							node->allReplayInstanceCardInfo[cardId].curCoolDown,
							node->allReplayInstanceCardInfo[cardId].curAvailableTimes);
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
			int32 launchCardUid = node->replayBoardRows[launchY].colCardInfos[launchX];
			int32 targetCardUid = node->replayBoardRows[targetY].colCardInfos[targetX];
			if (targetCardUid == -1)
			{
				if (launchCardUid == -1)
				{
					int32 launchGridY = launchY - UGlobalConstFunctionLibrary::graveCardSectionRow -
						UGlobalConstFunctionLibrary::playCardSectionRow;
					int32 launchGridId = launchGridY * UGlobalConstFunctionLibrary::maxCol + launchX;

					int32 targetGridY = targetY - UGlobalConstFunctionLibrary::graveCardSectionRow -
						UGlobalConstFunctionLibrary::playCardSectionRow;
					int32 targetGridId = targetGridY * UGlobalConstFunctionLibrary::maxCol + targetX;

					FVector launchGridLoc = boardGrids[launchGridId]->GetActorLocation() + FVector(0.0, 5.0, 0.0);
					FVector targetGridLoc = boardGrids[targetGridId]->GetActorLocation();
					GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, "Target is killed");
					UKismetSystemLibrary::DrawDebugLine(this, launchGridLoc, targetGridLoc, FLinearColor::Yellow, 1.0, 10.0);
					UKismetSystemLibrary::DrawDebugPoint(this, launchGridLoc, 10.0, FLinearColor::Red, 1.0);
					UKismetSystemLibrary::DrawDebugPoint(this, targetGridLoc, 10.0, FLinearColor::Blue, 1.0);
				}
				else
				{
					int32 targetGridY = targetY - UGlobalConstFunctionLibrary::graveCardSectionRow -
						UGlobalConstFunctionLibrary::playCardSectionRow;
					int32 targetGridId = targetGridY * UGlobalConstFunctionLibrary::maxCol + targetX;
					FVector launchCardLoc = allReplayCards[launchCardUid]->GetActorLocation();
					FVector targetGridLoc = boardGrids[targetGridId]->GetActorLocation();
					GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "Target is killed");
					UKismetSystemLibrary::DrawDebugLine(this, launchCardLoc, targetGridLoc, FLinearColor::Red, 1.0, 10.0);
					UKismetSystemLibrary::DrawDebugPoint(this, launchCardLoc, 10.0, FLinearColor::Red, 1.0);
					UKismetSystemLibrary::DrawDebugPoint(this, targetGridLoc, 10.0, FLinearColor::Blue, 1.0);
				}
			}
			else
			{
				if (launchCardUid == -1)
				{
					int32 launchGridY = launchY - UGlobalConstFunctionLibrary::graveCardSectionRow -
						UGlobalConstFunctionLibrary::playCardSectionRow;
					int32 launchGridId = launchGridY * UGlobalConstFunctionLibrary::maxCol + launchX;

					int32 targetGridY = targetY - UGlobalConstFunctionLibrary::graveCardSectionRow -
						UGlobalConstFunctionLibrary::playCardSectionRow;
					int32 targetGridId = targetGridY * UGlobalConstFunctionLibrary::maxCol + targetX;

					FVector launchGridLoc = boardGrids[launchGridId]->GetActorLocation() + FVector(0.0, 5.0, 0.0);
					FVector targetCardLoc = allReplayCards[targetCardUid]->GetActorLocation();
					GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, "Target is killed");
					UKismetSystemLibrary::DrawDebugLine(this, launchGridLoc, targetCardLoc, FLinearColor::Yellow, 1.0, 10.0);
					UKismetSystemLibrary::DrawDebugPoint(this, launchGridLoc, 10.0, FLinearColor::Red, 1.0);
					UKismetSystemLibrary::DrawDebugPoint(this, targetCardLoc, 10.0, FLinearColor::Blue, 1.0);
				}
				else
				{
					FVector launchCardLoc = allReplayCards[launchCardUid]->GetActorLocation();
					FVector targetCardLoc = allReplayCards[targetCardUid]->GetActorLocation();
					UKismetSystemLibrary::DrawDebugLine(this, launchCardLoc, targetCardLoc, FLinearColor::Green, 1.0, 10.0);
					UKismetSystemLibrary::DrawDebugPoint(this, launchCardLoc, 10.0, FLinearColor::Red, 1.0);
					UKismetSystemLibrary::DrawDebugPoint(this, targetCardLoc, 10.0, FLinearColor::Blue, 1.0);
				}
			}
		}
		else if (actionType == ActionType::Move)
		{
			int32 launchGridY = launchY - UGlobalConstFunctionLibrary::graveCardSectionRow -
				UGlobalConstFunctionLibrary::playCardSectionRow;
			int32 launchGridId = launchGridY * UGlobalConstFunctionLibrary::maxCol + launchX;
			FVector launchGridLoc = boardGrids[launchGridId]->GetActorLocation();
			FVector launchGridOffsetLoc = launchGridLoc + gridCardVerticalOffset;

			int32 targetGridY = targetY - UGlobalConstFunctionLibrary::graveCardSectionRow -
				UGlobalConstFunctionLibrary::playCardSectionRow;
			int32 targetGridId = targetGridY * UGlobalConstFunctionLibrary::maxCol + targetX;
			FVector targetGridLoc = boardGrids[targetGridId]->GetActorLocation();
			FVector targetGridOffsetLoc = targetGridLoc + gridCardVerticalOffset;

			UKismetSystemLibrary::DrawDebugLine(this, launchGridOffsetLoc, targetGridOffsetLoc, FLinearColor::Blue, 1.0, 10.0);
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

void ACoreCardGameModeBase::TriggerSkillHurtRender(FVector startPt, const TArray<FVector>& targetPts)
{
	for (int32 i = 0; i < targetPts.Num(); i++)
	{
		APSGuideActor* psActor = GetWorld()->SpawnActor<APSGuideActor>(cardConnectPSBPClass, startPt, FRotator::ZeroRotator);
		psActor->Init(startPt, targetPts[i]);
		FRotator slashRot = FRotator(0.0, 180.0, 0.0);
		ASlashPSActor* slashPSActor = GetWorld()->SpawnActor<ASlashPSActor>(redSlashNiagaraBPClass, targetPts[i], slashRot);
	}
	
}

void ACoreCardGameModeBase::DeleteHandAllCards()
{
	for (TMap<int32, ACard*>::TConstIterator iter = allBattleCards.CreateConstIterator(); iter; ++iter)
	{
		iter->Value->ConditionalBeginDestroy();
	}
	allBattleCards.Empty();

	for (int32 i = 0; i < sectionZeroHandBattleCards.Num(); i++)
	{
		if (sectionZeroHandBattleCards[i]->IsValidLowLevel())
		{
			sectionZeroHandBattleCards[i]->ConditionalBeginDestroy();
		}
	}
	sectionZeroHandBattleCards.Empty();

	for (int32 i = 0; i < sectionOneHandBattleCards.Num(); i++)
	{
		if (sectionOneHandBattleCards[i]->IsValidLowLevel())
		{
			sectionOneHandBattleCards[i]->ConditionalBeginDestroy();
		}
	}
	sectionOneHandBattleCards.Empty();

	for (TMap<int32, AReplayCard*>::TConstIterator iter = allReplayCards.CreateConstIterator(); iter; ++iter)
	{
		iter->Value->ConditionalBeginDestroy();
	}
	allReplayCards.Empty();

	sectionZeroCardRots.Empty();
	sectionOneCardRots.Empty();
	sectionZeroOriginLocations.Empty();
	sectionOneOriginLocations.Empty();
	sectionZeroCardLocations.Empty();
	sectionOneCardLocations.Empty();
}

void ACoreCardGameModeBase::SpawnHandCard(FString cardName, uint8 sectionNb, int32 cardUid, int32 inCurHp, int32 inCurDefence, int32 inCurCd, int32 inCurAvailable, int32 handCardNb)
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
	card->InitCard(cardName, inCurHp, inCurDefence, inCurCd, inCurAvailable);

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

	mcts = NewObject<UMcts>(GetWorld(), mctsBPClass);
	//mcts = NewObject<UMcts>(this, mctsBPClass);
	mcts->InitMcts();
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


