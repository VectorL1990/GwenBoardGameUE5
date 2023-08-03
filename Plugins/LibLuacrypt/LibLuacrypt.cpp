// Fill out your copyright notice in the Description page of Project Settings.
#include "LibLuacrypt.h"

#include "lua.hpp"
#include "lsha1.c"
#include "lua-crypt.c"

extern "C" {

}

IMPLEMENT_MODULE(FLibLuacryptModule, LibLuacrypt);

void FLibLuacryptModule::StartupModule()
{

}

void FLibLuacryptModule::ShutdownModule()
{

}

void FLibLuacryptModule::SetupLuacrypt(lua_State* L)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
		 
	luaL_requiref(L, "crypt", luaopen_crypt, 1);
}

