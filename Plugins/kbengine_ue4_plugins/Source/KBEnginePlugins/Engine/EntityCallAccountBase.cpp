#include "EntityCallAccountBase.h"
#include "EntityDef.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_AccountBase::EntityBaseEntityCall_AccountBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_AccountBase::~EntityBaseEntityCall_AccountBase()
{
}

void EntityBaseEntityCall_AccountBase::reqEnterRoom()
{
	Bundle* pBundleRet = newCall("reqEnterRoom", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}

void EntityBaseEntityCall_AccountBase::reqMatch()
{
	Bundle* pBundleRet = newCall("reqMatch", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}

void EntityBaseEntityCall_AccountBase::reqModifyCardGroup(int32 arg1, const CARD_GROUP& arg2)
{
	Bundle* pBundleRet = newCall("reqModifyCardGroup", 0);
	if(!pBundleRet)
		return;

	pBundleRet->writeInt32(arg1);
	((DATATYPE_CARD_GROUP*)EntityDef::id2datatypes[44])->addToStreamEx(*pBundleRet, arg2);
	sendCall(NULL);
}

void EntityBaseEntityCall_AccountBase::reqTest(int32 arg1)
{
	Bundle* pBundleRet = newCall("reqTest", 0);
	if(!pBundleRet)
		return;

	pBundleRet->writeInt32(arg1);
	sendCall(NULL);
}



EntityCellEntityCall_AccountBase::EntityCellEntityCall_AccountBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_AccountBase::~EntityCellEntityCall_AccountBase()
{
}


}