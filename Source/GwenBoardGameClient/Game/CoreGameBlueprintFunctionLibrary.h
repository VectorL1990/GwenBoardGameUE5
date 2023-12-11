// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Scripts/BattleEvents.h"
#include "CoreGameBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class BattleCardStatus : uint8
{
    Select,
    Standby,
    InBattle,
    InGrave,
    UnUsed,
};

UENUM(BlueprintType)
enum class InterludeState : uint8
{
    Default = 0,
    SelectCardDemoPauseInterlude = 1,
    MoveCameraCardSelectionToBattle = 2,
    DemoBoardInterlude = 3,
};

UENUM(BlueprintType)
enum class ClientBattleState : uint8
{
    BeforeBattle = 0,
    SyncHeartBeat = 1,
};

UENUM(BLueprintType)
enum class NetworkStatus : uint8
{
    Default = 0,
    LossInfo = 1,
};

UENUM(BlueprintType)
enum class CameraType : uint8
{
    Default = 0,
    SelectCardCamera = 1,
    BattleCamera = 2,
};



/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCoreGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

};
