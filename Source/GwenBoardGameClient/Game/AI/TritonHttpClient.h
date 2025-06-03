// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "TritonHttpClient.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GWENBOARDGAMECLIENT_API UTritonHttpClient : public UObject
{
	GENERATED_BODY()
public:
	void InitTritonClient(UObject* inMcts);
	
	void SendInferenceRequest(const FString& modelName, const int32* inputData, int32 inputDataSize, int32 requestId);

	void SaveRequestData(const TArray<uint8>& inputData);

	void ProcessResponse(const int32& requestId, const int32& metaLen, const TArray<uint8>& responseData);

	UPROPERTY()
	UObject* mcts;
};
