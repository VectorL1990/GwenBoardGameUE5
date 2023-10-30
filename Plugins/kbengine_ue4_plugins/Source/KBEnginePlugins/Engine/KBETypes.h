/*
	Generated by KBEngine!
	Please do not modify this file!
	tools = kbcmd
*/

#pragma once

#include "KBECommon.h"

namespace KBEngine
{


// defined in */scripts/entity_defs/types.xml

typedef uint8 UINT8;

typedef uint16 UINT16;

typedef uint64 UINT64;

typedef uint32 UINT32;

typedef int8 INT8;

typedef int16 INT16;

typedef int32 INT32;

typedef int64 INT64;

typedef FString STRING;

typedef FString KBE_UNICODE;

typedef float FLOAT;

typedef double DOUBLE;

typedef TArray<uint8> PYTHON;

typedef TArray<uint8> PY_DICT;

typedef TArray<uint8> PY_TUPLE;

typedef TArray<uint8> PY_LIST;

typedef TArray<uint8> ENTITYCALL;

typedef TArray<uint8> BLOB;

typedef FVector2D VECTOR2;

typedef FVector VECTOR3;

typedef FVector4 VECTOR4;

typedef int32 OBJECT_ID;

typedef uint8 BOOL;

typedef int32 CONTROLLER_ID;

typedef int32 EXPERIENCE;

typedef int32 ITEM_ID;

typedef int32 SKILLID;

typedef int32 QUESTID;

typedef uint64 DBID;

typedef uint64 UID;

typedef TArray<uint8> UID1;

typedef int32 ENTITY_ID;

typedef uint32 ENTITY_NO;

typedef uint32 SPACE_ID;

typedef FVector POSITION3D;

typedef FVector DIRECTION3D;

typedef uint32 ENTITY_UTYPE;

typedef int32 DAMAGE_TYPE;

typedef int32 ENMITY;

typedef int32 HP;

typedef int32 MP;

typedef int8 ENTITY_STATE;

typedef uint8 ENTITY_SUBSTATE;

typedef int32 ENTITY_FORBIDS;

class ENTITY_FORBID_COUNTER : public TArray<int8>
{
public:

	ENTITY_FORBID_COUNTER()
	{
	}

};

inline bool operator ==(const ENTITY_FORBID_COUNTER& a, const ENTITY_FORBID_COUNTER& b)
{
	return a == b;
};

class ENTITYID_LIST : public TArray<int32>
{
public:

	ENTITYID_LIST()
	{
	}

};

inline bool operator ==(const ENTITYID_LIST& a, const ENTITYID_LIST& b)
{
	return a == b;
};

class AVATAR_DATA
{
public:
	int8 param1;
	TArray<uint8> param2;

	AVATAR_DATA():
	param1(0),
	param2()
	{
	}

};

inline bool operator ==(const AVATAR_DATA& a, const AVATAR_DATA& b)
{
	return a.param1 == b.param1 && a.param2 == b.param2;
};

class AVATAR_INFOS
{
public:
	uint64 dbid;
	FString name;
	uint8 roleType;
	uint16 level;
	AVATAR_DATA data;

	AVATAR_INFOS():
	dbid(0),
	name(),
	roleType(0),
	level(0),
	data()
	{
	}

};

inline bool operator ==(const AVATAR_INFOS& a, const AVATAR_INFOS& b)
{
	return a.dbid == b.dbid && a.name == b.name && a.roleType == b.roleType && a.level == b.level && a.data == b.data;
};

class AVATAR_INFOS_LIST
{
public:
	TArray<AVATAR_INFOS> values;

	AVATAR_INFOS_LIST():
	values()
	{
	}

};

inline bool operator ==(const AVATAR_INFOS_LIST& a, const AVATAR_INFOS_LIST& b)
{
	return a.values == b.values;
};

class BAG
{
public:
	TArray<TArray<int64>> values22;

	BAG():
	values22()
	{
	}

};

inline bool operator ==(const BAG& a, const BAG& b)
{
	return a.values22 == b.values22;
};

class EXAMPLES
{
public:
	int64 k1;
	int64 k2;

	EXAMPLES():
	k1(0),
	k2(0)
	{
	}

};

inline bool operator ==(const EXAMPLES& a, const EXAMPLES& b)
{
	return a.k1 == b.k1 && a.k2 == b.k2;
};

class TEST_ENTITY_PROPS
{
public:
	int32 test1;

	TEST_ENTITY_PROPS():
	test1(0)
	{
	}

};

inline bool operator ==(const TEST_ENTITY_PROPS& a, const TEST_ENTITY_PROPS& b)
{
	return a.test1 == b.test1;
};

class PLAYER_ACTION
{
public:
	uint8 actionType;
	FString playCardName;
	int32 targetGridNb;

	PLAYER_ACTION():
	actionType(0),
	playCardName(),
	targetGridNb(0)
	{
	}

};

inline bool operator ==(const PLAYER_ACTION& a, const PLAYER_ACTION& b)
{
	return a.actionType == b.actionType && a.playCardName == b.playCardName && a.targetGridNb == b.targetGridNb;
};

class UPDATE_GRID_INFO
{
public:
	int32 gridNb;
	FString updateType;
	int32 updateValue;

	UPDATE_GRID_INFO():
	gridNb(0),
	updateType(),
	updateValue(0)
	{
	}

};

inline bool operator ==(const UPDATE_GRID_INFO& a, const UPDATE_GRID_INFO& b)
{
	return a.gridNb == b.gridNb && a.updateType == b.updateType && a.updateValue == b.updateValue;
};

class CORE_UPDATE_BATLLE_INFO
{
public:
	TArray<UPDATE_GRID_INFO> updateList;

	CORE_UPDATE_BATLLE_INFO():
	updateList()
	{
	}

};

inline bool operator ==(const CORE_UPDATE_BATLLE_INFO& a, const CORE_UPDATE_BATLLE_INFO& b)
{
	return a.updateList == b.updateList;
};


}