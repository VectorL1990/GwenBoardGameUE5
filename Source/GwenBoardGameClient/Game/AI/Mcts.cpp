// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/Mcts.h"
#include "Kismet/GameplayStatics.h"
#include "../CoreCardGameModeBase.h"
#include "../CoreCardGamePC.h"
#include "../../Base/GwenBoardGameInstance.h"
#include "HAL/UnrealMemory.h"
#include "Misc/FileHelper.h"
#include "Serialization/MemoryWriter.h"
#include "../CoreGameBlueprintFunctionLibrary.h"



void UMcts::InitMcts(int32 simulationMoves)
{
	expandSimulationMoves = simulationMoves;

	veryFirstNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	tritonHttpClient = NewObject<UTritonHttpClient>(GetWorld(), tritonHttpClientBPClass);
	tritonHttpClient->InitTritonClient(this);
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
	uint8 curPlayingSectionNb = sectionNb;
	curSearchNode = treeRoot;

	FBoardInfo copyBoard = realBoard.GetCopyBoard();
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
		copyBoard.TriggerAction(curPlayingSectionNb, action, renderEffectRoundList);
		if (actionType == ActionType::EndRound)
		{
			if (curPlayingSectionNb == 0)
			{
				curPlayingSectionNb = 1;
			}
			else
			{
				curPlayingSectionNb = 0;
			}
		}
	}

	copyBoard.StateCoding(curPlayingSectionNb, curSearchNode->stateCoding);

	int32 requestID = GetCurTritonRequestID();
	tritonResponseData.curBoardInfo = copyBoard;
	tritonResponseData.curMctsTreeNode = curSearchNode;
	tritonResponseData.curPlayingSectionNb = curPlayingSectionNb;
	tritonHttpClient->SendInferenceRequest("GwenNetModel", curSearchNode->stateCoding, TotalCHW, requestID);
}

int32 UMcts::GetCurTritonRequestID()
{
	int32 tmpTritonRequestID = curTritonRequestID;
	curTritonRequestID += 1;
	return tmpTritonRequestID;
}

bool UMcts::CheckTritonReponseAll()
{
	if (!receivedTritonResponse)
	{
		return false;
	}

	int32 winner;
	bool isGameEnd = tritonResponseData.curBoardInfo.GameEnd(winner);

	if (isGameEnd)
	{
		if (winner == -1)
		{
			tritonResponseData.boardValue = 0.0;
		}
		else
		{
			if (winner == tritonResponseData.curPlayingSectionNb)
			{
				tritonResponseData.boardValue = 1.0;
			}
			else
			{
				tritonResponseData.boardValue = -1.0;
			}
		}
	}
	else
	{
		// Expand searching tree first
		TArray<int32> legalActionIds;
		TArray<ActionType> legalActionTypes;
		tritonResponseData.curBoardInfo.GetLegalMoves(tritonResponseData.curPlayingSectionNb, legalActionIds, legalActionTypes);

		for (int32 j = 0; j < legalActionIds.Num(); j++)
		{
			TArray<FRenderEffectRound> renderEffectRoundList;
			ActionType testActionType = legalActionTypes[j];
			// Trigger action just for replay
			FBoardInfo copyBoard = tritonResponseData.curBoardInfo.GetCopyBoard();
			copyBoard.TriggerAction(tritonResponseData.curPlayingSectionNb,
				legalActionIds[j], renderEffectRoundList);
			UMctsTreeNode* newNode = tritonResponseData.curMctsTreeNode->ExpandNode(
				tritonResponseData.curMctsTreeNode->hirachy,
				legalActionIds[j],
				legalActionTypes[j],
				tritonResponseData.policies[legalActionIds[j]],
				copyBoard.boardRows,
				copyBoard.allInstanceCardInfo);
			newAddNodes.Add(newNode);
		}
	}

	tritonResponseData.curMctsTreeNode->UpdateQValueRecursive(tritonResponseData.boardValue);

	receivedTritonResponse = false;
	return true;
}

void UMcts::GetTritonAction(int32& actionId, ActionType& outActionType, TArray<float>& softmaxProbs)
{
	// After expand nodes, we should find ideal motion and do action
	TArray<float> logVisits;
	TArray<ActionType> actionTypes;
	TArray<int32> candidateActs;
	for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = treeRoot->children.CreateConstIterator(); iter; ++iter)
	{
		float logVisit = FMath::Loge((double)(iter->Value->visit) + 1e-10);
		candidateActs.Add(iter->Key);
		logVisits.Add(logVisit);
		actionTypes.Add(iter->Value->actionType);
	}
	UCoreGameBlueprintFunctionLibrary::Softmax(logVisits, 0.001, softmaxProbs);

	if (isTraining)
	{
		
		int32 targetMove;
		ActionType targetActionType;
		UCoreGameBlueprintFunctionLibrary::GetDirichletAction(candidateActs, actionTypes, softmaxProbs, targetMove, targetActionType);
		UpdateCurSearchNode(targetMove);
		actionId = targetMove;
		outActionType = targetActionType;
	}
	else
	{
		int32 targetMove;
		ActionType targetActionType;
		UCoreGameBlueprintFunctionLibrary::GetDirichletAction(candidateActs, actionTypes, softmaxProbs, targetMove, targetActionType);
		// reset search tree in real battle case
		UpdateCurSearchNode(-1);
		actionId = targetMove;
	}
}

void UMcts::SaveTrainingData(const TArray<FTrainingData>& trainingDatas)
{
	FString saveDir = FPaths::ProjectSavedDir() / TEXT("TrainingData.bin");

	TArray<uint8> binaryData;
	FMemoryWriter writer(binaryData);

	int32 numDatas = trainingDatas.Num();
	writer << numDatas;

	for (const FTrainingData& data : trainingDatas)
	{
		for (int32 i = 0; i < TotalCHW; i++)
		{
			int32 stateCoding = data.stateCoding[i];
			writer << stateCoding;
		}

		int32 actionProbsSize = data.actionProbs.Num();
		writer << actionProbsSize;
		for (int32 i = 0; i < actionProbsSize; i++)
		{
			float actionProb = data.actionProbs[i];
			writer << actionProb;
		}

		int32 scoreSize = data.scores.Num();
		writer << scoreSize;
		for (int32 i = 0; i < scoreSize; i++)
		{
			float score = data.scores[i];
			writer << score;
		}
	}

	FFileHelper::SaveArrayToFile(binaryData, *saveDir);
}
