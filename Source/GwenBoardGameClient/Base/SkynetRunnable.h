// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
//#include "SkynetRunnable.generated.h"


class UGwenBoardGameInstance;
/**
 * 
 */
//UCLASS()
class GWENBOARDGAMECLIENT_API FSkynetRunnable : public FRunnable
{
public:
    FSkynetRunnable(UGwenBoardGameInstance* gameInstance);
    ~FSkynetRunnable();

    static FSkynetRunnable* skynetRunnable;
    static FSkynetRunnable* JoyInit();

    static void Shutdown();

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    void EnsureCompletion();

    class FRunnableThread* skynetRunnableThread;
    FThreadSafeCounter stopTaskCounter;
    
    UGwenBoardGameInstance* gwenGI;
};
