#include "CustomDataTypes.h"
#include "EntityDef.h"
#include "KBDebug.h"
#include "DataTypes.h"
#include "Runtime/Core/Public/Misc/Variant.h"

namespace KBEngine
{

void DATATYPE_ENTITY_FORBID_COUNTER::createFromStreamEx(MemoryStream& stream, ENTITY_FORBID_COUNTER& datas)
{
	uint32 size = stream.readUint32();
	while(size > 0)
	{
		--size;
		datas.Add(stream.readInt8());
	};

}

void DATATYPE_ENTITY_FORBID_COUNTER::addToStreamEx(Bundle& stream, const ENTITY_FORBID_COUNTER& v)
{
	stream.writeUint32((uint32)v.Num());
	for(int i=0; i<v.Num(); ++i)
	{
		stream.writeInt8(v[i]);
	};
}

void DATATYPE_ENTITYID_LIST::createFromStreamEx(MemoryStream& stream, ENTITYID_LIST& datas)
{
	uint32 size = stream.readUint32();
	while(size > 0)
	{
		--size;
		datas.Add(stream.readInt32());
	};

}

void DATATYPE_ENTITYID_LIST::addToStreamEx(Bundle& stream, const ENTITYID_LIST& v)
{
	stream.writeUint32((uint32)v.Num());
	for(int i=0; i<v.Num(); ++i)
	{
		stream.writeInt32(v[i]);
	};
}

void DATATYPE_TEST_ENTITY_PROPS::createFromStreamEx(MemoryStream& stream, TEST_ENTITY_PROPS& datas)
{
	datas.test1 = stream.readInt32();
}

void DATATYPE_TEST_ENTITY_PROPS::addToStreamEx(Bundle& stream, const TEST_ENTITY_PROPS& v)
{
	stream.writeInt32(v.test1);
}

void DATATYPE_STATE_INFO::createFromStreamEx(MemoryStream& stream, STATE_INFO& datas)
{
	datas.state = stream.readString();
	datas.stipulation = stream.readUint8();
	datas.curCount = stream.readUint8();
}

void DATATYPE_STATE_INFO::addToStreamEx(Bundle& stream, const STATE_INFO& v)
{
	stream.writeString(v.state);
	stream.writeUint8(v.stipulation);
	stream.writeUint8(v.curCount);
}

void DATATYPE_SYNC_EFFECT_INFO::createFromStreamEx(MemoryStream& stream, SYNC_EFFECT_INFO& datas)
{
	datas.effectName = stream.readString();
	datas.countDown = stream.readUint8();
	datas.availableTimes = stream.readUint8();
}

void DATATYPE_SYNC_EFFECT_INFO::addToStreamEx(Bundle& stream, const SYNC_EFFECT_INFO& v)
{
	stream.writeString(v.effectName);
	stream.writeUint8(v.countDown);
	stream.writeUint8(v.availableTimes);
}

void DATATYPE_BATTLE_GRID_ROW_INFO::createFromStreamEx(MemoryStream& stream, BATTLE_GRID_ROW_INFO& datas)
{
	cols_DataType.createFromStreamEx(stream, datas.cols);
}

void DATATYPE_BATTLE_GRID_ROW_INFO::addToStreamEx(Bundle& stream, const BATTLE_GRID_ROW_INFO& v)
{
	cols_DataType.addToStreamEx(stream, v.cols);
}

void DATATYPE_BATTLE_BOARD_INFO::createFromStreamEx(MemoryStream& stream, BATTLE_BOARD_INFO& datas)
{
	rows_DataType.createFromStreamEx(stream, datas.rows);
}

void DATATYPE_BATTLE_BOARD_INFO::addToStreamEx(Bundle& stream, const BATTLE_BOARD_INFO& v)
{
	rows_DataType.addToStreamEx(stream, v.rows);
}

void DATATYPE_BATTLE_GRID_STRING_STATE::createFromStreamEx(MemoryStream& stream, BATTLE_GRID_STRING_STATE& datas)
{
	datas.gridNb = stream.readInt32();
	datas.gridState = stream.readString();
}

void DATATYPE_BATTLE_GRID_STRING_STATE::addToStreamEx(Bundle& stream, const BATTLE_GRID_STRING_STATE& v)
{
	stream.writeInt32(v.gridNb);
	stream.writeString(v.gridState);
}

void DATATYPE_SYNC_CARD_INFO::createFromStreamEx(MemoryStream& stream, SYNC_CARD_INFO& datas)
{
	datas.cardKey = stream.readString();
	datas.cardName = stream.readString();
	datas.hp = stream.readUint8();
	datas.defence = stream.readUint8();
	datas.agility = stream.readUint8();
	tags_DataType.createFromStreamEx(stream, datas.tags);
	stateTags_DataType.createFromStreamEx(stream, datas.stateTags);
	effectInfos_DataType.createFromStreamEx(stream, datas.effectInfos);
}

void DATATYPE_SYNC_CARD_INFO::addToStreamEx(Bundle& stream, const SYNC_CARD_INFO& v)
{
	stream.writeString(v.cardKey);
	stream.writeString(v.cardName);
	stream.writeUint8(v.hp);
	stream.writeUint8(v.defence);
	stream.writeUint8(v.agility);
	tags_DataType.addToStreamEx(stream, v.tags);
	stateTags_DataType.addToStreamEx(stream, v.stateTags);
	effectInfos_DataType.addToStreamEx(stream, v.effectInfos);
}

void DATATYPE_SYNC_PLAYER_BATTLE_INFO::createFromStreamEx(MemoryStream& stream, SYNC_PLAYER_BATTLE_INFO& datas)
{
	cardList_DataType.createFromStreamEx(stream, datas.cardList);
	handCardList_DataType.createFromStreamEx(stream, datas.handCardList);
}

void DATATYPE_SYNC_PLAYER_BATTLE_INFO::addToStreamEx(Bundle& stream, const SYNC_PLAYER_BATTLE_INFO& v)
{
	cardList_DataType.addToStreamEx(stream, v.cardList);
	handCardList_DataType.addToStreamEx(stream, v.handCardList);
}

void DATATYPE_CORE_UPDATE_BATLLE_INFO::createFromStreamEx(MemoryStream& stream, CORE_UPDATE_BATLLE_INFO& datas)
{
	datas.curSwitchControllerSequence = stream.readInt32();
	datas.curControllerNb = stream.readUint8();
	datas.curControllerAvatarId = stream.readUint64();
	datas.curActionSequence = stream.readInt32();
	updateList_DataType.createFromStreamEx(stream, datas.updateList);
	playerInfo_DataType.createFromStreamEx(stream, datas.playerInfo);
}

void DATATYPE_CORE_UPDATE_BATLLE_INFO::addToStreamEx(Bundle& stream, const CORE_UPDATE_BATLLE_INFO& v)
{
	stream.writeInt32(v.curSwitchControllerSequence);
	stream.writeUint8(v.curControllerNb);
	stream.writeUint64(v.curControllerAvatarId);
	stream.writeInt32(v.curActionSequence);
	updateList_DataType.addToStreamEx(stream, v.updateList);
	playerInfo_DataType.addToStreamEx(stream, v.playerInfo);
}

void DATATYPE_SYNC_BATTLE_TIME_INFO::createFromStreamEx(MemoryStream& stream, SYNC_BATTLE_TIME_INFO& datas)
{
	datas.curTime = stream.readInt32();
	datas.battleState = stream.readUint8();
}

void DATATYPE_SYNC_BATTLE_TIME_INFO::addToStreamEx(Bundle& stream, const SYNC_BATTLE_TIME_INFO& v)
{
	stream.writeInt32(v.curTime);
	stream.writeUint8(v.battleState);
}

void DATATYPE_SYNC_MODIFICATION_INFO::createFromStreamEx(MemoryStream& stream, SYNC_MODIFICATION_INFO& datas)
{
	datas.actionSequence = stream.readInt32();
	updateGridList_DataType.createFromStreamEx(stream, datas.updateGridList);
	updateCardList_DataType.createFromStreamEx(stream, datas.updateCardList);
}

void DATATYPE_SYNC_MODIFICATION_INFO::addToStreamEx(Bundle& stream, const SYNC_MODIFICATION_INFO& v)
{
	stream.writeInt32(v.actionSequence);
	updateGridList_DataType.addToStreamEx(stream, v.updateGridList);
	updateCardList_DataType.addToStreamEx(stream, v.updateCardList);
}

void DATATYPE_PLAYER_PERSIST_INFO::createFromStreamEx(MemoryStream& stream, PLAYER_PERSIST_INFO& datas)
{
	persistCardList_DataType.createFromStreamEx(stream, datas.persistCardList);
	datas.campNb = stream.readUint8();
}

void DATATYPE_PLAYER_PERSIST_INFO::addToStreamEx(Bundle& stream, const PLAYER_PERSIST_INFO& v)
{
	persistCardList_DataType.addToStreamEx(stream, v.persistCardList);
	stream.writeUint8(v.campNb);
}

void DATATYPE_STRING_LIST::createFromStreamEx(MemoryStream& stream, STRING_LIST& datas)
{
	stringList_DataType.createFromStreamEx(stream, datas.stringList);
}

void DATATYPE_STRING_LIST::addToStreamEx(Bundle& stream, const STRING_LIST& v)
{
	stringList_DataType.addToStreamEx(stream, v.stringList);
}


}