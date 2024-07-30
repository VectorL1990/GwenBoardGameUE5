// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PassiveEffectFunctionLibrary.h"

UPassiveEffectFunctionLibrary::FEffectResultDict GetPassiveEffect(
    TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
    TMap<int32, FBoardRow>& boardCardInfo,
    FEffectInfo& effectInfo,
    int32 launchX,
    int32 launchY,
    int32 targetX,
    int32 targetY,
    TArray<int32>& triggeredUids)
{
    FEffectResultDict effectResultDict;
    return effectResultDict;
}

