#include "EntityCallPvPRoomBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_PvPRoomBase::EntityBaseEntityCall_PvPRoomBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_PvPRoomBase::~EntityBaseEntityCall_PvPRoomBase()
{
}



EntityCellEntityCall_PvPRoomBase::EntityCellEntityCall_PvPRoomBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_PvPRoomBase::~EntityCellEntityCall_PvPRoomBase()
{
}


}