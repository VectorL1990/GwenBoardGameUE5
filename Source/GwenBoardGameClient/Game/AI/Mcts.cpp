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



void UMcts::InitMcts()
{

	veryFirstNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	tritonHttpClient = NewObject<UTritonHttpClient>(GetWorld(), tritonHttpClientBPClass);
	tritonHttpClient->InitTritonClient(this);
	treeRoot = veryFirstNode;


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

void UMcts::ResetMcts()
{
	realBoard.ResetBoard();
	curSimulationMove = 0;
	curTritonRequestID = 0;

	veryFirstNode = NewObject<UMctsTreeNode>(GetWorld(), mctsTreeNodeBPClass);
	treeRoot = veryFirstNode;


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
			/*
			coreCardGameMode->SpawnHandCard(instanceCardInfo.originCardInfo.cardName,
				0, generateCardId,
				instanceCardInfo.originCardInfo.hp,
				instanceCardInfo.originCardInfo.defence,
				i);
			*/
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
			/*
			coreCardGameMode->SpawnHandCard(instanceCardInfo.originCardInfo.cardName,
				1, generateCardId,
				instanceCardInfo.originCardInfo.hp,
				instanceCardInfo.originCardInfo.defence, i);
			*/
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
		//treeRoot->parent = NULL;
	}
	else
	{
		treeRoot->ResetNode();
	}
}

void UMcts::SendTritonRequest()
{
	UMctsTreeNode* curSearchNode = treeRoot;

	FBoardInfo copyBoard = realBoard.GetCopyBoard();
	while (true)
	{
		if (curSearchNode == NULL || curSearchNode->IsLeaf())
		{
			break;
		}
		int32 action = 0;
		curSearchNode = curSearchNode->Select(action, isTraining);
		int32 launchX = 0;
		int32 launchY = 0;
		int32 targetX = 0;
		int32 targetY = 0;
		ActionType actionType = ActionType::EndRound;
		copyBoard.ActionDecoding(action, launchX, launchY, targetX, targetY, actionType);
		// we should do move here! So that we can predict next action probs
		TArray<FRenderEffectRound> renderEffectRoundList;
		copyBoard.TriggerAction(false, copyBoard.curPlayingSectionNb, action, renderEffectRoundList);
	}

	copyBoard.StateCoding(copyBoard.curPlayingSectionNb, curSearchNode->stateCoding);

	int32 requestID = GetCurTritonRequestID();
	tritonResponseData.curBoardInfo = copyBoard;
	tritonResponseData.curMctsTreeNode = curSearchNode;
	tritonHttpClient->SendInferenceRequest("GwenNetModel", curSearchNode->stateCoding, StateCodingTotalCHW, requestID);
}

void UMcts::SendTestTritonRequest()
{
	int32 stateCoding[StateCodingTotalCHW] = { 0 };
	realBoard.StateCoding(realBoard.curPlayingSectionNb, stateCoding);
	SaveTestStateData(stateCoding);
	int32 requestID = GetCurTritonRequestID();
	tritonHttpClient->SendInferenceRequest("GwenNetModel", stateCoding, StateCodingTotalCHW, requestID);
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
			if (winner == tritonResponseData.curBoardInfo.curPlayingSectionNb)
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
		tritonResponseData.curBoardInfo.GetLegalMoves(tritonResponseData.curBoardInfo.curPlayingSectionNb, legalActionIds, legalActionTypes);

		for (int32 j = 0; j < legalActionIds.Num(); j++)
		{
			TArray<FRenderEffectRound> renderEffectRoundList;
			// Trigger action just for replay
			FBoardInfo copyBoard = tritonResponseData.curBoardInfo.GetCopyBoard();

			copyBoard.TriggerAction(false, tritonResponseData.curBoardInfo.curPlayingSectionNb,
				legalActionIds[j], renderEffectRoundList);

			float expPolicy = exp(tritonResponseData.policies[legalActionIds[j]]);
			UMctsTreeNode* newNode = tritonResponseData.curMctsTreeNode->ExpandNode(
				tritonResponseData.curMctsTreeNode->hirachy,
				legalActionIds[j],
				legalActionTypes[j],
				tritonResponseData.curBoardInfo.curPlayingSectionNb,
				expPolicy,
				copyBoard.boardRows,
				copyBoard.allInstanceCardInfo);
			newNode->sectionZeroScore = copyBoard.sectionZeroScores;
			newNode->sectionOneScore = copyBoard.sectionOneScores;
			newAddNodes.Add(newNode);
		}
	}

	tritonResponseData.curMctsTreeNode->UpdateQValueRecursive(tritonResponseData.curMctsTreeNode->actionId,
		tritonResponseData.curMctsTreeNode->curPlayingSectionNb,
		tritonResponseData.curMctsTreeNode->hirachy,
		tritonResponseData.boardValue,
		isTraining);

	receivedTritonResponse = false;
	return true;
}

void UMcts::GetTritonAction(
	int32& actionId, 
	ActionType& outActionType,
	uint8& outSectionNb,
	TMap<int32, float>& trainDataActionProbsMap)
{
	// After expand nodes, we should find ideal motion and do action
	TArray<float> logVisits;
	TArray<ActionType> actionTypes;
	TArray<int32> candidateActs;
	TArray<float> softmaxProbs;
	float temp = 0.001;
	for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = treeRoot->children.CreateConstIterator(); iter; ++iter)
	{
		float logVisit = 1.0 / temp * FMath::Loge((double)(iter->Value->visit) + 1e-10);
		candidateActs.Add(iter->Key);
		logVisits.Add(logVisit);
		actionTypes.Add(iter->Value->actionType);
	}
	UCoreGameBlueprintFunctionLibrary::Softmax(logVisits, softmaxProbs);
	for (int32 i = 0; i < candidateActs.Num(); i++)
	{
		trainDataActionProbsMap.Add(candidateActs[i], softmaxProbs[i]);
	}

	if (isTraining)
	{
		TArray<float> copyProbs = softmaxProbs;
		copyProbs.Sort(TGreater<float>());
		TArray<int32> finalCandidateActs;
		TArray<ActionType> finalActionTypes;
		if (copyProbs.Num() >= 10)
		{
			float thresholdProb = copyProbs[9];
			for (int32 i = 0; i < softmaxProbs.Num(); i++)
			{
				if (softmaxProbs[i] >= thresholdProb)
				{
					finalCandidateActs.Add(candidateActs[i]);
					finalActionTypes.Add(actionTypes[i]);
				}
			}
		}
		else
		{
			finalCandidateActs = candidateActs;
			finalActionTypes = actionTypes;
		}

		int32 randActNb = FMath::RandRange(0, finalCandidateActs.Num() - 1);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "select rand act nb: " + FString::FromInt(randActNb));
		
		int32 targetMove = finalCandidateActs[randActNb];
		outSectionNb = treeRoot->children[targetMove]->curPlayingSectionNb;
		UpdateCurSearchNode(targetMove);
		actionId = targetMove;
		outActionType = finalActionTypes[randActNb];
		/*
		int32 targetMove;
		ActionType targetActionType;
		UCoreGameBlueprintFunctionLibrary::GetDirichletAction(candidateActs, actionTypes, softmaxProbs, targetMove, targetActionType);
		outSectionNb = treeRoot->children[targetMove]->curPlayingSectionNb;
		UpdateCurSearchNode(targetMove);
		actionId = targetMove;
		outActionType = targetActionType;
		*/
	}
	else
	{
		int32 maxSoftmaxProbNb = 0;
		float maxSoftmaxProb = -FLT_MAX;
		for (int32 i = 0; i < softmaxProbs.Num(); i++)
		{
			if (softmaxProbs[i] > maxSoftmaxProb)
			{
				maxSoftmaxProb = softmaxProbs[i];
				maxSoftmaxProbNb = i;
			}
		}
		int32 targetMove = candidateActs[maxSoftmaxProbNb];
		outSectionNb = treeRoot->children[targetMove]->curPlayingSectionNb;
		UpdateCurSearchNode(targetMove);
		actionId = targetMove;
		outActionType = actionTypes[maxSoftmaxProbNb];
		/*
		int32 targetMove;
		ActionType targetActionType;
		UCoreGameBlueprintFunctionLibrary::GetDirichletAction(candidateActs, actionTypes, softmaxProbs, targetMove, targetActionType);
		outSectionNb = treeRoot->children[targetMove]->curPlayingSectionNb;
		// reset search tree in real battle case
		UpdateCurSearchNode(-1);
		actionId = targetMove;
		*/
	}
	
}

void UMcts::SaveTestStateData(int32* stateCoding)
{
	FString stateCodingSaveDir = FPaths::ProjectSavedDir() / TEXT("TestCaseStateCoding.bin");

	TArray<uint8> stateCodingBinaryData;
	FMemoryWriter stateCodingWriter(stateCodingBinaryData);

	for (int32 i = 0; i < StateCodingTotalCHW; i++)
	{
		int32 coding = stateCoding[i];
		stateCodingWriter << coding;
	}

	FFileHelper::SaveArrayToFile(stateCodingBinaryData, *stateCodingSaveDir);
}

void UMcts::SaveTrainingData(const TArray<FTrainingData>& trainingDatas,
	int32 channelNb,
	int32 height,
	int32 width)
{
	FString stateCodingSaveDir = FPaths::ProjectSavedDir() / TEXT("StateCoding.bin");
	FString actionProbsSaveDir = FPaths::ProjectSavedDir() / TEXT("ActionProbs.bin");
	FString winScoreSaveDir = FPaths::ProjectSavedDir() / TEXT("WinScores.bin");

	TArray<uint8> stateCodingBinaryData;
	FMemoryWriter stateCodingWriter(stateCodingBinaryData);

	TArray<uint8> actionProbsBinrayData;
	FMemoryWriter actionProbsWriter(actionProbsBinrayData);

	TArray<uint8> winScoreBinaryData;
	FMemoryWriter winScoreWriter(winScoreBinaryData);

	int32 gameNb = trainingDatas.Num();
	stateCodingWriter << gameNb;
	stateCodingWriter << channelNb;
	stateCodingWriter << height;
	stateCodingWriter << width;
	actionProbsWriter << gameNb;
	winScoreWriter << gameNb;
	for (int32 i = 0; i < trainingDatas.Num(); i++)
	{
		int32 stepNb = trainingDatas[i].stateCodingAndActionProbs.Num();
		stateCodingWriter << stepNb;
		actionProbsWriter << stepNb;
		for (int32 j = 0; j < trainingDatas[i].stateCodingAndActionProbs.Num(); j++)
		{
			for (int32 k = 0; k < StateCodingTotalCHW; k++)
			{
				int32 coding = trainingDatas[i].stateCodingAndActionProbs[j].stateCoding[k];
				stateCodingWriter << coding;
			}

			int32 actionProbsNb = trainingDatas[i].stateCodingAndActionProbs[j].actionProbs.Num();
			actionProbsWriter << actionProbsNb;
			for (TMap<int32, float>::TConstIterator iter = trainingDatas[i].stateCodingAndActionProbs[j].actionProbs.CreateConstIterator(); iter; ++iter)
			{
				int32 actionId = iter->Key;
				actionProbsWriter << actionId;
				float actionProb = iter->Value;
				actionProbsWriter << actionProb;
			}
		}

		winScoreWriter << stepNb;
		for (int32 j = 0; j < trainingDatas[i].scores.Num(); j++)
		{
			float score = trainingDatas[i].scores[j];
			winScoreWriter << score;
		}
	}


	FFileHelper::SaveArrayToFile(stateCodingBinaryData, *stateCodingSaveDir);
	FFileHelper::SaveArrayToFile(actionProbsBinrayData, *actionProbsSaveDir);
	FFileHelper::SaveArrayToFile(winScoreBinaryData, *winScoreSaveDir);
}

void UMcts::AddTrainingData(const FTrainingData& trainingData,
	int32 channelNb,
	int32 height,
	int32 width)
{
	FString stateCodingSaveDir = FPaths::ProjectSavedDir() / TEXT("StateCoding.bin");
	FString actionProbsSaveDir = FPaths::ProjectSavedDir() / TEXT("ActionProbs.bin");
	FString winScoreSaveDir = FPaths::ProjectSavedDir() / TEXT("WinScores.bin");



	// 获取当前文件大小（如果文件存在）
	int32 existingStateCodingSize = 0;
	int32 existingActionProbsSize = 0;
	int32 existingWinScoreSize = 0;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*stateCodingSaveDir))
	{
		existingStateCodingSize = PlatformFile.FileSize(*stateCodingSaveDir);
	}
	if (PlatformFile.FileExists(*actionProbsSaveDir))
	{
		existingActionProbsSize = PlatformFile.FileSize(*actionProbsSaveDir);
	}
	if (PlatformFile.FileExists(*winScoreSaveDir))
	{
		existingWinScoreSize = PlatformFile.FileSize(*winScoreSaveDir);
	}

	bool appendMode = false;
	TUniquePtr<FArchive> stateCodingArchive;
	TUniquePtr<FArchive> actionProbsArchive;
	TUniquePtr<FArchive> winScoreArchive;

	if (existingStateCodingSize > 0)
	{
		appendMode = true;
		stateCodingArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*stateCodingSaveDir, FILEWRITE_Append));
	}
	else
	{
		stateCodingArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*stateCodingSaveDir));
	}

	if (existingActionProbsSize > 0)
	{
		appendMode = true;
		actionProbsArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*actionProbsSaveDir, FILEWRITE_Append));
	}
	else
	{
		actionProbsArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*actionProbsSaveDir));
	}

	if (existingWinScoreSize > 0)
	{
		appendMode = true;
		winScoreArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*winScoreSaveDir, FILEWRITE_Append));
	}
	else
	{
		winScoreArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*winScoreSaveDir));
	}

	// 写入文件头（仅当创建新文件时）
	if (!appendMode)
	{
		*stateCodingArchive << channelNb;
		*stateCodingArchive << height;
		*stateCodingArchive << width;
	}





	int32 stepNb = trainingData.stateCodingAndActionProbs.Num();
	*stateCodingArchive << stepNb;

	for (int32 j = 0; j < trainingData.stateCodingAndActionProbs.Num(); j++)
	{
		for (int32 k = 0; k < StateCodingTotalCHW; k++)
		{
			int32 coding = trainingData.stateCodingAndActionProbs[j].stateCoding[k];
			*stateCodingArchive << coding;
		}
	}

	// 写入动作概率
	*actionProbsArchive << stepNb;

	for (int32 j = 0; j < trainingData.stateCodingAndActionProbs.Num(); j++)
	{
		int32 actionProbsNb = trainingData.stateCodingAndActionProbs[j].actionProbs.Num();
		*actionProbsArchive << actionProbsNb;

		for (TMap<int32, float>::TConstIterator iter = trainingData.stateCodingAndActionProbs[j].actionProbs.CreateConstIterator(); iter; ++iter)
		{
			int32 actionId = iter->Key;
			float actionProb = iter->Value;
			*actionProbsArchive << actionId;
			*actionProbsArchive << actionProb;
		}
	}

	// 写入得分
	*winScoreArchive << stepNb;

	for (int32 j = 0; j < trainingData.scores.Num(); j++)
	{
		float score = trainingData.scores[j];
		*winScoreArchive << score;
	}

	// 关闭文件（智能指针会自动关闭，但显式关闭更安全）
	stateCodingArchive->Close();
	actionProbsArchive->Close();
	winScoreArchive->Close();
}

void UMcts::ClearTree(UMctsTreeNode* curTreeNode)
{
	if (curTreeNode)
	{
		if (curTreeNode->children.Num() > 0)
		{
			for (TMap<int32, UMctsTreeNode*>::TConstIterator iter = curTreeNode->children.CreateConstIterator(); iter; ++iter)
			{
				if (iter->Value->IsValidLowLevel())
				{
					ClearTree(iter->Value);
				}
			}
			curTreeNode->children.Empty();
		}
		curTreeNode->ConditionalBeginDestroy();
	}
}
