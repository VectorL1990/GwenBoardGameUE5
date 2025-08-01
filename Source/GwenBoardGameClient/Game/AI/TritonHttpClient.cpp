// Fill out your copyright notice in the Description page of Project Settings.


#include "TritonHttpClient.h"
#include "Mcts.h"

void UTritonHttpClient::InitTritonClient(UObject* inMcts)
{
    mcts = inMcts;
}

void UTritonHttpClient::SaveRequestData(const TArray<uint8>& inputData)
{
    FString saveDir = FPaths::ProjectSavedDir() / TEXT("TritonRequests");
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!platformFile.DirectoryExists(*saveDir))
    {
        platformFile.CreateDirectory(*saveDir);
    }

    FString timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    FString filename = FString::Printf(TEXT("Request_%s.bin"), *timestamp);
    FString fullPath = saveDir / filename;

    FFileHelper::SaveArrayToFile(inputData, *fullPath);
}

void UTritonHttpClient::SendInferenceRequest(const FString& modelName, const int32* inputData, int32 inputDataSize, int32 requestId)
{
    TArray<float> floatInput;
    floatInput.SetNumUninitialized(inputDataSize);
    for (int32 i = 0; i < inputDataSize; i++) {
        floatInput[i] = static_cast<float>(inputData[i]);
    }

    FString url = FString::Printf(TEXT("http://localhost:8000/v2/models/%s/infer"), *modelName);
    FHttpModule* httpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = httpModule->CreateRequest();
    httpRequest->SetURL(url);
    httpRequest->SetVerb("POST");
    httpRequest->SetHeader("Content-Type", "application/octet-stream");
    httpRequest->SetHeader(TEXT("X-Request-ID"), FString::FromInt(requestId));
    int32 binaryDataByteSize = inputDataSize * sizeof(float);
    FString metadata = FString::Printf(TEXT(R"({
        "inputs": [
            {
                "name": "input_0",
                "shape": [1, 84, 14, 4],
                "datatype": "FP32",
                "parameters":
                {
                    "binary_data_size": %d
                }
            }
        ]
    })"),
    binaryDataByteSize);
    FString metaDataLen = FString::FromInt(metadata.Len());
    httpRequest->SetHeader("Inference-Header-Content-Length", metaDataLen);

    TArray<uint8> binaryData;

    FTCHARToUTF8 metaConverter(*metadata);
    binaryData.Append((uint8*)metaConverter.Get(), metaConverter.Length());

    const uint8* bytes = reinterpret_cast<const uint8*>(floatInput.GetData());
    int32 byteSize = inputDataSize * sizeof(int32);
    binaryData.Append(bytes, byteSize);

    httpRequest->SetContent(binaryData);
    httpRequest->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr request, FHttpResponsePtr response, bool bConnectedSuccessfully)
        {
            if (bConnectedSuccessfully && response.IsValid())
            {
                const FString receivedId = request->GetHeader("X-Request-ID");
                FString metaLengthHeader = request->GetHeader("Inference-Header-Content-Length");
                if (!metaLengthHeader.IsEmpty())
                {
                    int32 metaLength = FCString::Atoi(*metaLengthHeader);
                    const TArray<uint8>& content = response->GetContent();
                    ProcessResponse(FCString::Atoi(*receivedId), metaLength, content);
                }
            }
        }
    );

    httpRequest->ProcessRequest();
}

void UTritonHttpClient::ProcessResponse(const int32& requestId, const int32& metaLen, const TArray<uint8>& responseData)
{
    FString jsonString;
    FUTF8ToTCHAR converter(reinterpret_cast<const ANSICHAR*>(responseData.GetData()), responseData.Num());
    jsonString = FString(converter.Length(), converter.Get());

    TSharedPtr<FJsonObject> jsonObject;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(jsonString);

    if (!FJsonSerializer::Deserialize(reader, jsonObject) || !jsonObject.IsValid())
    {
        return;
    }

    // ����3����ȡoutputs����
    const TArray<TSharedPtr<FJsonValue>>* outputsArray;
    if (!jsonObject->TryGetArrayField("outputs", outputsArray))
    {
        return;
    }

    UMcts* castMcts = Cast<UMcts>(mcts);
    TSharedPtr<FJsonObject> outputObj_0 = (*outputsArray)[0]->AsObject();
    if (!outputObj_0.IsValid())
    {
        return;
    }
    // ����5����ȡ������data����
    const TArray<TSharedPtr<FJsonValue>>* dataArray_0;
    if (outputObj_0->TryGetArrayField("data", dataArray_0))
    {
        // ������������洢���
        for (const TSharedPtr<FJsonValue>& dataValue : *dataArray_0)
        {
            double policy = 0.0;
            if (dataValue->TryGetNumber(policy))
            {
                castMcts->tritonResponseData.policies.Add(static_cast<float>(policy));
                //castMcts->tritonResponseDatas[castMcts->requestIdResponseNbMap[requestId]].policies.Add(static_cast<float>(policy));
            }
            else
            {
                // �������ܵĴ���
                castMcts->tritonResponseData.policies.Add(-10.0);
                //castMcts->tritonResponseDatas[castMcts->requestIdResponseNbMap[requestId]].policies.Add(0.0f);
            }
        }
    }

    TSharedPtr<FJsonObject> outputObj_1 = (*outputsArray)[1]->AsObject();
    if (!outputObj_1.IsValid())
    {
        return;
    }
    const TArray<TSharedPtr<FJsonValue>>* dataArray_1;
    if (outputObj_1->TryGetArrayField("data", dataArray_1))
    {
        double boardVal = 0.0;
        (*dataArray_1)[0]->TryGetNumber(boardVal);
        castMcts->tritonResponseData.boardValue = static_cast<float>(boardVal);
        //castMcts->tritonResponseDatas[castMcts->requestIdResponseNbMap[requestId]].boardValue = static_cast<float>(boardVal);
    }

    castMcts->receivedTritonResponse = true;
}

