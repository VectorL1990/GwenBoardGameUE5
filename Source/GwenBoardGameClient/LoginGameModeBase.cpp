// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "Base/GwenBoardGameInstance.h"
#include "UnLua.h"

void ALoginGameModeBase::RequestSignup(FString packMsg)
{
				CallGameInstanceSendData(packMsg);
}
