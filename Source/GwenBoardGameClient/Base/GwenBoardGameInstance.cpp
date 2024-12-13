// Fill out your copyright notice in the Description page of Project Settings.


#include "GwenBoardGameInstance.h"
#include "UnLua.h"
#include "SkynetRunnable.h"
#include "../Game/AI/AIRunnable.h"

void UGwenBoardGameInstance::Init()
{
				//ConnectServer("192.168.19.57", 5678, 1);
				TriggerReadCardInfos();
}

void UGwenBoardGameInstance::Shutdown()
{
				TArray<int32> keys;
				workers.GetKeys(keys);

				for (auto& key : keys)
				{
								Disconnect(key);
				}
}

void UGwenBoardGameInstance::ConnectServer(const FString& ipAddr, int32 inPort, int32 connectionId)
{
				TWeakObjectPtr<UGwenBoardGameInstance> weakObjPtr = TWeakObjectPtr<UGwenBoardGameInstance>(this);
				TSharedRef<FSkynetRunnable> worker(new FSkynetRunnable(ipAddr,
								inPort,
								connectionId,
								receiveBufferSize,
								sendBufferSize,
								timeBetweenTicks,
								weakObjPtr));
				workers.Add(connectionId, worker);
				worker->Start();
}

void UGwenBoardGameInstance::Disconnect(int32 id)
{
				auto worker = workers.Find(id);
				if (worker)
				{
								worker->Get().Stop();
								workers.Remove(id);
				}
}

bool UGwenBoardGameInstance::Connected(int32 workerId)
{
				if (workers.Contains(workerId))
				{
								return workers[workerId]->Connected();
				}
				return false;
}

bool UGwenBoardGameInstance::SendData(int32 workerId, TArray<uint8> dataSend)
{
				if (workers.Contains(workerId))
				{
								if (workers[workerId]->Connected())
								{
												workers[workerId]->AddToOutbox(dataSend);
												return true;
								}
				}
				return false;
}

void UGwenBoardGameInstance::CallLuaRead()
{

				UnLua::FLuaEnv luaEnv;

				const auto Require = UnLua::FLuaFunction(&luaEnv, "_G", "require");
				const auto RetValues = Require.Call("BP_GwenBoardGI_Lua");
				//const auto successFlag = luaEnv.DoString("G_BP_GwenBoardGI_Lua = require 'BP_GwenBoardGI_Lua'");
				//check(successFlag);

				//UnLua::CallTableFunc(luaEnv.GetMainState(), "G_BP_GwenBoardGI_Lua", "SecondThreadCallLua");
				const auto RetValue = RetValues[0];
				const auto LuaTable = UnLua::FLuaTable(&luaEnv, RetValue);
				const auto RetValues2 = LuaTable.Call("SecondThreadCallLua");
}

FString UGwenBoardGameInstance::Message_ReadString(TArray<uint8>& message, int32 bytesLength)
{
				if (bytesLength <= 0)
				{
								return FString("");
				}
				if (message.Num() < bytesLength)
				{
								return FString("");
				}

				TArray<uint8> stringAsArray;
				stringAsArray.Reserve(bytesLength);

				for (int i = 0; i < bytesLength; i++)
				{
								stringAsArray.Add(message[0]);
								message.RemoveAt(0);
				}

				std::string cstr(reinterpret_cast<const char*>(stringAsArray.GetData()), stringAsArray.Num());
				return FString(UTF8_TO_TCHAR(cstr.c_str()));
}

TArray<uint8> UGwenBoardGameInstance::Conv_StringToBytes(const FString& inStr)
{
				FTCHARToUTF8 convert(*inStr);
				int bytesLength = convert.Length(); //length of the utf-8 string in bytes (when non-latin letters are used, it's longer than just the number of characters)
				uint8* messageBytes = static_cast<uint8*>(FMemory::Malloc(bytesLength));
				FMemory::Memcpy(messageBytes, (uint8*)TCHAR_TO_UTF8(inStr.GetCharArray().GetData()), bytesLength); //mcmpy is required, since TCHAR_TO_UTF8 returns an object with a very short lifetime

				TArray<uint8> result;
				for (int i = 0; i < bytesLength; i++)
				{
								result.Add(messageBytes[i]);
				}

				FMemory::Free(messageBytes);

				return result;
}

void UGwenBoardGameInstance::ExecuteOnConnected(int32 workerId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj)
{

}

void UGwenBoardGameInstance::ExecuteOnDisconnected(int32 workerId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj)
{

}

void UGwenBoardGameInstance::ExecuteOnMessageReceived(int32 connectionId, TWeakObjectPtr<UGwenBoardGameInstance> thisObj)
{
				if (!thisObj.IsValid())
				{
								return;
				}

				if (!workers.Contains(connectionId))
				{
								return;
				}

				TArray<uint8> msg = workers[connectionId]->ReadFromInbox();
}

void UGwenBoardGameInstance::TriggerReadCardInfos_Implementation()
{

}

void UGwenBoardGameInstance::SetCardInfo(FString cardName,
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
				int32 initCoolDown,
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
				FString cardCategory)
{
				FCardInfo cardInfo;
				cardInfo.cardName = cardName;
				cardInfo.aliasCN = aliasCN;
				cardInfo.aliasEN = aliasEN;
				cardInfo.cardFunctionCN = cardFunctionCN;
				cardInfo.cardFunctionEN = cardFunctionEN;
				cardInfo.despCN = despCN;
				cardInfo.despEN = despEN;
				cardInfo.hp = hp;
				cardInfo.defence = defence;
				cardInfo.agility = agility;
				cardInfo.cardTags = cardTags;
				cardInfo.launchType = launchType;
				cardInfo.coolDown = initCoolDown;
				cardInfo.coolDown = coolDown;
				cardInfo.availableTimes = availableTimes;
				cardInfo.launchGeoType = launchGeoType;
				cardInfo.autoSkillTargetGeoType = autoSkillTargetGeoType;
				cardInfo.targetGeoType = targetGeoType;
				cardInfo.aoeType = aoeType;
				cardInfo.targetCamp = targetCamp;
				cardInfo.effectType = effectType;
				cardInfo.effectAffix = effectAffix;
				cardInfo.effectAffixCamp = effectAffixCamp;
				cardInfo.prereqTagCondition = prereqTagCondition;
				cardInfo.prereqTag = prereqTag;
				cardInfo.prereqCampType = prereqCampType;
				cardInfo.prereqType = prereqType;
				cardInfo.passivePrereqType = passivePrereqType;
				cardInfo.values = values;
				cardInfo.texture = texture;
				cardInfo.moveType = moveType;
				cardInfo.cardCategory = cardCategory;
				allCardInfos.Add(cardName, cardInfo);
}


