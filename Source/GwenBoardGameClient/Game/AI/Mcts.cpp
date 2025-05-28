// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGameModeBase.h"
#include "../CoreCardGamePC.h"
#include "../../Base/GwenBoardGameInstance.h"
#include "../CoreGameBlueprintFunctionLibrary.h"



void UMcts::InitMcts(int32 simulationMoves)
{
	expandSimulationMoves = simulationMoves;

	veryFirstNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	tritonHttpClient = NewObject<UTritonHttpClient>(GetWorld(), tritonHttpClientBPClass);
	treeRoot = veryFirstNode;

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
			AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
			ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
			coreCardGameMode->SpawnHandCard(instanceCardInfo.originCardInfo.cardName,
				0, generateCardId,
				instanceCardInfo.originCardInfo.hp,
				instanceCardInfo.originCardInfo.defence,
				i);
			realBoard.allInstanceCardInfo.Add(generateCardId, instanceCardInfo);
			generateCardId += 1;
		}
		else
		{
			instanceCardInfo.curCol = -1;
			instanceCardInfo.curRow = -1;
		}
		
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
			AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
			ACoreCardGameModeBase* coreCardGameMode = Cast<ACoreCardGameModeBase>(gameMode);
			coreCardGameMode->SpawnHandCard(instanceCardInfo.originCardInfo.cardName, 
				1, generateCardId,
				instanceCardInfo.originCardInfo.hp,
				instanceCardInfo.originCardInfo.defence, i);
			realBoard.allInstanceCardInfo.Add(generateCardId, instanceCardInfo);
			generateCardId += 1;
		}
		else
		{
			instanceCardInfo.curCol = -1;
			instanceCardInfo.curRow = -1;
		}
		
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

void UMcts::UpdateCurSearchNode(int32 targetMove)
{
	if (treeRoot->children.Contains(targetMove))
	{
		treeRoot = treeRoot->children[targetMove];
		treeRoot->parent = NULL;
	}
	else
	{
		treeRoot->ResetNode();
	}
}

void UMcts::SendTritonRequest(uint8 sectionNb)
{
	for (int32 i = 0; i < expandSimulationMoves; i++)
	{
		uint8 curSectionNb = sectionNb;
		curSearchNode = treeRoot;

		FBoardInfo copyBoard = realBoard.GetCopyBoard();
		//GetLatestSimulationBoard();
		while (true)
		{
			if (curSearchNode == NULL || curSearchNode->IsLeaf())
			{
				break;
			}
			int32 action = 0;
			curSearchNode = curSearchNode->Select(action);
			int32 launchX = 0;
			int32 launchY = 0;
			int32 targetX = 0;
			int32 targetY = 0;
			ActionType actionType = ActionType::EndRound;
			copyBoard.ActionDecoding(action, launchX, launchY, targetX, targetY, actionType);
			// we should do move here! So that we can predict next action probs
			TArray<FRenderEffectRound> renderEffectRoundList;
			copyBoard.TriggerAction(curSectionNb, action, renderEffectRoundList);
			if (actionType == ActionType::EndRound)
			{
				if (curSectionNb == 0)
				{
					curSectionNb = 1;
				}
				else
				{
					curSectionNb = 0;
				}
			}
		}

		int32 boardCoding[TotalCHW] = { 0 };
		copyBoard.StateCoding(boardCoding);

		int32 requestID = GetCurTritonRequestID();
		FTritonResponseData tritonResponseData;
		tritonResponseData.curBoardInfo = copyBoard;
		tritonResponseData.curMctsTreeNode = curSearchNode;
		tritonResponseData.curSectionNb = curSectionNb;
		tritonResponseDatas.Add(tritonResponseData);
		tritonHttpClient->SendInferenceRequest("GwenNetModel", boardCoding, TotalCHW, requestID);
	}
}

int32 UMcts::GetCurTritonRequestID()
{
	int32 tmpTritonRequestID = curTritonRequestID;
	curTritonRequestID += 1;
	return tmpTritonRequestID;
}

void UMcts::CheckTritonReponseAll()
{
	if (receivedTritonResponseNb < expandSimulationMoves)
	{
		return;
	}

	// Expand searching tree first
	for (int32 i = 0; i < tritonResponseDatas.Num(); i++)
	{
		TArray<int32> legalActionIds;
		TArray<ActionType> legalActionTypes;
		tritonResponseDatas[i].curBoardInfo.GetLegalMoves(tritonResponseDatas[i].curSectionNb, legalActionIds, legalActionTypes);

		for (int32 j = 0; j < legalActionIds.Num(); j++)
		{
			TArray<FRenderEffectRound> renderEffectRoundList;
			ActionType testActionType = legalActionTypes[legalActionIds[j]];
			// Trigger action just for replay
			tritonResponseDatas[i].curBoardInfo.TriggerAction(
				tritonResponseDatas[i].curSectionNb, legalActionIds[j], renderEffectRoundList);
			UMctsTreeNode* newNode = tritonResponseDatas[i].curMctsTreeNode->ExpandNode(
				tritonResponseDatas[i].curMctsTreeNode->hirachy,
				legalActionIds[j],
				tritonResponseDatas[i].policies[legalActionIds[j]],
				tritonResponseDatas[i].curBoardInfo.boardRows,
				tritonResponseDatas[i].curBoardInfo.allInstanceCardInfo);
			newAddNodes.Add(newNode);
		}

		tritonResponseDatas[i].curMctsTreeNode->UpdateQValueRecursive(tritonResponseDatas[i].boardValue);
	}

	// After expand nodes, we should find ideal motion and do action
	TArray<float> logVisits;
	TArray<int32> candidateActs;
	for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = treeRoot->children.CreateConstIterator(); iter; ++iter)
	{
		float logVisit = FMath::Loge(iter->Value->visit + 1e-10);
		candidateActs.Add(iter->Key);
		logVisits.Add(logVisit);
	}
	TArray<float> softmaxProbs;
	UCoreGameBlueprintFunctionLibrary::Softmax(logVisits, 0.001, softmaxProbs);

	int32 targetMove = UCoreGameBlueprintFunctionLibrary::GetDirichletAction(candidateActs, softmaxProbs);

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
