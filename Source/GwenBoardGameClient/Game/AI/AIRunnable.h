// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "../GlobalConstFunctionLibrary.h"
#include "Mcts.h"

class ACoreCardGameModeBase;

/**
 * 
 */
class GWENBOARDGAMECLIENT_API FAIRunnable : public FRunnable
{
public:
				class FRunnableThread* aiRunnableThread = nullptr;

				ACoreCardGameModeBase* threadOwner;

				bool running = false;

				EAIRunnableState aiRunnableState = EAIRunnableState::Default;

				UMcts* mcts;

				uint8 curSectionNb = 0;

				FAIRunnable(ACoreCardGameModeBase* inOwner);
				~FAIRunnable();

				void Start(UMcts* inMcts);

				virtual bool Init() override;
				virtual uint32 Run() override;
				virtual void Stop() override;
				virtual void Exit() override;

				void TriggerMctsGetAction(uint8 sectionNb);
};
