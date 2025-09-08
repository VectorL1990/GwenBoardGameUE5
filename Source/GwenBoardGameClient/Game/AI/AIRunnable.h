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

	UPROPERTY(Transient)
	TObjectPtr<UMcts> mcts;

	int32 waitLaunchX;
	int32 waitLaunchY;
	int32 waitTargetX;
	int32 waitTargetY;
	uint8 waitLaunchCamp;
	ActionType waitActionType;

	UPROPERTY()
	FRenderActionNode newStateRenderRoot;

	ActionType newStateActionType;

	//FCriticalSection criticalSection;

	FAIRunnable(ACoreCardGameModeBase* inOwner);
	~FAIRunnable();

	void Start(TObjectPtr<UMcts> inMcts);

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void TriggerMctsGetAction(uint8 campNb);

	void TriggerAssignAction(uint8 campNb,
		int32 launchX,
		int32 launchY,
		int32 targetX,
		int32 targetY,
		ActionType actionType);

	void TriggerTestGetAction();

	void TriggerTestTritonInference();

	void TriggerStartSelfPlay();
};
