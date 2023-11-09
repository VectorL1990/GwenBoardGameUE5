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

void DATATYPE_BAG::createFromStreamEx(MemoryStream& stream, BAG& datas)
{
	values22_DataType.createFromStreamEx(stream, datas.values22);
}

void DATATYPE_BAG::addToStreamEx(Bundle& stream, const BAG& v)
{
	values22_DataType.addToStreamEx(stream, v.values22);
}

void DATATYPE_EXAMPLES::createFromStreamEx(MemoryStream& stream, EXAMPLES& datas)
{
	datas.k1 = stream.readInt64();
	datas.k2 = stream.readInt64();
}

void DATATYPE_EXAMPLES::addToStreamEx(Bundle& stream, const EXAMPLES& v)
{
	stream.writeInt64(v.k1);
	stream.writeInt64(v.k2);
}

void DATATYPE_TEST_ENTITY_PROPS::createFromStreamEx(MemoryStream& stream, TEST_ENTITY_PROPS& datas)
{
	datas.test1 = stream.readInt32();
}

void DATATYPE_TEST_ENTITY_PROPS::addToStreamEx(Bundle& stream, const TEST_ENTITY_PROPS& v)
{
	stream.writeInt32(v.test1);
}

void DATATYPE_PLAYER_ACTION::createFromStreamEx(MemoryStream& stream, PLAYER_ACTION& datas)
{
	datas.actionType = stream.readUint8();
	datas.playCardName = stream.readString();
	datas.targetGridNb = stream.readInt32();
}

void DATATYPE_PLAYER_ACTION::addToStreamEx(Bundle& stream, const PLAYER_ACTION& v)
{
	stream.writeUint8(v.actionType);
	stream.writeString(v.playCardName);
	stream.writeInt32(v.targetGridNb);
}

void DATATYPE_UPDATE_GRID_INFO::createFromStreamEx(MemoryStream& stream, UPDATE_GRID_INFO& datas)
{
	datas.gridNb = stream.readInt32();
	datas.cardUid = stream.readInt32();
	datas.updateType = stream.readString();
	datas.updateValue = stream.readInt32();
}

void DATATYPE_UPDATE_GRID_INFO::addToStreamEx(Bundle& stream, const UPDATE_GRID_INFO& v)
{
	stream.writeInt32(v.gridNb);
	stream.writeInt32(v.cardUid);
	stream.writeString(v.updateType);
	stream.writeInt32(v.updateValue);
}

void DATATYPE_UPDATE_GRID_INFO_LIST::createFromStreamEx(MemoryStream& stream, UPDATE_GRID_INFO_LIST& datas)
{
	datas.updateId = stream.readInt32();
	updateGridList_DataType.createFromStreamEx(stream, datas.updateGridList);
}

void DATATYPE_UPDATE_GRID_INFO_LIST::addToStreamEx(Bundle& stream, const UPDATE_GRID_INFO_LIST& v)
{
	stream.writeInt32(v.updateId);
	updateGridList_DataType.addToStreamEx(stream, v.updateGridList);
}

void DATATYPE_BATTLE_GRID_INFO::createFromStreamEx(MemoryStream& stream, BATTLE_GRID_INFO& datas)
{
	datas.gridNb = stream.readInt32();
	datas.cardUid = stream.readInt32();
	datas.hp = stream.readInt32();
	datas.defence = stream.readInt32();
	datas.agility = stream.readUint8();
}

void DATATYPE_BATTLE_GRID_INFO::addToStreamEx(Bundle& stream, const BATTLE_GRID_INFO& v)
{
	stream.writeInt32(v.gridNb);
	stream.writeInt32(v.cardUid);
	stream.writeInt32(v.hp);
	stream.writeInt32(v.defence);
	stream.writeUint8(v.agility);
}

void DATATYPE_CORE_UPDATE_BATLLE_INFO::createFromStreamEx(MemoryStream& stream, CORE_UPDATE_BATLLE_INFO& datas)
{
	updateList_DataType.createFromStreamEx(stream, datas.updateList);
}

void DATATYPE_CORE_UPDATE_BATLLE_INFO::addToStreamEx(Bundle& stream, const CORE_UPDATE_BATLLE_INFO& v)
{
	updateList_DataType.addToStreamEx(stream, v.updateList);
}

void DATATYPE_SYNC_BATTLE_TIME_INFO::createFromStreamEx(MemoryStream& stream, SYNC_BATTLE_TIME_INFO& datas)
{
	datas.curTime = stream.readInt32();
	datas.playerNb = stream.readUint8();
}

void DATATYPE_SYNC_BATTLE_TIME_INFO::addToStreamEx(Bundle& stream, const SYNC_BATTLE_TIME_INFO& v)
{
	stream.writeInt32(v.curTime);
	stream.writeUint8(v.playerNb);
}


}