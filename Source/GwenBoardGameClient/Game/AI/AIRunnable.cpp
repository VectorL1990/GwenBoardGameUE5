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
												int32 targetMove;
												mcts->GetAction(curSectionNb, targetMove);
												aiRunnableState = EAIRunnableState::Default;
								}
				}
				return 0;
}

void FAIRunnable::TriggerMctsGetAction(uint8 sectionNb)
{
				aiRunnableState = EAIRunnableState::Working;
				curSectionNb = sectionNb;
}


