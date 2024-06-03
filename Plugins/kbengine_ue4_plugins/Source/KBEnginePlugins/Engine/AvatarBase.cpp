#include "AvatarBase.h"
#include "KBVar.h"
#include "EntityDef.h"
#include "ScriptModule.h"
#include "Property.h"
#include "Method.h"
#include "DataTypes.h"
#include "CustomDataTypes.h"
#include "MemoryStream.h"
#include "EntityComponent.h"

namespace KBEngine
{



void AvatarBase::onComponentsEnterworld()
{
}

void AvatarBase::onComponentsLeaveworld()
{
}

void AvatarBase::onGetBase()
{
	if(pBaseEntityCall)
		delete pBaseEntityCall;

	pBaseEntityCall = new EntityBaseEntityCall_AvatarBase(id(), className());
}

void AvatarBase::onGetCell()
{
	if(pCellEntityCall)
		delete pCellEntityCall;

	pCellEntityCall = new EntityCellEntityCall_AvatarBase(id(), className());
}

void AvatarBase::onLoseCell()
{
	delete pCellEntityCall;
	pCellEntityCall = NULL;
}

EntityCall* AvatarBase::getBaseEntityCall()
{
	return pBaseEntityCall;
}

EntityCall* AvatarBase::getCellEntityCall()
{
	return pCellEntityCall;
}

void AvatarBase::onRemoteMethodCall(MemoryStream& stream)
{
	ScriptModule* sm = *EntityDef::moduledefs.Find("Avatar");
	uint16 methodUtype = 0;
	uint16 componentPropertyUType = 0;

	if (sm->usePropertyDescrAlias)
	{
		componentPropertyUType = stream.readUint8();
	}
	else
	{
		componentPropertyUType = stream.readUint16();
	}

	if (sm->useMethodDescrAlias)
	{
		methodUtype = stream.read<uint8>();
	}
	else
	{
		methodUtype = stream.read<uint16>();
	}

	if(componentPropertyUType > 0)
	{
		KBE_ASSERT(false);

		return;
	}

	Method* pMethod = sm->idmethods[methodUtype];

	switch(pMethod->methodUtype)
	{
		case 31:
		{
			STRING_LIST onSyncBattleResult_arg1;
			((DATATYPE_STRING_LIST*)pMethod->args[0])->createFromStreamEx(stream, onSyncBattleResult_arg1);
			onSyncBattleResult(onSyncBattleResult_arg1);
			break;
		}
		case 22:
		{
			uint8 onSyncChangeHandCardSuccess_arg1 = stream.readUint8();
			FString onSyncChangeHandCardSuccess_arg2 = stream.readString();
			FString onSyncChangeHandCardSuccess_arg3 = stream.readString();
			onSyncChangeHandCardSuccess(onSyncChangeHandCardSuccess_arg1, onSyncChangeHandCardSuccess_arg2, onSyncChangeHandCardSuccess_arg3);
			break;
		}
		case 21:
		{
			onSyncExhaustCardReplacement();
			break;
		}
		case 29:
		{
			int32 onSyncHeartBeat_arg1 = stream.readInt32();
			onSyncHeartBeat(onSyncHeartBeat_arg1);
			break;
		}
		case 30:
		{
			CORE_UPDATE_BATLLE_INFO onSyncLatestBattleState_arg1;
			((DATATYPE_CORE_UPDATE_BATLLE_INFO*)pMethod->args[0])->createFromStreamEx(stream, onSyncLatestBattleState_arg1);
			onSyncLatestBattleState(onSyncLatestBattleState_arg1);
			break;
		}
		case 19:
		{
			int32 onSyncLaunchSkillFailed_arg1 = stream.readInt32();
			int32 onSyncLaunchSkillFailed_arg2 = stream.readInt32();
			onSyncLaunchSkillFailed(onSyncLaunchSkillFailed_arg1, onSyncLaunchSkillFailed_arg2);
			break;
		}
		case 20:
		{
			SYNC_PLAYER_BATTLE_INFO onSyncPlayerBattleInfo_arg1;
			((DATATYPE_SYNC_PLAYER_BATTLE_INFO*)pMethod->args[0])->createFromStreamEx(stream, onSyncPlayerBattleInfo_arg1);
			onSyncPlayerBattleInfo(onSyncPlayerBattleInfo_arg1);
			break;
		}
		case 17:
		{
			uint8 onSyncReceiveEnterRoom_arg1 = stream.readUint8();
			onSyncReceiveEnterRoom(onSyncReceiveEnterRoom_arg1);
			break;
		}
		case 32:
		{
			onSyncReceiveFinishCardSelection();
			break;
		}
		case 28:
		{
			int32 onSyncResumeBattle_arg1 = stream.readInt32();
			onSyncResumeBattle(onSyncResumeBattle_arg1);
			break;
		}
		case 24:
		{
			onSyncRoomStartBattle();
			break;
		}
		case 26:
		{
			SYNC_PLAYER_BATTLE_INFO onSyncSelectCardInterlude_arg1;
			((DATATYPE_SYNC_PLAYER_BATTLE_INFO*)pMethod->args[0])->createFromStreamEx(stream, onSyncSelectCardInterlude_arg1);
			onSyncSelectCardInterlude(onSyncSelectCardInterlude_arg1);
			break;
		}
		case 27:
		{
			int32 onSyncSwitchController_arg1 = stream.readInt32();
			uint64 onSyncSwitchController_arg2 = stream.readUint64();
			onSyncSwitchController(onSyncSwitchController_arg1, onSyncSwitchController_arg2);
			break;
		}
		case 25:
		{
			SYNC_BATTLE_TIME_INFO onSyncTimeInterval_arg1;
			((DATATYPE_SYNC_BATTLE_TIME_INFO*)pMethod->args[0])->createFromStreamEx(stream, onSyncTimeInterval_arg1);
			onSyncTimeInterval(onSyncTimeInterval_arg1);
			break;
		}
		case 18:
		{
			SYNC_MODIFICATION_INFO onSyncUpdateActionInfo_arg1;
			((DATATYPE_SYNC_MODIFICATION_INFO*)pMethod->args[0])->createFromStreamEx(stream, onSyncUpdateActionInfo_arg1);
			onSyncUpdateActionInfo(onSyncUpdateActionInfo_arg1);
			break;
		}
		case 23:
		{
			uint8 onSyncUpdateSelectedCards_arg1 = stream.readUint8();
			SYNC_PLAYER_BATTLE_INFO onSyncUpdateSelectedCards_arg2;
			((DATATYPE_SYNC_PLAYER_BATTLE_INFO*)pMethod->args[1])->createFromStreamEx(stream, onSyncUpdateSelectedCards_arg2);
			onSyncUpdateSelectedCards(onSyncUpdateSelectedCards_arg1, onSyncUpdateSelectedCards_arg2);
			break;
		}
		default:
			break;
	};
}

void AvatarBase::onUpdatePropertys(MemoryStream& stream)
{
	ScriptModule* sm = *EntityDef::moduledefs.Find("Avatar");

	while(stream.length() > 0)
	{
		uint16 componentPropertyUType = 0;
		uint16 properUtype = 0;

		if (sm->usePropertyDescrAlias)
		{
			componentPropertyUType = stream.readUint8();
			properUtype = stream.read<uint8>();
		}
		else
		{
			componentPropertyUType = stream.readUint16();
			properUtype = stream.read<uint16>();
		}

		if(componentPropertyUType > 0)
		{
			KBE_ASSERT(false);

			return;
		}

		Property* pProp = sm->idpropertys[properUtype];

		switch(pProp->properUtype)
		{
			case 40001:
			{
				FVector oldval_direction = direction;
				direction = stream.readVector3();

				if(pProp->isBase())
				{
					if(inited())
						onDirectionChanged(oldval_direction);
				}
				else
				{
					if(inWorld())
						onDirectionChanged(oldval_direction);
				}

				break;
			}
			case 41006:
			{
				uint32 oldval_modelID = modelID;
				modelID = stream.readUint32();

				if(pProp->isBase())
				{
					if(inited())
						onModelIDChanged(oldval_modelID);
				}
				else
				{
					if(inWorld())
						onModelIDChanged(oldval_modelID);
				}

				break;
			}
			case 41007:
			{
				uint8 oldval_modelScale = modelScale;
				modelScale = stream.readUint8();

				if(pProp->isBase())
				{
					if(inited())
						onModelScaleChanged(oldval_modelScale);
				}
				else
				{
					if(inWorld())
						onModelScaleChanged(oldval_modelScale);
				}

				break;
			}
			case 41003:
			{
				FString oldval_name = name;
				name = stream.readUnicode();

				if(pProp->isBase())
				{
					if(inited())
						onNameChanged(oldval_name);
				}
				else
				{
					if(inWorld())
						onNameChanged(oldval_name);
				}

				break;
			}
			case 40000:
			{
				FVector oldval_position = position;
				position = stream.readVector3();

				if(pProp->isBase())
				{
					if(inited())
						onPositionChanged(oldval_position);
				}
				else
				{
					if(inWorld())
						onPositionChanged(oldval_position);
				}

				break;
			}
			case 40002:
			{
				stream.readUint32();
				break;
			}
			case 41004:
			{
				uint32 oldval_uid = uid;
				uid = stream.readUint32();

				if(pProp->isBase())
				{
					if(inited())
						onUidChanged(oldval_uid);
				}
				else
				{
					if(inWorld())
						onUidChanged(oldval_uid);
				}

				break;
			}
			case 41005:
			{
				uint32 oldval_utype = utype;
				utype = stream.readUint32();

				if(pProp->isBase())
				{
					if(inited())
						onUtypeChanged(oldval_utype);
				}
				else
				{
					if(inWorld())
						onUtypeChanged(oldval_utype);
				}

				break;
			}
			default:
				break;
		};
	}
}

void AvatarBase::callPropertysSetMethods()
{
	ScriptModule* sm = EntityDef::moduledefs["Avatar"];
	TMap<uint16, Property*>& pdatas = sm->idpropertys;

	FVector oldval_direction = direction;
	Property* pProp_direction = pdatas[2];
	if(pProp_direction->isBase())
	{
		if(inited() && !inWorld())
			onDirectionChanged(oldval_direction);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_direction->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onDirectionChanged(oldval_direction);
			}
		}
	}

	uint32 oldval_modelID = modelID;
	Property* pProp_modelID = pdatas[4];
	if(pProp_modelID->isBase())
	{
		if(inited() && !inWorld())
			onModelIDChanged(oldval_modelID);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_modelID->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onModelIDChanged(oldval_modelID);
			}
		}
	}

	uint8 oldval_modelScale = modelScale;
	Property* pProp_modelScale = pdatas[5];
	if(pProp_modelScale->isBase())
	{
		if(inited() && !inWorld())
			onModelScaleChanged(oldval_modelScale);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_modelScale->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onModelScaleChanged(oldval_modelScale);
			}
		}
	}

	FString oldval_name = name;
	Property* pProp_name = pdatas[6];
	if(pProp_name->isBase())
	{
		if(inited() && !inWorld())
			onNameChanged(oldval_name);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_name->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onNameChanged(oldval_name);
			}
		}
	}

	FVector oldval_position = position;
	Property* pProp_position = pdatas[1];
	if(pProp_position->isBase())
	{
		if(inited() && !inWorld())
			onPositionChanged(oldval_position);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_position->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onPositionChanged(oldval_position);
			}
		}
	}

	uint32 oldval_uid = uid;
	Property* pProp_uid = pdatas[7];
	if(pProp_uid->isBase())
	{
		if(inited() && !inWorld())
			onUidChanged(oldval_uid);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_uid->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onUidChanged(oldval_uid);
			}
		}
	}

	uint32 oldval_utype = utype;
	Property* pProp_utype = pdatas[8];
	if(pProp_utype->isBase())
	{
		if(inited() && !inWorld())
			onUtypeChanged(oldval_utype);
	}
	else
	{
		if(inWorld())
		{
			if(pProp_utype->isOwnerOnly() && !isPlayer())
			{
			}
			else
			{
				onUtypeChanged(oldval_utype);
			}
		}
	}

}

AvatarBase::AvatarBase():
	Entity(),
	pBaseEntityCall(NULL),
	pCellEntityCall(NULL),
	modelID((uint32)FCString::Atoi64(TEXT("0"))),
	modelScale((uint8)FCString::Atoi64(TEXT("30"))),
	name(TEXT("")),
	uid((uint32)FCString::Atoi64(TEXT("0"))),
	utype((uint32)FCString::Atoi64(TEXT("0")))
{
}

AvatarBase::~AvatarBase()
{
	if(pBaseEntityCall)
		delete pBaseEntityCall;

	if(pCellEntityCall)
		delete pCellEntityCall;

}

void AvatarBase::attachComponents()
{
}

void AvatarBase::detachComponents()
{
}

}