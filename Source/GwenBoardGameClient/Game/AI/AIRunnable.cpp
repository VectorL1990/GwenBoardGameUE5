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

void FAIRunnable::Start(UMcts* inMcts)
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
			mcts->realBoard.TriggerAction(waitLaunchCamp, actionCode, renderEffectRounds);
			aiRunnableState = EAIRunnableState::NewState;
		}
		else if (aiRunnableState == EAIRunnableState::StartSelfPlay)
		{
			mcts->curSelfPlayLoop = 0;
			mcts->trainingDatas.Empty();
			aiRunnableState = EAIRunnableState::SelfPlayLooping;
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayLooping)
		{
			if (mcts->curSelfPlayLoop >= mcts->maxSelfPlayLoop)
			{
				mcts->curSimulationMove = 0;
				aiRunnableState = EAIRunnableState::SelfPlayEnd;
			}
			else
			{
				aiRunnableState = EAIRunnableState::SelfPlaySendTritonRequest;
			}
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlaySendTritonRequest)
		{
			if (mcts->curSimulationMove >= mcts->expandSimulationMoves)
			{
				FTrainingData trainingData;
				int32 targetAction;
				ActionType targetActionType;
				mcts->GetTritonAction(targetAction, targetActionType, trainingData.actionProbs);
				mcts->realBoard.StateCoding(mcts->realBoard.curPlayingSectionNb, trainingData.stateCoding);
				TArray<FRenderEffectRound> renderEffectList;
				mcts->realBoard.TriggerAction(mcts->realBoard.curPlayingSectionNb, targetAction, renderEffectList);
				if (targetActionType == ActionType::EndRound)
				{
					if (mcts->realBoard.curPlayingSectionNb == 0)
					{
						mcts->realBoard.curPlayingSectionNb = 1;
					}
					else
					{
						mcts->realBoard.curPlayingSectionNb = 0;
					}
				}

				int32 winner;
				bool isGameEnd = mcts->realBoard.GameEnd(winner);

				if (isGameEnd)
				{
					if (winner == 0)
					{
						trainingData.scores[0] = 1.0;
						trainingData.scores[1] = -1.0;
					}
					else
					{
						trainingData.scores[0] = -1.0;
						trainingData.scores[1] = 1.0;
					}
					mcts->curSimulationMove = 0;
					aiRunnableState = EAIRunnableState::SelfPlayLoopEnd;
				}
				else
				{
					mcts->curSimulationMove = 0;
					aiRunnableState = EAIRunnableState::SelfPlayLooping;
				}

				mcts->trainingDatas.Add(trainingData);
			}
			else
			{
				mcts->SendTritonRequest(curSectionNb);
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
			// save training data to file
			mcts->SaveTrainingData(mcts->trainingDatas);
			mcts->curSelfPlayLoop += 1;
			aiRunnableState = EAIRunnableState::SelfPlayLooping;
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayEnd)
		{

		}
	}
	return 0;
}

void FAIRunnable::TriggerMctsGetAction(uint8 campNb)
{
	aiRunnableState = EAIRunnableState::Working;
	curSectionNb = campNb;
}

void FAIRunnable::TriggerTestGetAction()
{
	aiRunnableState = EAIRunnableState::SelfPlayLooping;
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


