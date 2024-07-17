// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "BattleBoard.generated.h"




UCLASS()
class GWENBOARDGAMECLIENT_API ABattleBoard : public AActor
{
	GENERATED_BODY()
	
public:	
				// Sets default values for this actor's properties
				ABattleBoard();

protected:
				// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
				// Called every frame
				virtual void Tick(float DeltaTime) override;

				void LaunchSkill(int32 launchX, int32 launchY, int32 targetX, int32 targetY, FEffectInfo& effectInfo);

				void SimulationMove();

				void StateCoding();

				uint8* GetSkillLaunchTypeCoding(FString launchType);
				uint8* GetSkillLaunchGeoCoding(FString launchGeoType);
				uint8* GetSkillTargetGeoCoding(FString targetGeoType);
				uint8* GetSkillTargetLocateGeoCoding(FString targetLocateGeoType);
				uint8* GetSkillAoeCoding(FString aoeType);
				uint8* GetSkillTargetCampCoding(FString targetCampType);
				uint8* GetSkillEffectCoding(FString effectType);
				uint8* GetSkillAffixCampCoding(FString affixCampType);
				uint8* GetSkillAffixCoding(FString affixType);
				uint8* GetSkillTagConditionCoding(FString tagCondition);
				uint8* GetCardTagCoding(FString tag);
				uint8* GetPrereqTagCoding(FString prereqTag);
				uint8* GetPrereqCampCoding(FString prereqCampType);
				uint8* GetSkillPrereqCoding(FString skillPrereq);
				uint8* GetPassiveSkillPrereqTypeCoding(FString passiveSkillPrereq);

    UPROPERTY()
    TMap<int32, FBoardRow> boardRows;

				UPROPERTY()
				TMap<int32, FInstanceCardInfo> allInstanceCardInfo;

				UPROPERTY()
				TMap<int32, FString> boardCardUids;

				uint8 skillLaunchTypeCoding[6] =								{0};
				uint8 skillLaunchGeoCoding[10] =								{0};
				uint8 skillTargetGeoCoding[6] =									{0};
				uint8 skillTargetLocateGeoCoding[9] =			{0};
				uint8 skillAoeCoding[6] =															{0};
				uint8 skillTargetCampCoding[3] =								{0};
				uint8 skillEffectCoding[86] =											{0};
				uint8 skillAffixCampCoding[3] =									{0};
				uint8 skillAffixCoding[84] =												{0};
				uint8 skillTagConditionCoding[5] =						{0};
				uint8 cardTagCoding[42] =															{0};
				uint8 skillPrereqTagCoding[42] =								{0};
				uint8 skillPrereqCampCoding[3] =								{0};
				uint8 skillPrereqCoding[126] =										{0};
				uint8 passiveSkillPrereqCoding[11] =				{0};
};
