// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BattleBoard.h"

// Sets default values
ABattleBoard::ABattleBoard()
{
				// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
				PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleBoard::BeginPlay()
{
				Super::BeginPlay();
	
}

// Called every frame
void ABattleBoard::Tick(float DeltaTime)
{
				Super::Tick(DeltaTime);

}

void ABattleBoard::LaunchSkill(int32 launchX, int32 launchY, int32 targetX, int32 targetY, FEffectInfo& effectInfo)
{
				//if (effectInfo.aoeType == "")
}

void ABattleBoard::SimulationMove()
{

}

void ABattleBoard::StateCoding()
{
				for (TMap<int32, FBoardRow>::TConstIterator rowIter = boardRows.CreateConstIterator(); rowIter; ++rowIter)
				{
								for (TMap<int32, int32>::TConstIterator colIter = rowIter->Value.colCardInfos.CreateConstIterator(); colIter; ++colIter)
								{
												int32 row = rowIter->Key;
												int32 col = colIter->Key;
												int32 uid = colIter->Value;
												
												// 6
												uint8* skillLaunchTypeCoding = GetSkillLaunchTypeCoding(allInstanceCardInfo[uid].originCardInfo.launchType);
												// 10
												uint8* skillLaunchGeoCoding = GetSkillLaunchGeoCoding(allInstanceCardInfo[uid].originCardInfo.launchGeoType);
												// 6
												uint8* skillTargetGeoCoding = GetSkillTargetGeoCoding(allInstanceCardInfo[uid].originCardInfo.targetGeoType);
												// 9
												//uint8* skillTargetLocateGeoCoding = GetSkillTargetLocateGeoCoding(allInstanceCardInfo[uid].originCardInfo.target);
												// 6
												uint8* skillAoeCoding = GetSkillAoeCoding(allInstanceCardInfo[uid].originCardInfo.aoeType);
												// 3
								}
				}
}

uint8* ABattleBoard::GetSkillLaunchTypeCoding(FString launchType)
{
				uint8* coding = skillLaunchTypeCoding;
				if (launchType == "auto")																			coding[0] = 1;
				else if (launchType == "autoRoundEnd")						coding[1] = 1;
				else if (launchType == "manual")												coding[2] = 1;
				else if (launchType == "manualImmediate")			coding[3] = 1;
				else if (launchType == "passive")											coding[4] = 1;
				else if (launchType == "playCard")										coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillLaunchGeoCoding(FString launchGeoType)
{
				uint8* coding = skillLaunchGeoCoding;
				if (launchGeoType == "point")															coding[0] = 1;
				else if (launchGeoType == "three")										coding[1] = 1;
				else if (launchGeoType == "horn")											coding[2] = 1;
				else if (launchGeoType == "triangle")							coding[3] = 1;
				else if (launchGeoType == "obliqueCross")			coding[4] = 1;
				else if (launchGeoType == "normalCross")				coding[5] = 1;
				else if (launchGeoType == "selfSection")				coding[6] = 1;
				else if (launchGeoType == "selfFrontLine")		coding[7] = 1;
				else if (launchGeoType == "oppoSection")				coding[8] = 1;
				else if (launchGeoType == "oppoFrontLine")		coding[9] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetGeoCoding(FString targetGeoType)
{
				uint8* coding = skillTargetGeoCoding;
				if (targetGeoType == "line")																			coding[0] = 1;
				else if (targetGeoType == "seperated")									coding[1] = 1;
				else if (targetGeoType == "diagonal")										coding[2] = 1;
				else if (targetGeoType == "diagonalSeperated") coding[3] = 1;
				else if (targetGeoType == "connect")											coding[4] = 1;
				else if (targetGeoType == "arbitrary")									coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetLocateGeoCoding(FString targetLocateGeoType)
{
				uint8* coding = skillTargetLocateGeoCoding;
				if (targetLocateGeoType == "three")																	coding[0] = 1;
				else if (targetLocateGeoType == "horn")													coding[1] = 1;
				else if (targetLocateGeoType == "triangle")									coding[2] = 1;
				else if (targetLocateGeoType == "obliqueCross")					coding[3] = 1;
				else if (targetLocateGeoType == "normalCross")						coding[4] = 1;
				else if (targetLocateGeoType == "selfSection")						coding[5] = 1;
				else if (targetLocateGeoType == "selfFrontLine")				coding[6] = 1;
				else if (targetLocateGeoType == "oppoSection")						coding[7] = 1;
				else if (targetLocateGeoType == "oppoFrontLine")				coding[8] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillAoeCoding(FString aoeType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillTargetCampCoding(FString targetCampType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillEffectCoding(FString effectType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillAffixCampCoding(FString affixCampType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillAffixCoding(FString affixType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillTagConditionCoding(FString tagCondition)
{
				return 0;
}

uint8* ABattleBoard::GetCardTagCoding(FString tag)
{
				return 0;
}

uint8* ABattleBoard::GetPrereqTagCoding(FString prereqTag)
{
				return 0;
}

uint8* ABattleBoard::GetPrereqCampCoding(FString prereqCampType)
{
				return 0;
}

uint8* ABattleBoard::GetSkillPrereqCoding(FString skillPrereq)
{
				return 0;
}

uint8* ABattleBoard::GetPassiveSkillPrereqTypeCoding(FString passiveSkillPrereq)
{
				return 0;
}

