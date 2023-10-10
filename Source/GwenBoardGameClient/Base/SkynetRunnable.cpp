// Fill out your copyright notice in the Description page of Project Settings.


#include "SkynetRunnable.h"
#include "GwenBoardGameInstance.h"

FSkynetRunnable* FSkynetRunnable::skynetRunnable = nullptr;

FSkynetRunnable::FSkynetRunnable(UGwenBoardGameInstance* gameInstance)
				:stopTaskCounter(0)
{
				gwenGI = gameInstance;
				skynetRunnableThread = FRunnableThread::Create(this, TEXT("SkynetRunnable"));
}

FSkynetRunnable::~FSkynetRunnable()
{
				delete skynetRunnableThread;
				skynetRunnableThread = nullptr;
}

FSkynetRunnable* FSkynetRunnable::JoyInit()
{
				/*
				if (!skynetRunnable && FPlatformProcess::SupportsMultithreading())
				{
								skynetRunnable = new FSkynetRunnable();
				}
				return skynetRunnable;*/
				return nullptr;
}

void FSkynetRunnable::EnsureCompletion()
{
				Stop();
				skynetRunnableThread->WaitForCompletion();
}

void FSkynetRunnable::Shutdown()
{
				if (skynetRunnable)
				{
								skynetRunnable->EnsureCompletion();
								delete skynetRunnable;
								skynetRunnable = nullptr;
				}
}

bool FSkynetRunnable::Init()
{
				
				return true;
}

uint32 FSkynetRunnable::Run()
{
				while (true)
				{
								if (gwenGI)
								{
												gwenGI->CallLuaRead();
												FPlatformProcess::Sleep(0.5);
								}
				}
				return 0;
}

void FSkynetRunnable::Stop()
{
				stopTaskCounter.Increment();
}

void FSkynetRunnable::Exit()
{

}

