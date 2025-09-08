// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.h"
#include "Scripts/BattleEvents.h"
#include <vector>
#include <random>
#include "CheckTargetGeoRuleLibrary.h"
#include "CheckPrereqFunctionLibrary.h"
#include "CheckPrereqTagFunctionLibrary.h"
#include "PassiveEffectFunctionLibrary.h"
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
    InPile,
    InHand,
    InBattle,
    InGrave,
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
    AfterSelectCardInterlude = 2,
    SelectCardAnimInterlude = 3,
    Battle = 3,
    BattleInterlude = 4,
    ActionInterlude = 5,
    RenderEffectInterlude = 6
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



UENUM(BlueprintType)
enum class EGamingType : uint8
{
    Training = 0,
    SinglePlay = 1,
};






class FastGammaSampler
{
public:
    FastGammaSampler(double alpha, unsigned seed = std::random_device{}())
        : alpha(alpha), rng(seed), uniform(0.0, 1.0) {
    }

    double operator()() {
        // Johnk's算法：适用于α < 1
        while (true) {
            double U = uniform(rng);
            double V = uniform(rng);
            double X = std::pow(U, 1.0 / alpha);
            double Y = std::pow(V, 1.0 / (1.0 - alpha));
            if (X + Y <= 1.0) {
                double Z = X / (X + Y);
                double W = -std::log(uniform(rng)); // 指数分布
                return Z * W;
            }
        }
    }

private:
    double alpha;
    std::mt19937 rng;
    std::uniform_real_distribution<double> uniform;
};





/**
 *
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCoreGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    static void Softmax(const TArray<float>& x, TArray<float>& softmax);

    static int32 GetRealDirichletAction(const TArray<int32>& actions,
        const TArray<ActionType>& actionTypes,
        const TArray<float>& probs,
        int32& outAction,
        ActionType& outActionType);

    static int32 GetDirichletAction(const TArray<int32>& actions, 
        const TArray<ActionType>& actionTypes, 
        const TArray<float>& probs, 
        int32& outAction, 
        ActionType& outActionType);

    static void GetActionDetailFromId(int32 actionId, int32& launchX, int32& launchY, int32& targetX, int32& targetY, ActionType& actionType);

    static TArray<FGridXY> GetAoeTargetGrids(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TArray<FBoardRow>& boardCardInfo, 
            int32 launchX, 
            int32 launchY, 
            int32 targetX, 
            int32 targetY, 
            FString aoeType, 
            FString targetCamp);

    static FEffectResultDict LaunchRoundEndSkillDict(
        uint8 launchCamp,
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        bool isPotentialVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict LaunchPlayCardSkillDict(
        uint8 launchCamp,
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict LaunchPassiveSkillDict(
        uint8 launchCamp,
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectResultDict triggerEffectResult,
        FEffectInfo& effectInfo,
        FString triggerEffectType,
        int32 launchX,
        int32 launchY,
        int32 triggerX,
        int32 triggerY,
        bool isVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict LaunchSkillDict(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isPotentialVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict Hurt(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isPotentialVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict Heal(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isPotentialVirtual,
        int32& sectionZeroScore,
        int32& sectionOneScore);

    static FEffectResultDict IncreaseDefence(
        TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isPotentialVirtual);

    static FEffectResultDict Wound(TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
        TArray<FBoardRow>& boardCardInfo,
        FEffectInfo& effectInfo,
        int32 launchX,
        int32 launchY,
        int32 targetX,
        int32 targetY,
        bool isPotentialVirtual);

    static void ConvertStateToJson(const TArray<FString>& stateArray,
        const int32 col,
        const int32 row);

    static bool WriteStringToFile(const FString& fileName, const FString& relativePath, const FString& writeString);
};
