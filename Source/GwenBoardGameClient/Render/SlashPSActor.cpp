// Fill out your copyright notice in the Description page of Project Settings.


#include "Render/SlashPSActor.h"

void ASlashPSActor::CheckLive(float dT)
{
	if (curLiveTime >= 1.0)
	{
		ConditionalBeginDestroy();
	}
	else
	{
		curLiveTime += dT;
	}
}


