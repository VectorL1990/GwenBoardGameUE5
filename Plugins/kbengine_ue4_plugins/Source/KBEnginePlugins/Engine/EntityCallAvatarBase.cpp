#include "EntityCallAvatarBase.h"
#include "Bundle.h"

namespace KBEngine
{

EntityBaseEntityCall_AvatarBase::EntityBaseEntityCall_AvatarBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_BASE;
}

EntityBaseEntityCall_AvatarBase::~EntityBaseEntityCall_AvatarBase()
{
}

void EntityBaseEntityCall_AvatarBase::reqChangeSelectCard(const FString& arg1)
{
	Bundle* pBundleRet = newCall("reqChangeSelectCard", 0);
	if(!pBundleRet)
		return;

	pBundleRet->writeString(arg1);
	sendCall(NULL);
}

void EntityBaseEntityCall_AvatarBase::reqFinishSelectCards()
{
	Bundle* pBundleRet = newCall("reqFinishSelectCards", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}

void EntityBaseEntityCall_AvatarBase::reqLatestBattleInfo()
{
	Bundle* pBundleRet = newCall("reqLatestBattleInfo", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}

void EntityBaseEntityCall_AvatarBase::reqSyncHeartBeat()
{
	Bundle* pBundleRet = newCall("reqSyncHeartBeat", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}

void EntityBaseEntityCall_AvatarBase::reqUpdateSelectedCard()
{
	Bundle* pBundleRet = newCall("reqUpdateSelectedCard", 0);
	if(!pBundleRet)
		return;

	sendCall(NULL);
}



EntityCellEntityCall_AvatarBase::EntityCellEntityCall_AvatarBase(int32 eid, const FString& ename) : EntityCall(eid, ename)
{
	type = ENTITYCALL_TYPE_CELL;
}

EntityCellEntityCall_AvatarBase::~EntityCellEntityCall_AvatarBase()
{
}


}