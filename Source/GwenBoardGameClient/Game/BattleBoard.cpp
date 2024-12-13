// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BattleBoard.h"
#include "Kismet/GameplayStatics.h"
#include "Base/GwenBoardGameInstance.h"
#include "CheckTargetGeoRuleLibrary.h"
#include "CheckPrereqTagFunctionLibrary.h"
#include "PassiveEffectFunctionLibrary.h"
#include "CheckPrereqFunctionLibrary.h"

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

void ABattleBoard::InitBattleBoard()
{
				/*
				UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
				UGwenBoardGameInstance* gwenGI = Cast<UGwenBoardGameInstance>(gi);
				int32 generateCardId = 0;

				int32 curHandCardRow = UGlobalConstFunctionLibrary::graveCardSectionRow;
				int32 curHandCardCol = 0;
				for (int32 i = 0; i < gwenGI->sectionZeroPileCards.Num(); i++)
				{
								FInstanceCardInfo instanceCardInfo;
								instanceCardInfo.originCardInfo = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]];
								instanceCardInfo.camp = 0;
								instanceCardInfo.curAvailableTimes = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].availableTimes;
								instanceCardInfo.curCoolDown = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].initCoolDown;
								instanceCardInfo.curDefence = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].defence;
								instanceCardInfo.curHp = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].hp;
								if (i < UGlobalConstFunctionLibrary::handCardNb)
								{
												if (curHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
												{
																curHandCardCol = 0;
																curHandCardRow += 1;
												}
												instanceCardInfo.curCol = curHandCardCol;
												instanceCardInfo.curRow = curHandCardRow;
												
												curHandCardCol += 1;

												sectionZeroHandCards.Add(generateCardId);
								}
								else
								{
												instanceCardInfo.curCol = -1;
												instanceCardInfo.curRow = -1;
								}
								realBoard.allInstanceCardInfo.Add(generateCardId, instanceCardInfo);
								generateCardId += 1;
				}

				curHandCardRow = UGlobalConstFunctionLibrary::graveCardSectionRow +
								UGlobalConstFunctionLibrary::playCardSectionRow +
								UGlobalConstFunctionLibrary::boardSectionRow;
				curHandCardCol = 0;
				for (int32 i = 0; i < gwenGI->sectionOnePileCards.Num(); i++)
				{
								FInstanceCardInfo instanceCardInfo;
								instanceCardInfo.originCardInfo = gwenGI->allCardInfos[gwenGI->sectionOnePileCards[i]];
								instanceCardInfo.camp = 1;
								instanceCardInfo.curAvailableTimes = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].availableTimes;
								instanceCardInfo.curCoolDown = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].initCoolDown;
								instanceCardInfo.curDefence = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].defence;
								instanceCardInfo.curHp = gwenGI->allCardInfos[gwenGI->sectionZeroPileCards[i]].hp;
								if (i < UGlobalConstFunctionLibrary::handCardNb)
								{
												if (curHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
												{
																curHandCardCol = 0;
																curHandCardRow += 1;
												}
												instanceCardInfo.curCol = curHandCardCol;
												instanceCardInfo.curRow = curHandCardRow;

												curHandCardCol += 1;

												sectionOneHandCards.Add(generateCardId);
								}
								else
								{
												instanceCardInfo.curCol = -1;
												instanceCardInfo.curRow = -1;
								}
								realBoard.allInstanceCardInfo.Add(generateCardId, instanceCardInfo);
								generateCardId += 1;
				}


				realBoard.boardRows.Init(FBoardRow(), UGlobalConstFunctionLibrary::graveCardSectionRow * 2 +
								UGlobalConstFunctionLibrary::playCardSectionRow * 2 +
								UGlobalConstFunctionLibrary::boardSectionRow);

				for (int32 i = 0; i < realBoard.boardRows.Num(); i++)
				{
								realBoard.boardRows[i].colCardInfos.Init(-1, UGlobalConstFunctionLibrary::maxCol);
				}

				int32 curInitHandCardRow = 0;
				int32 curInitHandCardCol = 0;
				for (int32 i = 0; i < sectionZeroHandCards.Num(); i++)
				{
								int32 rowNb = UGlobalConstFunctionLibrary::graveCardSectionRow + curInitHandCardRow;
								realBoard.boardRows[rowNb].colCardInfos[curInitHandCardCol] = sectionZeroHandCards[i];
								curInitHandCardCol += 1;
								if (curInitHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
								{
												curInitHandCardRow += 1;
												curInitHandCardCol = 0;
								}
				}

				curInitHandCardRow = 0;
				curInitHandCardCol = 0;
				for (int32 i = 0; i < sectionOneHandCards.Num(); i++)
				{
								int32 rowNb = UGlobalConstFunctionLibrary::graveCardSectionRow + 
												UGlobalConstFunctionLibrary::playCardSectionRow + 
												UGlobalConstFunctionLibrary::boardSectionRow + 
												curInitHandCardRow;
								realBoard.boardRows[rowNb].colCardInfos[curInitHandCardCol] = sectionOneHandCards[i];
								curInitHandCardCol += 1;
								if (curInitHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
								{
												curInitHandCardRow += 1;
												curInitHandCardCol = 0;
								}
				}
				*/
}

bool ABattleBoard::CheckGameEnd()
{
				return false;
}


