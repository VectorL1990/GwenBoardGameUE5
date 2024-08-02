// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalConstFunctionLibrary.generated.h"



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
        int32 moveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 attackDistance;

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
    int32 curCoolDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 curAvailableTimes;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> modifyUids;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> modifyValues;
};

USTRUCT(BlueprintType, Blueprintable)
struct FCopyBoardInfo
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, FBoardRow> boardRows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, FString> boardCardUids;
};

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UGlobalConstFunctionLibrary : public UBlueprintFunctionLibrary
{
				GENERATED_BODY()
public:

    static const int32 maxCol = 8;
    static const int32 maxRow = 8;
    static const int32 playCardSectionRow = 2;
    static const int32 graveCardSectionRow = 3;
    static const int32 boardStateLen = 1000;
};
