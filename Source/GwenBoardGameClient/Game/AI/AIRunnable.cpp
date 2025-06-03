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
			aiRunnableState = EAIRunnableState::GetTritonAction;
		}
		else if (aiRunnableState == EAIRunnableState::GetTritonAction)
		{
			mcts->curSimulationMove = 0;
			aiRunnableState = EAIRunnableState::SendTritonRequest;
		}
		else if (aiRunnableState == EAIRunnableState::SendTritonRequest)
		{
			if (mcts->curSimulationMove >= mcts->expandSimulationMoves)
			{
				FTrainingData trainingData;
				int32 targetAction;
				mcts->GetTritonAction(targetAction, trainingData.actionProbs);
				mcts->realBoard.StateCoding(mcts->realBoard.curSectionNb, trainingData.stateCoding);
				TArray<FRenderEffectRound> renderEffectList;
				mcts->realBoard.TriggerAction(mcts->realBoard.curSectionNb, targetAction, renderEffectList);
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
					aiRunnableState = EAIRunnableState::SelfPlayEnd;
				}
				else
				{
					mcts->curSimulationMove = 0;
					aiRunnableState = EAIRunnableState::GetTritonAction;
				}
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
				aiRunnableState = EAIRunnableState::SendTritonRequest;
			}
		}
		else if (aiRunnableState == EAIRunnableState::SelfPlayEnd)
		{
			// save training data to file
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
	aiRunnableState = EAIRunnableState::GetTritonAction;
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


