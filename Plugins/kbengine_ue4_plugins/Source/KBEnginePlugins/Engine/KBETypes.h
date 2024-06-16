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

class STATE_INFO
{
public:
	FString state;
	uint8 stipulation;
	uint8 curCount;

	STATE_INFO():
	state(),
	stipulation(0),
	curCount(0)
	{
	}

};

inline bool operator ==(const STATE_INFO& a, const STATE_INFO& b)
{
	return a.state == b.state && a.stipulation == b.stipulation && a.curCount == b.curCount;
};

class SYNC_EFFECT_INFO
{
public:
	FString effectName;
	uint8 countDown;
	uint8 availableTimes;

	SYNC_EFFECT_INFO():
	effectName(),
	countDown(0),
	availableTimes(0)
	{
	}

};

inline bool operator ==(const SYNC_EFFECT_INFO& a, const SYNC_EFFECT_INFO& b)
{
	return a.effectName == b.effectName && a.countDown == b.countDown && a.availableTimes == b.availableTimes;
};

class BATTLE_GRID_ROW_INFO
{
public:
	TArray<FString> cols;

	BATTLE_GRID_ROW_INFO():
	cols()
	{
	}

};

inline bool operator ==(const BATTLE_GRID_ROW_INFO& a, const BATTLE_GRID_ROW_INFO& b)
{
	return a.cols == b.cols;
};

class BATTLE_BOARD_INFO
{
public:
	TArray<BATTLE_GRID_ROW_INFO> rows;

	BATTLE_BOARD_INFO():
	rows()
	{
	}

};

inline bool operator ==(const BATTLE_BOARD_INFO& a, const BATTLE_BOARD_INFO& b)
{
	return a.rows == b.rows;
};

class BATTLE_GRID_STRING_STATE
{
public:
	int32 gridNb;
	FString gridState;

	BATTLE_GRID_STRING_STATE():
	gridNb(0),
	gridState()
	{
	}

};

inline bool operator ==(const BATTLE_GRID_STRING_STATE& a, const BATTLE_GRID_STRING_STATE& b)
{
	return a.gridNb == b.gridNb && a.gridState == b.gridState;
};

class SYNC_CARD_INFO
{
public:
	FString cardKey;
	FString cardName;
	uint8 hp;
	uint8 defence;
	uint8 agility;
	TArray<FString> tags;
	TArray<STATE_INFO> stateTags;
	TArray<SYNC_EFFECT_INFO> effectInfos;

	SYNC_CARD_INFO():
	cardKey(),
	cardName(),
	hp(0),
	defence(0),
	agility(0),
	tags(),
	stateTags(),
	effectInfos()
	{
	}

};

inline bool operator ==(const SYNC_CARD_INFO& a, const SYNC_CARD_INFO& b)
{
	return a.cardKey == b.cardKey && a.cardName == b.cardName && a.hp == b.hp && a.defence == b.defence && a.agility == b.agility && a.tags == b.tags && a.stateTags == b.stateTags && a.effectInfos == b.effectInfos;
};

class SYNC_PLAYER_BATTLE_INFO
{
public:
	TArray<SYNC_CARD_INFO> cardList;
	TArray<FString> handCardList;

	SYNC_PLAYER_BATTLE_INFO():
	cardList(),
	handCardList()
	{
	}

};

inline bool operator ==(const SYNC_PLAYER_BATTLE_INFO& a, const SYNC_PLAYER_BATTLE_INFO& b)
{
	return a.cardList == b.cardList && a.handCardList == b.handCardList;
};

class CORE_UPDATE_BATLLE_INFO
{
public:
	int32 curSwitchControllerSequence;
	uint8 curControllerNb;
	uint64 curControllerAvatarId;
	int32 curActionSequence;
	BATTLE_BOARD_INFO updateList;
	SYNC_PLAYER_BATTLE_INFO playerInfo;

	CORE_UPDATE_BATLLE_INFO():
	curSwitchControllerSequence(0),
	curControllerNb(0),
	curControllerAvatarId(0),
	curActionSequence(0),
	updateList(),
	playerInfo()
	{
	}

};

inline bool operator ==(const CORE_UPDATE_BATLLE_INFO& a, const CORE_UPDATE_BATLLE_INFO& b)
{
	return a.curSwitchControllerSequence == b.curSwitchControllerSequence && a.curControllerNb == b.curControllerNb && a.curControllerAvatarId == b.curControllerAvatarId && a.curActionSequence == b.curActionSequence && a.updateList == b.updateList && a.playerInfo == b.playerInfo;
};

class SYNC_BATTLE_TIME_INFO
{
public:
	int32 curTime;
	uint8 battleState;

	SYNC_BATTLE_TIME_INFO():
	curTime(0),
	battleState(0)
	{
	}

};

inline bool operator ==(const SYNC_BATTLE_TIME_INFO& a, const SYNC_BATTLE_TIME_INFO& b)
{
	return a.curTime == b.curTime && a.battleState == b.battleState;
};

class SYNC_MODIFICATION_INFO
{
public:
	int32 actionSequence;
	TArray<BATTLE_GRID_STRING_STATE> updateGridList;
	TArray<SYNC_CARD_INFO> updateCardList;

	SYNC_MODIFICATION_INFO():
	actionSequence(0),
	updateGridList(),
	updateCardList()
	{
	}

};

inline bool operator ==(const SYNC_MODIFICATION_INFO& a, const SYNC_MODIFICATION_INFO& b)
{
	return a.actionSequence == b.actionSequence && a.updateGridList == b.updateGridList && a.updateCardList == b.updateCardList;
};

class CARD_GROUP
{
public:
	TArray<FString> stringList;

	CARD_GROUP():
	stringList()
	{
	}

};

inline bool operator ==(const CARD_GROUP& a, const CARD_GROUP& b)
{
	return a.stringList == b.stringList;
};

class PLAYER_PERSIST_INFO
{
public:
	TArray<CARD_GROUP> persistCardList;
	uint8 campNb;

	PLAYER_PERSIST_INFO():
	persistCardList(),
	campNb(0)
	{
	}

};

inline bool operator ==(const PLAYER_PERSIST_INFO& a, const PLAYER_PERSIST_INFO& b)
{
	return a.persistCardList == b.persistCardList && a.campNb == b.campNb;
};

class STRING_LIST
{
public:
	TArray<FString> stringList;

	STRING_LIST():
	stringList()
	{
	}

};

inline bool operator ==(const STRING_LIST& a, const STRING_LIST& b)
{
	return a.stringList == b.stringList;
};


}