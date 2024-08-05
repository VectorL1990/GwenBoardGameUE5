// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "Scripts/BattleEvents.h"
#include <vector>
#include <random>
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

UENUM(BlueprintType)
enum class BattleCardWidgetType : uint8
{
    BattleSelectCard = 0,
};

UENUM(BlueprintType)
enum class AoeType : uint8
{
    Point = 0,
    H3 = 1,
    V3 = 2,
    Sweep = 3,
    NormalCross = 4,
    ObliqueCross = 5,
};

enum class ActionType : uint8
{
    PlayCard = 0,
    LaunchSkill = 1,
    Move = 2,
    EndRound = 3
};

enum class TrainState : uint8
{
    NoAction = 0,
};




/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCoreGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static const int32 maxCol = 8;
    static const int32 maxRow = 8;

    static void Softmax(const TArray<float>& x, float temp, TArray<float>& softmax);

    static void QueryRemotePolicyValue(uint8* boardState, TArray<float>& actionProbs, float& stateValue);

    static int32 GetDirichletAction(const TArray<int32>& actions, const TArray<float>& probs);

    static int32 GetActionId(int32 launchX, int32 launchY, int32 targetX, int32 targetY, ActionType actionType);

    static TArray<FGridXY> GetAoeTargetGrids(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TMap<int32, FBoardRow>& boardCardInfo, 
            int32 launchX, 
            int32 launchY, 
            int32 targetX, 
            int32 targetY, 
            FString aoeType, 
            FString targetCamp);

    static FEffectResultDict LaunchSkillDict(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TMap<int32, FBoardRow>& boardCardInfo,
            FEffectInfo& effectInfo,
            int32 launchX,
            int32 launchY,
            int32 targetX,
            int32 targetY);

    static FEffectResultDict IncreaseDefence(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TMap<int32, FBoardRow>& boardCardInfo,
            FEffectInfo& effectInfo,
            int32 launchX,
            int32 launchY,
            int32 targetX,
            int32 targetY);

    static FEffectResultDict ReplaceDefence(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TMap<int32, FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY);
};
