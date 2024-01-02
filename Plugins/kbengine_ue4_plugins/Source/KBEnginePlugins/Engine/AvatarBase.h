/*
	Generated by KBEngine!
	Please do not modify this file!
	Please inherit this module, such as: (class Avatar : public AvatarBase)
	tools = kbcmd
*/

#pragma once
#include "KBECommon.h"
#include "Entity.h"
#include "KBETypes.h"
#include "EntityCallAvatarBase.h"

namespace KBEngine
{

class Method;
class Property;
class MemoryStream;

// defined in */scripts/entity_defs/Avatar.def
	// Please inherit and implement "class Avatar : public AvatarBase"
class KBENGINEPLUGINS_API AvatarBase : public Entity
{
public:
	EntityBaseEntityCall_AvatarBase* pBaseEntityCall;
	EntityCellEntityCall_AvatarBase* pCellEntityCall;

	uint32 modelID;
	virtual void onModelIDChanged(uint32 oldValue) {}
	uint8 modelScale;
	virtual void onModelScaleChanged(uint8 oldValue) {}
	FString name;
	virtual void onNameChanged(const FString& oldValue) {}
	uint32 uid;
	virtual void onUidChanged(uint32 oldValue) {}
	uint32 utype;
	virtual void onUtypeChanged(uint32 oldValue) {}

	virtual void onSyncBattleResult(const STRING_LIST& arg1) = 0; 
	virtual void onSyncChangeHandCardSuccess(uint8 arg1, const FString& arg2, const FString& arg3) = 0; 
	virtual void onSyncExhaustCardReplacement() = 0; 
	virtual void onSyncHeartBeat(int32 arg1) = 0; 
	virtual void onSyncLatestBattleState(const CORE_UPDATE_BATLLE_INFO& arg1) = 0; 
	virtual void onSyncLaunchSkillFailed(int32 arg1, int32 arg2) = 0; 
	virtual void onSyncPlayerBattleInfo(const SYNC_PLAYER_BATTLE_INFO& arg1) = 0; 
	virtual void onSyncReceiveFinishCardSelection() = 0; 
	virtual void onSyncResumeBattle(int32 arg1) = 0; 
	virtual void onSyncRoomStartBattle() = 0; 
	virtual void onSyncSelectCardInterlude(const SYNC_PLAYER_BATTLE_INFO& arg1) = 0; 
	virtual void onSyncSwitchController(int32 arg1, uint64 arg2) = 0; 
	virtual void onSyncTimeInterval(const SYNC_BATTLE_TIME_INFO& arg1) = 0; 
	virtual void onSyncUpdateActionInfo(const SYNC_MODIFICATION_INFO& arg1) = 0; 
	virtual void onSyncUpdateSelectedCards(uint8 arg1, const SYNC_PLAYER_BATTLE_INFO& arg2) = 0; 

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

	AvatarBase();
	virtual ~AvatarBase();

	void attachComponents() override;
	void detachComponents() override;

};

}