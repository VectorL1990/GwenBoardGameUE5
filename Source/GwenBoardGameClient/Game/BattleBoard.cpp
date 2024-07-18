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
												
												uint8 coding[200] = { 0 };
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
				uint8* coding = skillAoeCoding;
				if (aoeType == "point")													coding[0] = 1;
				else if (aoeType == "sweep")								coding[1] = 1;
				else if (aoeType == "H3")											coding[2] = 1;
				else if (aoeType == "V3")											coding[3] = 1;
				else if (aoeType == "normalCross")						coding[4] = 1;
				else if (aoeType == "obliqueCross")					coding[5] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillTargetCampCoding(FString targetCampType)
{
				uint8* coding = skillTargetCampCoding;
				if (targetCampType == "self")							coding[0] = 1;
				else if (targetCampType == "oppo")		coding[1] = 1;
				else if (targetCampType == "none")		coding[2] = 1;
				return coding;
}

uint8* ABattleBoard::GetSkillEffectCoding(FString effectType)
{
				uint8* coding = skillEffectCoding;
				if (effectType == "hurt")																			coding[0] = 1;
				else if (effectType == "heal")																		coding[1] = 1;
				else if (effectType == "increaseDefence")											coding[2] = 1;
				else if (effectType == "replaceDefence")												coding[3] = 1;
				else if (effectType == "increaseSelfDefence")							coding[4] = 1;
				else if (effectType == "giveTempArmor")									coding[5] = 1;
				else if (effectType == "giveArmor")													coding[6] = 1;
				else if (effectType == "tempArmor")													coding[7] = 1;
				else if (effectType == "defenceHurt")											coding[8] = 1;
				else if (effectType == "defenceHeal")											coding[9] = 1;
				else if (effectType == "defenceDetonate")							coding[10] = 1;
				else if (effectType == "useArmorHurt")										coding[11] = 1;
				else if (effectType == "useArmorHeal")										coding[12] = 1;
				else if (effectType == "armorDetonate")									coding[13] = 1;
				else if (effectType == "armorDetonateSilence")						coding[14] = 1;
				else if (effectType == "armorDetonateWound")								coding[15] = 1;
				else if (effectType == "armorDetonatePoison")							coding[16] = 1;
				else if (effectType == "armorDetonateSublime")						coding[17] = 1;
				else if (effectType == "switchCamp")												coding[18] = 1;
				else if (effectType == "capture")															coding[19] = 1;
				else if (effectType == "hurtTransfer")														coding[20] = 1;
				else if (effectType == "hurtLink")																		coding[21] = 1;
				else if (effectType == "healTransfer")										coding[22] = 1;
				else if (effectType == "healLink")														coding[23] = 1;
				else if (effectType == "sublimeTransfer")											coding[24] = 1;
				else if (effectType == "silenceTransfer")											coding[25] = 1;
				else if (effectType == "woundTransfer")													coding[26] = 1;
				else if (effectType == "wound")																	coding[27] = 1;
				else if (effectType == "convertSublimeToWound")					coding[28] = 1;
				else if (effectType == "transferWound")													coding[29] = 1;
				else if (effectType == "revenge")																			coding[30] = 1;
				else if (effectType == "revengeWound")														coding[31] = 1;
				else if (effectType == "repayHeal")																	coding[32] = 1;
				else if (effectType == "repaySublime")														coding[33] = 1;
				else if (effectType == "devour")																coding[34] = 1;
				else if (effectType == "devourSublime")													coding[35] = 1;
				else if (effectType == "devourButWound")												coding[36] = 1;
				else if (effectType == "deadWishConvert")											coding[37] = 1;
				else if (effectType == "deadWishHurt")														coding[38] = 1;
				else if (effectType == "deadWishSpawnUp")											coding[39] = 1;
				else if (effectType == "deadWishSpawnDown")									coding[40] = 1;
				else if (effectType == "deadWishSpawnRight")								coding[41] = 1;
				else if (effectType == "deadWishSpawnLeft")									coding[42] = 1;
				else if (effectType == "deadWishDestroyUp")													coding[43] = 1;
				else if (effectType == "deadWishDestroyDown")											coding[44] = 1;
				else if (effectType == "deadWishDestroyRight") coding[45] = 1;
				else if (effectType == "deadWishDestroyLeft") coding[46] = 1;
				else if (effectType == "deadWishHeal") coding[47] = 1;
				else if (effectType == "deadWishHealUp") coding[48] = 1;
				else if (effectType == "deadWishHealDown") coding[49] = 1;
				else if (effectType == "deadWishHealRight") coding[50] = 1;
				else if (effectType == "deadWishHealLeft") coding[51] = 1;
				else if (effectType == "deadWishSublime") coding[52] = 1;
				else if (effectType == "deadWishTackle") coding[53] = 1;
				else if (effectType == "deadWishWound") coding[54] = 1;
				else if (effectType == "explode") coding[55] = 1;
				else if (effectType == "explodeToPoison") coding[56] = 1;
				else if (effectType == "explodeToWound") coding[57] = 1;
				else if (effectType == "exchange") coding[58] = 1;
				else if (effectType == "drag") coding[59] = 1;
				else if (effectType == "push") coding[60] = 1;
				else if (effectType == "sublime") coding[61] = 1;
				else if (effectType == "lock") coding[62] = 1;
				else if (effectType == "tempLock") coding[63] = 1;
				else if (effectType == "purify") coding[64] = 1;
				else if (effectType == "spawn") coding[65] = 1;
				else if (effectType == "cloneUp") coding[66] = 1;
				else if (effectType == "cloneDown") coding[67] = 1;
				else if (effectType == "cloneRight") coding[68] = 1;
				else if (effectType == "cloneLeft") coding[69] = 1;
				else if (effectType == "silence") coding[70] = 1;
				else if (effectType == "tempSilence") coding[71] = 1;
				else if (effectType == "copyHp") coding[72] = 1;
				else if (effectType == "summonFirstDeath") coding[73] = 1;
				else if (effectType == "exchangeFirstDeath") coding[74] = 1;
				else if (effectType == "tackle") coding[75] = 1;
				else if (effectType == "summonFirstPile") coding[76] = 1;
				else if (effectType == "exchangeFirstPile") coding[77] = 1;
				else if (effectType == "duel") coding[78] = 1;
				else if (effectType == "recover") coding[79] = 1;
				else if (effectType == "assignHp") coding[80] = 1;
				else if (effectType == "copyGraveFirstHp") coding[81] = 1;
				else if (effectType == "healFromWound") coding[82] = 1;
				else if (effectType == "copyFirstGraveHp") coding[83] = 1;
				else if (effectType == "increaseFirstDeathHpDefence") coding[84] = 1;
				else if (effectType == "increaseFirstPileHp") coding[85] = 1;

				return coding;
}

uint8* ABattleBoard::GetSkillAffixCampCoding(FString affixCampType)
{
				uint8* coding = skillAffixCampCoding;
				if (affixCampType == "self") coding[0] = 1;
				else if (affixCampType == "oppo") coding[1] = 1;
				else if (affixCampType == "none") coding[2] = 1;
				return coding;
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

