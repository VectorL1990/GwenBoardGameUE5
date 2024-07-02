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
        FString passivePrereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<int32> values;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture* texture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString moveType;

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
    int32 curDefence;
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
        FString passivePrereqType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<int32> values;
};


USTRUCT(BlueprintType, Blueprintable)
struct FBoardRow
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TMap<int32, int32> colCardInfos;
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
    TArray<FGridXY> modifyGrids;
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

    static TArray<FGridXY> GetAoeTargetGrids(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TMap<int32, FBoardRow>& boardCardInfo, 
            int32 launchX, 
            int32 launchY, 
            int32 targetX, 
            int32 targetY, 
            AoeType aoeType, 
            FString targetCamp);

    static FEffectResultDict IncreaseDefence(
            TMap<int32, FInstanceCardInfo>& allInstanceCardInfo,
            TMap<int32, FBoardRow>& boardCardInfo,
            FEffectInfo& effectInfo,
            int32 launchX,
            int32 launchY,
            int32 targetX,
            int32 targetY);
};
