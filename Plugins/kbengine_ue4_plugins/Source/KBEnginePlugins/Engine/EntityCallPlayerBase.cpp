#include "EntityCallPlayerBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_PlayerBase::EntityBaseEntityCall_PlayerBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_PlayerBase::~EntityBaseEntityCall_PlayerBase()
{
}



EntityCellEntityCall_PlayerBase::EntityCellEntityCall_PlayerBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_PlayerBase::~EntityCellEntityCall_PlayerBase()
{
}


}