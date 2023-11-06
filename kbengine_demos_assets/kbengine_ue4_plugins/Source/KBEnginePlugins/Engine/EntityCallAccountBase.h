/*
	Generated by KBEngine!
	Please do not modify this file!
	
	tools = kbcmd
*/

#pragma once

#include "KBECommon.h"
#include "EntityCall.h"
#include "KBETypes.h"
#include "CustomDataTypes.h"


// defined in */scripts/entity_defs/Account.def

namespace KBEngine
{

class KBENGINEPLUGINS_API EntityBaseEntityCall_AccountBase : public EntityCall
{
public:

	EntityBaseEntityCall_AccountBase(int32 eid, const FString& ename);

	virtual ~EntityBaseEntityCall_AccountBase();
	void reqAvatarList();
	void reqCreateAvatar(uint8 arg1, const FString& arg2);
	void reqRemoveAvatar(const FString& arg1);
	void reqRemoveAvatarDBID(uint64 arg1);
	void reqTest(int32 arg1);
	void selectAvatarGame(uint64 arg1);
};

class KBENGINEPLUGINS_API EntityCellEntityCall_AccountBase : public EntityCall
{
public:

	EntityCellEntityCall_AccountBase(int32 eid, const FString& ename);

	virtual ~EntityCellEntityCall_AccountBase();
};

}