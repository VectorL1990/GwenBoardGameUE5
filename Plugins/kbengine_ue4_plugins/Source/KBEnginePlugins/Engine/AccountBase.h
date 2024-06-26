/*
	Generated by KBEngine!
	Please do not modify this file!
	Please inherit this module, such as: (class Account : public AccountBase)
	tools = kbcmd
*/

#pragma once
#include "KBECommon.h"
#include "Entity.h"
#include "KBETypes.h"
#include "EntityCallAccountBase.h"

namespace KBEngine
{

class Method;
class Property;
class MemoryStream;

// defined in */scripts/entity_defs/Account.def
	// Please inherit and implement "class Account : public AccountBase"
class KBENGINEPLUGINS_API AccountBase : public Entity
{
public:
	EntityBaseEntityCall_AccountBase* pBaseEntityCall;
	EntityCellEntityCall_AccountBase* pCellEntityCall;


	virtual void onAccountClientEnabled(const PLAYER_PERSIST_INFO& arg1) = 0; 
	virtual void onReqTest(int32 arg1) = 0; 
	virtual void onSyncRoomCreated(uint64 arg1) = 0; 

	void onComponentsEnterworld() override;
	void onComponentsLeaveworld() override;

	void onGetBase() override;
	void onGetCell() override;
	void onLoseCell() override;

	EntityCall* getBaseEntityCall() override;
	EntityCall* getCellEntityCall() override;


	void onRemoteMethodCall(MemoryStream& stream) override;
	void onUpdatePropertys(MemoryStream& stream) override;
	void callPropertysSetMethods() override;

	AccountBase();
	virtual ~AccountBase();

	void attachComponents() override;
	void detachComponents() override;

};

}