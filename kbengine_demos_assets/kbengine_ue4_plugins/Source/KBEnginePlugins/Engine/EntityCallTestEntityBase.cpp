#include "EntityCallTestEntityBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_TestEntityBase::EntityBaseEntityCall_TestEntityBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_TestEntityBase::~EntityBaseEntityCall_TestEntityBase()
{
}

void EntityBaseEntityCall_TestEntityBase::reqTestProperty()
{
	Bundle* pBundleRet = newCall("reqTestProperty", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}



EntityCellEntityCall_TestEntityBase::EntityCellEntityCall_TestEntityBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_TestEntityBase::~EntityCellEntityCall_TestEntityBase()
{
}


}