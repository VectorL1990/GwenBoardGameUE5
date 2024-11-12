// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Game/GlobalConstFunctionLibrary.h"
#include "GwenBoardGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UGwenBoardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:

				virtual void Init() override;

				virtual void Shutdown() override;

				UFUNCTION(BlueprintCallable)
				void ConnectServer(const FString& ipAddr, int32 inPort, int32 connectionId);

				void Disconnect(int32 id);

				bool Connected(int32 workerId);

				bool SendData(int32 workerId, TArray<uint8> dataSend);

				void CallLuaRead();

				FString Message_ReadString(UPARAM(ref) TArray<uint8>& message, int32 stringLength);

				TArray<uint8> Conv_StringToBytes(const FString& inStr);

				void ExecuteOnConnected(int32 workerId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj);

				void ExecuteOnDisconnected(int32 workerId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj);

				void ExecuteOnMessageReceived(int32 connectionId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj);

    UFUNCTION(BlueprintCallable)
        void SetCardInfo(FString cardName,
            FString aliasCN,
            FString aliasEN,
            FString cardFunctionCN,
            FString cardFunctionEN,
            FString despCN,
            FString despEN,
            int32 hp,
            int32 defence,
            int32 agility,
            TArray<FString> cardTags,
            FString launchType,
            int32 coolDown,
            int32 availableTimes,
            FString launchGeoType,
            FString autoSkillTargetGeoType,
            FString targetGeoType,
            FString aoeType,
            FString targetCamp,
            FString effectType,
            FString effectAffix,
            FString effectAffixCamp,
            FString prereqTagCondition,
            FString prereqTag,
            FString prereqCampType,
            FString prereqType,
            FString passivePrereqType,
            TArray<int32> values,
            UTexture* texture,
            FString moveType,
            FString cardCategory);

    UFUNCTION(BlueprintNativeEvent)
    void TriggerReadCardInfos();

				FRunnableThread* skynetRunnableThread;

				TMap<int32, TSharedRef<class FSkynetRunnable>> workers;

				int32 sendBufferSize = 16384;

				/* Buffer size in bytes. It's set only when creating a socket, never afterwards. */
				int32 receiveBufferSize = 16384;

				/* Time between ticks. Please account for the fact that it takes 1ms to wake up on a modern PC, so 0.01f would effectively be 0.011f */
				float timeBetweenTicks = 0.008f;


				UPROPERTY()
				TMap<FString, FCardInfo> allCardInfos;

				FString curRoomKey;

    UPROPERTY(EditAnywhere)
				TArray<FString> selectCardList;

    UPROPERTY(EditAnywhere)
    TArray<FString> sectionOneSelectCardList;
};
