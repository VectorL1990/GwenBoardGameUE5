// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.generated.h"

#define StateCodingTotalCHW 12528
#define StateCodingC 87
#define StateCodingH 18
#define StateCodingW 8
#define defaultRenderPlayCardInterval 1.0;
#define defaultRenderMoveCardInterval 1.0;

UENUM(BlueprintType)
enum class EAtkDistanceType : uint8
{
    Closed = 0,
    Far = 1,
};

UENUM(BlueprintType)
enum class ECardVerticalMotionStage : uint8
{
    Default = 0,
    RiseAcc = 1,
    RiseDec = 2,
    DropAcc = 3,
    RebounceUp = 4,
    RebounceDown = 5,
};

UENUM(BlueprintType)
enum class ECardRotationStage : uint8
{
    Default = 0,
    PosRotate = 1,
    NegRotate = 2,
};

UENUM(BlueprintType)
enum class ECardHorizonMotionStage : uint8
{
    Default = 0,
    Acc = 1,
    Dec = 2,
};

UENUM(BlueprintType)
enum class ECardFloatMotionStage : uint8
{
    Default = 0,
    Acc = 1,
    Dec = 2,
};

enum class ECardFloatRotateStage : uint8
{
    Default,
    FirstStageIncrease,
    FirstStageDecrease,
    SecondStageIncrease,
    SecondStageDecrease
};

UENUM(BlueprintType)
enum class ActionType : uint8
{
    PlayCard = 0,
    LaunchSkill = 1,
    Move = 2,
    EndRound = 3
};

UENUM(BlueprintType)
enum class EGameModeRenderState : uint8
{
    Default = 0,
    EndRoundRender = 1,
    RenderingStep = 2,
    ActionTimeOut = 3,
    ActionTimeOutWaitRender = 4,
    ActionTimeOutRenderStep = 5,
    RenderingEndRound = 6,
};

UENUM(BlueprintType)
enum class EEndRoundButtonState : uint8
{
    SelfNotPlayCardYet = 0,
    SelfHasPlayCard = 1,
    OppoNotPlayCardYet = 2,
    OppoHasPlayCard = 3,
};


USTRUCT(BlueprintType, Blueprintable)
struct FCardInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString cardName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString aliasCN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString aliasEN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString cardFunctionCN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString cardFunctionEN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString despCN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString despEN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 hp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 defence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 agility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FString> cardTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString launchType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 coolDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 availableTimes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString launchGeoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString targetGeoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString aoeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString targetCamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectAffix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectAffixCamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqTagCondition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqCampType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 prereqValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString passivePrereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<int32> values;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAtkDistanceType moveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EAtkDistanceType attackDistanceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture* texture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString moveType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString renderEffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float renderEffectTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString cardCategory;
};

USTRUCT(BlueprintType, Blueprintable)
struct FInstanceCardInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FCardInfo originCardInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        uint8 camp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 curHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 curCoolDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 curAvailableTimes;

    UPROPERTY()
    TMap<FString, int32> curExtraTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 curDefence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 curRow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 curCol;

    UPROPERTY()
    bool passiveEffectTriggerThisRound = false;
};

USTRUCT(BlueprintType, Blueprintable)
struct FGetAffixInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY()
    int32 effectValue;

    UPROPERTY()
    FString costType;

    UPROPERTY()
    int32 costValue;
};



USTRUCT(BlueprintType, Blueprintable)
struct FEffectInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString launchType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 coolDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 availableTimes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString launchGeoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString targetGeoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString aoeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString targetCamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectAffix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString effectAffixCamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqTagCondition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqCampType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString prereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 prereqValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString passivePrereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<int32> values;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString renderEffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float renderEffectTime;
};


USTRUCT(BlueprintType, Blueprintable)
struct FBoardRow
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> colCardInfos;
};

USTRUCT(BlueprintType, Blueprintable)
struct FGridXY
{
    GENERATED_USTRUCT_BODY()
public:
    FGridXY()
    {
        x = 0;
        y = 0;
    }

    FGridXY(int32 inX, int32 inY)
    {
        x = inX;
        y = inY;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 x;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 y;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectResultDict
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool success;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString modifyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 triggerGridX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 triggerGridY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGridXY> modifyGrids;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> modifyUids;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> modifyValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString renderEffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float renderTime;
};


USTRUCT(BlueprintType, Blueprintable)
struct FRenderActionNode
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString renderEffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 triggerCardId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 triggerGridX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 triggerGridY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> targetGridXs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> targetGridYs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> modifyUids;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> modifyValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float renderTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ActionType actionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 renderRound;

    TArray<FRenderActionNode> children;

    float curRenderTime = 0.0;

    bool triggerRender = false;
};



USTRUCT(BlueprintType, Blueprintable)
struct FActionDemonstrationInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY()
    int32 launchX;

    UPROPERTY()
    int32 launchY;

    UPROPERTY()
    int32 targetX;

    UPROPERTY()
    int32 targetY;

    UPROPERTY()
    FString actionType;

    UPROPERTY()
    FString effectDemoType;
};

enum class EAIRunnableState
{
    Default,
    NewTask,
    Working,
    StartSelfPlay,
    SelfPlayLooping,
    SelfPlaySendTritonRequest,
    WaitTritonResponse,
    NextSimulation,
    SelfPlayLoopEnd,
    SelfPlayEnd,

    TestGetAction,
    FinishTestGetAction,
    NewState,

    TestTritonRequest,
};




/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UGlobalConstFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static const int32 handCardNb = 10;
    static const int32 maxCol = 8;
    static const int32 boardSectionRow = 6;
    static const int32 playCardSectionRow = 2;
    static const int32 graveCardSectionRow = 3;

    

    uint8 autoSkillGeoTargetTypeCoding[17] = { 0 };
    uint8 skillLaunchTypeCoding[6] = { 0 };
    uint8 skillLaunchGeoCoding[10] = { 0 };
    uint8 skillTargetGeoCoding[6] = { 0 };
    uint8 skillTargetLocateGeoCoding[9] = { 0 };
    uint8 skillAoeCoding[6] = { 0 };
    uint8 skillTargetCampCoding[3] = { 0 };
    uint8 skillEffectCoding[86] = { 0 };
    uint8 skillAffixCampCoding[3] = { 0 };
    uint8 skillAffixCoding[84] = { 0 };
    uint8 skillTagConditionCoding[5] = { 0 };
    uint8 cardTagCoding[42] = { 0 };
    uint8 skillPrereqTagCoding[42] = { 0 };
    uint8 skillPrereqCampCoding[3] = { 0 };
    uint8 skillPrereqCoding[126] = { 0 };
    uint8 passiveSkillPrereqCoding[11] = { 0 };
};
