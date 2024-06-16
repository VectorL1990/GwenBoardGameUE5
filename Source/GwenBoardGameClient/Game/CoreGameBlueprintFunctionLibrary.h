// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Scripts/BattleEvents.h"
#include "CoreGameBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class CardAnimationStatus : uint8
{
    Default = 0,
    Generating = 1,
    Discarding = 2,
};

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
    Default = 0,
    ReqEnterRoom = 1,
    SelectCard = 2,
    InBattle = 3,
};

UENUM(BlueprintType)
enum class SingleBattleState : uint8
{
    Default = 0,
    SelectCard = 1,
    SelectCardInterlude = 2,
    Battle = 3,
    BattleInterlude = 4
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
