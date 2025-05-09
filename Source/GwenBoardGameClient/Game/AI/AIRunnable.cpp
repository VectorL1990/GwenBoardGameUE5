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
		else if (aiRunnableState == EAIRunnableState::TestGetAction)
		{
			int32 targetMove;
			mcts->GetAction(curSectionNb, targetMove);
			aiRunnableState = EAIRunnableState::FinishTestGetAction;
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
	aiRunnableState = EAIRunnableState::TestGetAction;
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


