// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/AIRunnable.h"

FAIRunnable::FAIRunnable(ACoreCardGameModeBase* inOwner)
{
	threadOwner = inOwner;
	
}

FAIRunnable::~FAIRunnable()
{
	if (aiRunnableThread)
	{
		aiRunnableThread->Kill(true);
		delete aiRunnableThread;
	}
	
}

void FAIRunnable::Start(TObjectPtr<UMcts> inMcts)
{
	check(FPlatformProcess::SupportsMultithreading());
	aiRunnableThread = FRunnableThread::Create(this, *FString::Printf(TEXT("FAIRunnable")));
	mcts = inMcts;
}

bool FAIRunnable::Init()
{
	running = true;
	return true;
}

void FAIRunnable::Stop()
{
	running = false;
	if (aiRunnableThread)
	{
		aiRunnableThread->Kill(true);
		delete aiRunnableThread;
	}
}

void FAIRunnable::Exit()
{

}

uint32 FAIRunnable::Run()
{
	while (running)
	{
		if (aiRunnableState == EAIRunnableState::Working)
		{
			int32 actionCode = mcts->realBoard.ActionCoding(
				waitLaunchX, 
				waitLaunchY, 
				waitTargetX, 
				waitTargetY, 
				waitActionType);
			TArray<FRenderEffectRound> renderEffectRounds;
			mcts->realBoard.TriggerAction(true, waitLaunchCamp, actionCode, renderEffectRounds);
			aiRunnableState = EAIRunnableState::NewState;
		}
		else if (aiRunnableState == EAIRunnableState::StartSelfPlay)
		{
			mcts->curSelfPlayLoop = 0;
			//mcts->trainingDatas.Empty();
			mcts->treeRoot->curPlayingSectionNb = mcts->realBoard.curPlayingSectionNb;
			FTrainingData newTrainingData;
			mcts->curTrainingData = newTrainingData;
			aiRunnableState = EAIRunnableState::SelfPlayLooping;
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayLooping)
		{
			if (mcts->curSelfPlayLoop >= mcts->maxSelfPlayLoop)
			{
				mcts->curSimulationMove = 0;
				aiRunnableState = EAIRunnableState::SelfPlayEnd;
				//mcts->SaveTrainingData(mcts->trainingDatas, StateCodingC, StateCodingH, StateCodingW);
			}
			else
			{
				//mcts->ResetMcts();
				aiRunnableState = EAIRunnableState::SelfPlaySendTritonRequest;
			}
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlaySendTritonRequest)
		{
			if (mcts->curSimulationMove >= mcts->expandSimulationMoves)
			{
				//FScopeLock lock(&criticalSection);
				int32 targetAction;
				ActionType targetActionType;
				FTrainDataStateCodingAndActionProbs trainStateCodingAndActionProbs;
				uint8 targetSectionNb;
				mcts->GetTritonAction(targetAction, targetActionType, targetSectionNb, trainStateCodingAndActionProbs.actionProbs);

				int32 trainingDataStateCoding[StateCodingTotalCHW] = { 0 };
				mcts->realBoard.StateCoding(mcts->realBoard.curPlayingSectionNb, trainingDataStateCoding);
				FMemory::Memcpy(trainStateCodingAndActionProbs.stateCoding, trainingDataStateCoding, StateCodingTotalCHW * sizeof(int32));

				mcts->curTrainingData.stateCodingAndActionProbs.Add(trainStateCodingAndActionProbs);
				mcts->curTrainingData.playSectionNbs.Add(targetSectionNb);

				TArray<FRenderEffectRound> renderEffectList;
				mcts->realBoard.TriggerAction(false, mcts->realBoard.curPlayingSectionNb, targetAction, renderEffectList);

				int32 winner;
				bool isGameEnd = mcts->realBoard.GameEnd(winner);

				if (isGameEnd)
				{
					if (winner == -1)
					{
						// which means it's draw
						for (int32 i = 0; i < mcts->curTrainingData.playSectionNbs.Num(); i++)
						{
							mcts->curTrainingData.scores.Add(0.0);
						}
					}
					else
					{
						for (int32 i = 0; i < mcts->curTrainingData.playSectionNbs.Num(); i++)
						{
							if (mcts->curTrainingData.playSectionNbs[i] == winner)
							{
								mcts->curTrainingData.scores.Add(1.0);
							}
							else
							{
								mcts->curTrainingData.scores.Add(-1.0);
							}
						}
					}

					mcts->veryFirstNode->UpdateWinLoseResult(winner);
					
					mcts->curSimulationMove = 0;
					aiRunnableState = EAIRunnableState::SelfPlayLoopEnd;
					//mcts->trainingDatas.Add(mcts->curTrainingData);
				}
				else
				{
					mcts->curSimulationMove = 0;
					aiRunnableState = EAIRunnableState::SelfPlayLooping;
				}
			}
			else
			{
				mcts->SendTritonRequest();
				mcts->curSimulationMove += 1;
				aiRunnableState = EAIRunnableState::WaitTritonResponse;
			}
		}
		else if (aiRunnableState == EAIRunnableState::WaitTritonResponse)
		{
			if (mcts->CheckTritonReponseAll())
			{
				aiRunnableState = EAIRunnableState::SelfPlaySendTritonRequest;
			}
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayLoopEnd)
		{
			mcts->AddTrainingData(mcts->curTrainingData, StateCodingC, StateCodingH, StateCodingW);
			// save training data to file
			mcts->curSelfPlayLoop += 1;
			aiRunnableState = EAIRunnableState::SelfPlayLooping;
			FTrainingData newTrainingData;
			mcts->curTrainingData = newTrainingData;
			mcts->finishSelfPlayGameTreeRoots.Add(mcts->veryFirstNode);
			mcts->ResetMcts();
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayEnd)
		{

		}
		else if (aiRunnableState == EAIRunnableState::TestTritonRequest)
		{
			mcts->SendTestTritonRequest();
			aiRunnableState = EAIRunnableState::Default;
		}
	}
	return 0;
}

void FAIRunnable::TriggerMctsGetAction(uint8 campNb)
{
	aiRunnableState = EAIRunnableState::Working;
}

void FAIRunnable::TriggerTestGetAction()
{
	aiRunnableState = EAIRunnableState::SelfPlayLooping;
}

void FAIRunnable::TriggerTestTritonInference()
{
	aiRunnableState = EAIRunnableState::TestTritonRequest;
}

void FAIRunnable::TriggerStartSelfPlay()
{
	aiRunnableState = EAIRunnableState::StartSelfPlay;
}

void FAIRunnable::TriggerAssignAction(uint8 campNb,
	int32 launchX,
	int32 launchY,
	int32 targetX,
	int32 targetY,
	ActionType actionType)
{
	waitLaunchX = launchX;
	waitLaunchY = launchY;
	waitTargetX = targetX;
	waitTargetY = targetY;
	waitLaunchCamp = campNb;
	waitActionType = actionType;
	aiRunnableState = EAIRunnableState::Working;

	
}


