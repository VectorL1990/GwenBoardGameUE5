#include "EntityCallMatcherBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_MatcherBase::EntityBaseEntityCall_MatcherBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_MatcherBase::~EntityBaseEntityCall_MatcherBase()
{
}



EntityCellEntityCall_MatcherBase::EntityCellEntityCall_MatcherBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_MatcherBase::~EntityCellEntityCall_MatcherBase()
{
}


}