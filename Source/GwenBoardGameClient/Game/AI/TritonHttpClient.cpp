// Fill out your copyright notice in the Description page of Project Settings.


#include "TritonHttpClient.h"
#include "Mcts.h"

void UTritonHttpClient::InitTritonClient(UObject* inMcts)
{
    mcts = inMcts;
}

void UTritonHttpClient::SendInferenceRequest(const FString& modelName, const int32* inputData, int32 inputDataSize, int32 requestId)
{
    FString url = FString::Printf(TEXT("http://localhost:8000/v2/models/%s/infer"), *modelName);
    FHttpModule* httpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = httpModule->CreateRequest();
    httpRequest->SetURL(url);
    httpRequest->SetVerb("POST");
    httpRequest->SetHeader("Content-Type", "application/octet-stream");
    httpRequest->SetHeader(TEXT("X-Request-ID"), FString::FromInt(requestId));

    TArray<uint8> binaryData;
    const uint8* bytes = reinterpret_cast<const uint8*>(inputData);
    int32 byteSize = inputDataSize * sizeof(int32);
    binaryData.Append(bytes, byteSize);

    httpRequest->SetContent(binaryData);
    httpRequest->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr request, FHttpResponsePtr response, bool bConnectedSuccessfully)
        {
            if (bConnectedSuccessfully && response.IsValid())
            {
                const FString receivedId = request->GetHeader("X-Request-ID");
                const TArray<uint8>& content = response->GetContent();
                ProcessResponse(FCString::Atoi(*receivedId), content);
            }
        }
    );

    httpRequest->ProcessRequest();
}

void UTritonHttpClient::ProcessResponse(const int32& requestId, const TArray<uint8>& responseData)
{
    /*
    const float* outputBuffer = reinterpret_cast<const float*>(responseData.GetData());
    TArray<float> policyOutput(outputBuffer, 5056);
    TArray<float> valueOutput(outputBuffer + 5056, 1);

    UMcts* castMcts = Cast<UMcts>(mcts);
    FTritonResponseData receivedData;
    receivedData.policy = policyOutput;
    receivedData.boardValue = valueOutput.Last();
    int32 responseInferenceDataNb = castMcts->inferenceIdWaitResponseNbMap[requestId];
    castMcts->receivedResponseInferenceData[responseInferenceDataNb] = receivedData;
    castMcts->receivedTritonResponseDataNb += 1;
    */
}

