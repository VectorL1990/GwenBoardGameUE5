// Fill out your copyright notice in the Description page of Project Settings.

#include "LibLuasocket.h"

//#include "lua.hpp"

extern "C" {
#include "luasocket.h"
#include "mime.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

IMPLEMENT_MODULE(FLibLuasocketModule, LibLuasocket);

void FLibLuasocketModule::StartupModule()
{

}

void FLibLuasocketModule::ShutdownModule()
{

}

void FLibLuasocketModule::SetupLuasocket(struct lua_State* L)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);

	luaL_requiref(L, "socket.core", luaopen_socket_core, 1);
	luaL_requiref(L, "mime.core", luaopen_mime_core, 1);
	luaL_requiref(L, "socket", &FLibLuasocketModule::OpenLuaSocketSocket, 1);
	luaL_requiref(L, "ltn12", &FLibLuasocketModule::OpenLuaSocketLtn12, 1);
	luaL_requiref(L, "mime", &FLibLuasocketModule::OpenLuaSocketMime, 1);

	luaL_requiref(L, "socket.url", &FLibLuasocketModule::OpenLuaSocketUrl, 1);
	luaL_requiref(L, "socket.tp", &FLibLuasocketModule::OpenLuaSocketTp, 1);
	luaL_requiref(L, "socket.ftp", &FLibLuasocketModule::OpenLuaSocketFtp, 1);
	luaL_requiref(L, "socket.headers", &FLibLuasocketModule::OpenLuaSocketHeaders, 1);
	luaL_requiref(L, "socket.http", &FLibLuasocketModule::OpenLuaSocketHttp, 1);
	luaL_requiref(L, "mbox", &FLibLuasocketModule::OpenLuaSocketMbox, 1);
	luaL_requiref(L, "socket.smtp", &FLibLuasocketModule::OpenLuaSocketSmtp, 1);
	lua_pop(L, 1);
}

int FLibLuasocketModule::OpenLuaSocketFtp(lua_State* L)
{
	static const auto RawLua =
#include "ftp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketHeaders(lua_State* L)
{
	static const auto RawLua =
#include "headers.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketHttp(lua_State* L)
{
	static const auto RawLua =
#include "http.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketLtn12(lua_State* L)
{
	static const auto RawLua =
#include "ltn12.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketMbox(lua_State* L)
{
	static const auto RawLua =
#include "mbox.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketMime(lua_State* L)
{
	static const auto RawLua =
#include "mime.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketSmtp(lua_State* L)
{
	static const auto RawLua =
#include "smtp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketSocket(lua_State* L)
{
	static const auto RawLua =
#include "socket.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketTp(lua_State* L)
{
	static const auto RawLua =
#include "tp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketUrl(lua_State* L)
{
	static const auto RawLua =
#include "url.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}
