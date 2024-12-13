// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGamePC.h"
#include "../../Base/GwenBoardGameInstance.h"
#include "../CoreGameBlueprintFunctionLibrary.h"



void UMcts::InitMcts(int32 simulationMoves)
{
				expandSimulationMoves = simulationMoves;

				treeRoot = NewObject<UMctsTreeNode>();

				curSearchNode = treeRoot;


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

												realBoard.sectionZeroHandCards.Add(generateCardId);
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

												realBoard.sectionOneHandCards.Add(generateCardId);
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
				for (int32 i = 0; i < realBoard.sectionZeroHandCards.Num(); i++)
				{
								int32 rowNb = UGlobalConstFunctionLibrary::graveCardSectionRow + curInitHandCardRow;
								realBoard.boardRows[rowNb].colCardInfos[curInitHandCardCol] = realBoard.sectionZeroHandCards[i];
								curInitHandCardCol += 1;
								if (curInitHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
								{
												curInitHandCardRow += 1;
												curInitHandCardCol = 0;
								}
				}

				curInitHandCardRow = 0;
				curInitHandCardCol = 0;
				for (int32 i = 0; i < realBoard.sectionOneHandCards.Num(); i++)
				{
								int32 rowNb = UGlobalConstFunctionLibrary::graveCardSectionRow +
												UGlobalConstFunctionLibrary::playCardSectionRow +
												UGlobalConstFunctionLibrary::boardSectionRow +
												curInitHandCardRow;
								realBoard.boardRows[rowNb].colCardInfos[curInitHandCardCol] = realBoard.sectionOneHandCards[i];
								curInitHandCardCol += 1;
								if (curInitHandCardCol >= UGlobalConstFunctionLibrary::maxCol)
								{
												curInitHandCardRow += 1;
												curInitHandCardCol = 0;
								}
				}
}

void UMcts::GetLatestSimulationBoard()
{
				simulationBoard.boardRows = realBoard.boardRows;
				simulationBoard.allInstanceCardInfo = realBoard.allInstanceCardInfo;
				simulationBoard.sectionZeroHandCards = realBoard.sectionZeroHandCards;
				simulationBoard.sectionOneHandCards = realBoard.sectionOneHandCards;
				simulationBoard.sectionZeroGraveCards = realBoard.sectionZeroGraveCards;
				simulationBoard.sectionOneGraveCards = realBoard.sectionOneGraveCards;
}


void UMcts::TriggerSimulation(uint8 sectionNb, int32 simulationNb)
{
				curSearchNode = treeRoot;



				while (true)
				{
								if (curSearchNode == NULL || curSearchNode->IsLeaf())
								{
												break;
								}
								int32 action = 0;
								curSearchNode = curSearchNode->Select(action);
								// we should do move here! So that we can predict next action probs
								TArray<FRenderEffectRound> renderEffectRoundList;
								simulationBoard.TriggerAction(sectionNb, action, renderEffectRoundList);

								curSearchNode->stateStrings = simulationBoard.StateStringCoding();
				}

				int32 boardCoding[1] = {0};
				simulationBoard.StateCoding(boardCoding);
				GetLatestSimulationBoard();

				TMap<int32, float> predictActionProbs;
				float simulationStateValue;
				simulationBoard.GetLegalActionProbsBoardValue(true, sectionNb, boardCoding, predictActionProbs, simulationStateValue);

				// Tell whether game is end
				bool isGameEnd = false;

				if (curSearchNode)
				{
								// expand the tree and update P, U for each node
								if (!isGameEnd)
								{
												curSearchNode->Expand(curSearchNode->hirachy, predictActionProbs);
								}
								else
								{

								}

								curSearchNode->UpdateEvaluateQValue(simulationStateValue);
				}
				

				/*APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
				ACoreCardGamePC* coreCardGamePC = Cast<ACoreCardGamePC>(playerController);
				coreCardGamePC->RefreshMctReplayMenu(curSearchNode, simulationNb);*/
}

void UMcts::GetMoveProbs(uint8 sectionNb, TArray<int32>& outActs, TArray<float>& softmaxProbs)
{
				
				for (int32 i = 0; i < expandSimulationMoves; i++)
				{
								TriggerSimulation(sectionNb, i);
				}

				TArray<float> logVisits;
				for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = curSearchNode->children.CreateConstIterator(); iter; ++iter)
				{
								float logVisit = FMath::Loge(iter->Value->visit + 1e-10);
								outActs.Add(iter->Key);
								logVisits.Add(logVisit);
				}

				UCoreGameBlueprintFunctionLibrary::Softmax(logVisits, 0.001, softmaxProbs);
}

void UMcts::UpdateCurSearchNode(int32 targetMove)
{
				if (curSearchNode->children.Contains(targetMove))
				{
								curSearchNode = curSearchNode->children[targetMove];
								curSearchNode->parent = NULL;
				}
				else
				{
								curSearchNode = treeRoot;
				}
}

void UMcts::GetAction(uint8 sectionNb, int32& targetMove)
{
				TArray<int32> moves;
				TArray<float> softmaxProbs;
				GetMoveProbs(sectionNb, moves, softmaxProbs);
				targetMove = UCoreGameBlueprintFunctionLibrary::GetDirichletAction(moves, softmaxProbs);

				if (isTraining)
				{
								UpdateCurSearchNode(targetMove);
				}
				else
				{
								// reset search tree in real battle case
								UpdateCurSearchNode(-1);
				}
}


