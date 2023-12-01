#include "EntityCallHallBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_HallBase::EntityBaseEntityCall_HallBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_HallBase::~EntityBaseEntityCall_HallBase()
{
}



EntityCellEntityCall_HallBase::EntityCellEntityCall_HallBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_HallBase::~EntityCellEntityCall_HallBase()
{
}


}