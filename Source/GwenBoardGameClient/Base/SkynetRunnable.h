// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "Containers/Queue.h"
//#include "SkynetRunnable.generated.h"


class UGwenBoardGameInstance;
/**
 * 
 */
//UCLASS()
class GWENBOARDGAMECLIENT_API FSkynetRunnable : public FRunnable, public TSharedFromThis<FSkynetRunnable>
{
private:
    class FRunnableThread* skynetRunnableThread = nullptr;
    class FSocket* socket = nullptr;
    FString ipAddr;
    int port;
    FThreadSafeBool running = false;
    FThreadSafeBool connected = false;
    int32 runnableId;
    int32 recvBufferSize;
    int32 sendBufferSize;
    int32 actualRecvBufferSize;
    int32 actualSendBufferSize;
    float timeBetweenTicks;

    TQueue<TArray<uint8>, EQueueMode::Spsc> inBox;
    TQueue<TArray<uint8>, EQueueMode::Spsc> outBox;

    TWeakObjectPtr<UGwenBoardGameInstance> threadOwner;
    
public:
    FSkynetRunnable(FString ip, 
        const int32 port,
        int32 inRunnableId,
        int32 inRecvBufferSize,
        int32 inSendBufferSize,
        float inTimeBetweenTicks,
        TWeakObjectPtr<UGwenBoardGameInstance> inOwner);
    ~FSkynetRunnable();

    void Start();

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    void SocketShutdown();

    bool Connected();

    void AddToOutbox(TArray<uint8> msg);

    TArray<uint8> ReadFromInbox();

    bool BlockingSend(const uint8* data, int32 bytesToSend);
};
