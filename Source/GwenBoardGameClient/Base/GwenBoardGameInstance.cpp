// Fill out your copyright notice in the Description page of Project Settings.


#include "GwenBoardGameInstance.h"
#include "SkynetRunnable.h"
#include "UnLua.h"

void UGwenBoardGameInstance::StartSkynetClientThread()
{
				if (!skynetRunnableThread)
				{
								skynetRunnableThread = FRunnableThread::Create(new FSkynetRunnable(this),
												TEXT("SkynetRunnableThread"),
												0,
												TPri_BelowNormal);
				}
}

void UGwenBoardGameInstance::CallLuaRead()
{

				UnLua::FLuaEnv luaEnv;

				const auto successFlag = luaEnv.DoString("G_BP_GwenBoardGI_Lua = require 'BP_GwenBoardGI_Lua'");
				check(successFlag);

				UnLua::CallTableFunc(luaEnv.GetMainState(), "G_BP_GwenBoardGI_Lua", "TestCallLua");
}
