// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "SlashPSActor.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API ASlashPSActor : public ANiagaraActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CheckLive(float dT);

	float curLiveTime = 0.0;
};
