#include "EntityDef.h"
#include "DataTypes.h"
#include "CustomDataTypes.h"
#include "ScriptModule.h"
#include "Property.h"
#include "Method.h"
#include "KBVar.h"
#include "Entity.h"

#include "Scripts/Account.h"
#include "Scripts/Avatar.h"
#include "Scripts/Gate.h"
#include "Scripts/TestEntity.h"
#include "Scripts/Player.h"

namespace KBEngine
{

TMap<FString, uint16> EntityDef::datatype2id;
TMap<FString, DATATYPE_BASE*> EntityDef::datatypes;
TMap<uint16, DATATYPE_BASE*> EntityDef::id2datatypes;
TMap<FString, int32> EntityDef::entityclass;
TMap<FString, ScriptModule*> EntityDef::moduledefs;
TMap<uint16, ScriptModule*> EntityDef::idmoduledefs;

bool EntityDef::initialize()
{
	initDataTypes();
	initDefTypes();
	initScriptModules();
	return true;
}

bool EntityDef::reset()
{
	clear();
	return initialize();
}

void EntityDef::clear()
{
	TArray<DATATYPE_BASE*> deleted_datatypes;
	for (auto& item : EntityDef::datatypes)
	{
		int32 idx = deleted_datatypes.Find(item.Value);
		if (idx != INDEX_NONE)
			continue;

		deleted_datatypes.Add(item.Value);
		delete item.Value;
	}

	for (auto& item : EntityDef::moduledefs)
		delete item.Value;

	datatype2id.Empty();
	datatypes.Empty();
	id2datatypes.Empty();
	entityclass.Empty();
	moduledefs.Empty();
	idmoduledefs.Empty();
}

void EntityDef::initDataTypes()
{
	datatypes.Add(TEXT("UINT8"), new DATATYPE_UINT8());
	datatypes.Add(TEXT("UINT16"), new DATATYPE_UINT16());
	datatypes.Add(TEXT("UINT32"), new DATATYPE_UINT32());
	datatypes.Add(TEXT("UINT64"), new DATATYPE_UINT64());

	datatypes.Add(TEXT("INT8"), new DATATYPE_INT8());
	datatypes.Add(TEXT("INT16"), new DATATYPE_INT16());
	datatypes.Add(TEXT("INT32"), new DATATYPE_INT32());
	datatypes.Add(TEXT("INT64"), new DATATYPE_INT64());

	datatypes.Add(TEXT("FLOAT"), new DATATYPE_FLOAT());
	datatypes.Add(TEXT("DOUBLE"), new DATATYPE_DOUBLE());

	datatypes.Add(TEXT("STRING"), new DATATYPE_STRING());
	datatypes.Add(TEXT("VECTOR2"), new DATATYPE_VECTOR2());

	datatypes.Add(TEXT("VECTOR3"), new DATATYPE_VECTOR3());

	datatypes.Add(TEXT("VECTOR4"), new DATATYPE_VECTOR4());
	datatypes.Add(TEXT("PYTHON"), new DATATYPE_PYTHON());

	datatypes.Add(TEXT("UNICODE"), new DATATYPE_UNICODE());
	datatypes.Add(TEXT("ENTITYCALL"), new DATATYPE_ENTITYCALL());

	datatypes.Add(TEXT("BLOB"), new DATATYPE_BLOB());
}

Entity* EntityDef::createEntity(int utype)
{
	Entity* pEntity = NULL;

	switch(utype)
	{
		case 1:
			pEntity = new Account();
			break;
		case 2:
			pEntity = new Avatar();
			break;
		case 3:
			pEntity = new Gate();
			break;
		case 4:
			pEntity = new TestEntity();
			break;
		case 5:
			pEntity = new Player();
			break;
		default:
			SCREEN_ERROR_MSG("EntityDef::createEntity() : entity(%d) not found!", utype);
			break;
	};

	return pEntity;
}

void EntityDef::initScriptModules()
{
	ScriptModule* pAccountModule = new ScriptModule("Account", 1);
	EntityDef::moduledefs.Add(TEXT("Account"), pAccountModule);
	EntityDef::idmoduledefs.Add(1, pAccountModule);

	Property* pAccount_position = new Property();
	pAccount_position->name = TEXT("position");
	pAccount_position->properUtype = 40000;
	pAccount_position->properFlags = 4;
	pAccount_position->aliasID = 1;
	KBVar* pAccount_position_defval = new KBVar(FVector());
	pAccount_position->pDefaultVal = pAccount_position_defval;
	pAccountModule->propertys.Add(TEXT("position"), pAccount_position); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_position->aliasID, pAccount_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(position / 40000).");

	Property* pAccount_direction = new Property();
	pAccount_direction->name = TEXT("direction");
	pAccount_direction->properUtype = 40001;
	pAccount_direction->properFlags = 4;
	pAccount_direction->aliasID = 2;
	KBVar* pAccount_direction_defval = new KBVar(FVector());
	pAccount_direction->pDefaultVal = pAccount_direction_defval;
	pAccountModule->propertys.Add(TEXT("direction"), pAccount_direction); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_direction->aliasID, pAccount_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(direction / 40001).");

	Property* pAccount_spaceID = new Property();
	pAccount_spaceID->name = TEXT("spaceID");
	pAccount_spaceID->properUtype = 40002;
	pAccount_spaceID->properFlags = 16;
	pAccount_spaceID->aliasID = 3;
	KBVar* pAccount_spaceID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("")));
	pAccount_spaceID->pDefaultVal = pAccount_spaceID_defval;
	pAccountModule->propertys.Add(TEXT("spaceID"), pAccount_spaceID); 

	pAccountModule->usePropertyDescrAlias = true;
	pAccountModule->idpropertys.Add((uint16)pAccount_spaceID->aliasID, pAccount_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), property(spaceID / 40002).");

	TArray<DATATYPE_BASE*> Account_onReqTest_args;
	Account_onReqTest_args.Add(EntityDef::id2datatypes[8]);

	Method* pAccount_onReqTest = new Method();
	pAccount_onReqTest->name = TEXT("onReqTest");
	pAccount_onReqTest->methodUtype = 4;
	pAccount_onReqTest->aliasID = 1;
	pAccount_onReqTest->args = Account_onReqTest_args;

	pAccountModule->methods.Add(TEXT("onReqTest"), pAccount_onReqTest); 
	pAccountModule->useMethodDescrAlias = true;
	pAccountModule->idmethods.Add((uint16)pAccount_onReqTest->aliasID, pAccount_onReqTest);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(onReqTest / 4).");

	TArray<DATATYPE_BASE*> Account_onSyncRoomCreated_args;
	Account_onSyncRoomCreated_args.Add(EntityDef::id2datatypes[5]);

	Method* pAccount_onSyncRoomCreated = new Method();
	pAccount_onSyncRoomCreated->name = TEXT("onSyncRoomCreated");
	pAccount_onSyncRoomCreated->methodUtype = 5;
	pAccount_onSyncRoomCreated->aliasID = 2;
	pAccount_onSyncRoomCreated->args = Account_onSyncRoomCreated_args;

	pAccountModule->methods.Add(TEXT("onSyncRoomCreated"), pAccount_onSyncRoomCreated); 
	pAccountModule->useMethodDescrAlias = true;
	pAccountModule->idmethods.Add((uint16)pAccount_onSyncRoomCreated->aliasID, pAccount_onSyncRoomCreated);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(onSyncRoomCreated / 5).");

	TArray<DATATYPE_BASE*> Account_reqEnterRoom_args;

	Method* pAccount_reqEnterRoom = new Method();
	pAccount_reqEnterRoom->name = TEXT("reqEnterRoom");
	pAccount_reqEnterRoom->methodUtype = 2;
	pAccount_reqEnterRoom->aliasID = -1;
	pAccount_reqEnterRoom->args = Account_reqEnterRoom_args;

	pAccountModule->methods.Add(TEXT("reqEnterRoom"), pAccount_reqEnterRoom); 
	pAccountModule->base_methods.Add(TEXT("reqEnterRoom"), pAccount_reqEnterRoom);

	pAccountModule->idbase_methods.Add(pAccount_reqEnterRoom->methodUtype, pAccount_reqEnterRoom);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqEnterRoom / 2).");

	TArray<DATATYPE_BASE*> Account_reqMatch_args;

	Method* pAccount_reqMatch = new Method();
	pAccount_reqMatch->name = TEXT("reqMatch");
	pAccount_reqMatch->methodUtype = 3;
	pAccount_reqMatch->aliasID = -1;
	pAccount_reqMatch->args = Account_reqMatch_args;

	pAccountModule->methods.Add(TEXT("reqMatch"), pAccount_reqMatch); 
	pAccountModule->base_methods.Add(TEXT("reqMatch"), pAccount_reqMatch);

	pAccountModule->idbase_methods.Add(pAccount_reqMatch->methodUtype, pAccount_reqMatch);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqMatch / 3).");

	TArray<DATATYPE_BASE*> Account_reqTest_args;
	Account_reqTest_args.Add(EntityDef::id2datatypes[8]);

	Method* pAccount_reqTest = new Method();
	pAccount_reqTest->name = TEXT("reqTest");
	pAccount_reqTest->methodUtype = 1;
	pAccount_reqTest->aliasID = -1;
	pAccount_reqTest->args = Account_reqTest_args;

	pAccountModule->methods.Add(TEXT("reqTest"), pAccount_reqTest); 
	pAccountModule->base_methods.Add(TEXT("reqTest"), pAccount_reqTest);

	pAccountModule->idbase_methods.Add(pAccount_reqTest->methodUtype, pAccount_reqTest);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Account), method(reqTest / 1).");

	ScriptModule* pAvatarModule = new ScriptModule("Avatar", 2);
	EntityDef::moduledefs.Add(TEXT("Avatar"), pAvatarModule);
	EntityDef::idmoduledefs.Add(2, pAvatarModule);

	Property* pAvatar_position = new Property();
	pAvatar_position->name = TEXT("position");
	pAvatar_position->properUtype = 40000;
	pAvatar_position->properFlags = 4;
	pAvatar_position->aliasID = 1;
	KBVar* pAvatar_position_defval = new KBVar(FVector());
	pAvatar_position->pDefaultVal = pAvatar_position_defval;
	pAvatarModule->propertys.Add(TEXT("position"), pAvatar_position); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_position->aliasID, pAvatar_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(position / 40000).");

	Property* pAvatar_direction = new Property();
	pAvatar_direction->name = TEXT("direction");
	pAvatar_direction->properUtype = 40001;
	pAvatar_direction->properFlags = 4;
	pAvatar_direction->aliasID = 2;
	KBVar* pAvatar_direction_defval = new KBVar(FVector());
	pAvatar_direction->pDefaultVal = pAvatar_direction_defval;
	pAvatarModule->propertys.Add(TEXT("direction"), pAvatar_direction); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_direction->aliasID, pAvatar_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(direction / 40001).");

	Property* pAvatar_spaceID = new Property();
	pAvatar_spaceID->name = TEXT("spaceID");
	pAvatar_spaceID->properUtype = 40002;
	pAvatar_spaceID->properFlags = 16;
	pAvatar_spaceID->aliasID = 3;
	KBVar* pAvatar_spaceID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("")));
	pAvatar_spaceID->pDefaultVal = pAvatar_spaceID_defval;
	pAvatarModule->propertys.Add(TEXT("spaceID"), pAvatar_spaceID); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_spaceID->aliasID, pAvatar_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(spaceID / 40002).");

	Property* pAvatar_modelID = new Property();
	pAvatar_modelID->name = TEXT("modelID");
	pAvatar_modelID->properUtype = 41006;
	pAvatar_modelID->properFlags = 4;
	pAvatar_modelID->aliasID = 4;
	KBVar* pAvatar_modelID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pAvatar_modelID->pDefaultVal = pAvatar_modelID_defval;
	pAvatarModule->propertys.Add(TEXT("modelID"), pAvatar_modelID); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_modelID->aliasID, pAvatar_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(modelID / 41006).");

	Property* pAvatar_modelScale = new Property();
	pAvatar_modelScale->name = TEXT("modelScale");
	pAvatar_modelScale->properUtype = 41007;
	pAvatar_modelScale->properFlags = 4;
	pAvatar_modelScale->aliasID = 5;
	KBVar* pAvatar_modelScale_defval = new KBVar((uint8)FCString::Atoi64(TEXT("30")));
	pAvatar_modelScale->pDefaultVal = pAvatar_modelScale_defval;
	pAvatarModule->propertys.Add(TEXT("modelScale"), pAvatar_modelScale); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_modelScale->aliasID, pAvatar_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(modelScale / 41007).");

	Property* pAvatar_name = new Property();
	pAvatar_name->name = TEXT("name");
	pAvatar_name->properUtype = 41003;
	pAvatar_name->properFlags = 4;
	pAvatar_name->aliasID = 6;
	KBVar* pAvatar_name_defval = new KBVar(FString());
	pAvatar_name->pDefaultVal = pAvatar_name_defval;
	pAvatarModule->propertys.Add(TEXT("name"), pAvatar_name); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_name->aliasID, pAvatar_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(name / 41003).");

	Property* pAvatar_uid = new Property();
	pAvatar_uid->name = TEXT("uid");
	pAvatar_uid->properUtype = 41004;
	pAvatar_uid->properFlags = 4;
	pAvatar_uid->aliasID = 7;
	KBVar* pAvatar_uid_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pAvatar_uid->pDefaultVal = pAvatar_uid_defval;
	pAvatarModule->propertys.Add(TEXT("uid"), pAvatar_uid); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_uid->aliasID, pAvatar_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(uid / 41004).");

	Property* pAvatar_utype = new Property();
	pAvatar_utype->name = TEXT("utype");
	pAvatar_utype->properUtype = 41005;
	pAvatar_utype->properFlags = 4;
	pAvatar_utype->aliasID = 8;
	KBVar* pAvatar_utype_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pAvatar_utype->pDefaultVal = pAvatar_utype_defval;
	pAvatarModule->propertys.Add(TEXT("utype"), pAvatar_utype); 

	pAvatarModule->usePropertyDescrAlias = true;
	pAvatarModule->idpropertys.Add((uint16)pAvatar_utype->aliasID, pAvatar_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), property(utype / 41005).");

	TArray<DATATYPE_BASE*> Avatar_onStopCardSelection_args;

	Method* pAvatar_onStopCardSelection = new Method();
	pAvatar_onStopCardSelection->name = TEXT("onStopCardSelection");
	pAvatar_onStopCardSelection->methodUtype = 14;
	pAvatar_onStopCardSelection->aliasID = 1;
	pAvatar_onStopCardSelection->args = Avatar_onStopCardSelection_args;

	pAvatarModule->methods.Add(TEXT("onStopCardSelection"), pAvatar_onStopCardSelection); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onStopCardSelection->aliasID, pAvatar_onStopCardSelection);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onStopCardSelection / 14).");

	TArray<DATATYPE_BASE*> Avatar_onSyncChangeHandCardSuccess_args;
	Avatar_onSyncChangeHandCardSuccess_args.Add(EntityDef::id2datatypes[2]);
	Avatar_onSyncChangeHandCardSuccess_args.Add(EntityDef::id2datatypes[1]);
	Avatar_onSyncChangeHandCardSuccess_args.Add(EntityDef::id2datatypes[1]);

	Method* pAvatar_onSyncChangeHandCardSuccess = new Method();
	pAvatar_onSyncChangeHandCardSuccess->name = TEXT("onSyncChangeHandCardSuccess");
	pAvatar_onSyncChangeHandCardSuccess->methodUtype = 11;
	pAvatar_onSyncChangeHandCardSuccess->aliasID = 2;
	pAvatar_onSyncChangeHandCardSuccess->args = Avatar_onSyncChangeHandCardSuccess_args;

	pAvatarModule->methods.Add(TEXT("onSyncChangeHandCardSuccess"), pAvatar_onSyncChangeHandCardSuccess); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onSyncChangeHandCardSuccess->aliasID, pAvatar_onSyncChangeHandCardSuccess);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onSyncChangeHandCardSuccess / 11).");

	TArray<DATATYPE_BASE*> Avatar_onSyncExhaustCardReplacement_args;

	Method* pAvatar_onSyncExhaustCardReplacement = new Method();
	pAvatar_onSyncExhaustCardReplacement->name = TEXT("onSyncExhaustCardReplacement");
	pAvatar_onSyncExhaustCardReplacement->methodUtype = 10;
	pAvatar_onSyncExhaustCardReplacement->aliasID = 3;
	pAvatar_onSyncExhaustCardReplacement->args = Avatar_onSyncExhaustCardReplacement_args;

	pAvatarModule->methods.Add(TEXT("onSyncExhaustCardReplacement"), pAvatar_onSyncExhaustCardReplacement); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onSyncExhaustCardReplacement->aliasID, pAvatar_onSyncExhaustCardReplacement);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onSyncExhaustCardReplacement / 10).");

	TArray<DATATYPE_BASE*> Avatar_onSyncPlayerBattleInfo_args;
	Avatar_onSyncPlayerBattleInfo_args.Add(EntityDef::id2datatypes[39]);

	Method* pAvatar_onSyncPlayerBattleInfo = new Method();
	pAvatar_onSyncPlayerBattleInfo->name = TEXT("onSyncPlayerBattleInfo");
	pAvatar_onSyncPlayerBattleInfo->methodUtype = 9;
	pAvatar_onSyncPlayerBattleInfo->aliasID = 4;
	pAvatar_onSyncPlayerBattleInfo->args = Avatar_onSyncPlayerBattleInfo_args;

	pAvatarModule->methods.Add(TEXT("onSyncPlayerBattleInfo"), pAvatar_onSyncPlayerBattleInfo); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onSyncPlayerBattleInfo->aliasID, pAvatar_onSyncPlayerBattleInfo);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onSyncPlayerBattleInfo / 9).");

	TArray<DATATYPE_BASE*> Avatar_onSyncRoomStartBattle_args;

	Method* pAvatar_onSyncRoomStartBattle = new Method();
	pAvatar_onSyncRoomStartBattle->name = TEXT("onSyncRoomStartBattle");
	pAvatar_onSyncRoomStartBattle->methodUtype = 13;
	pAvatar_onSyncRoomStartBattle->aliasID = 5;
	pAvatar_onSyncRoomStartBattle->args = Avatar_onSyncRoomStartBattle_args;

	pAvatarModule->methods.Add(TEXT("onSyncRoomStartBattle"), pAvatar_onSyncRoomStartBattle); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onSyncRoomStartBattle->aliasID, pAvatar_onSyncRoomStartBattle);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onSyncRoomStartBattle / 13).");

	TArray<DATATYPE_BASE*> Avatar_onSyncUpdateSelectedCards_args;
	Avatar_onSyncUpdateSelectedCards_args.Add(EntityDef::id2datatypes[2]);
	Avatar_onSyncUpdateSelectedCards_args.Add(EntityDef::id2datatypes[39]);

	Method* pAvatar_onSyncUpdateSelectedCards = new Method();
	pAvatar_onSyncUpdateSelectedCards->name = TEXT("onSyncUpdateSelectedCards");
	pAvatar_onSyncUpdateSelectedCards->methodUtype = 12;
	pAvatar_onSyncUpdateSelectedCards->aliasID = 6;
	pAvatar_onSyncUpdateSelectedCards->args = Avatar_onSyncUpdateSelectedCards_args;

	pAvatarModule->methods.Add(TEXT("onSyncUpdateSelectedCards"), pAvatar_onSyncUpdateSelectedCards); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_onSyncUpdateSelectedCards->aliasID, pAvatar_onSyncUpdateSelectedCards);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(onSyncUpdateSelectedCards / 12).");

	TArray<DATATYPE_BASE*> Avatar_resumeBattle_args;

	Method* pAvatar_resumeBattle = new Method();
	pAvatar_resumeBattle->name = TEXT("resumeBattle");
	pAvatar_resumeBattle->methodUtype = 18;
	pAvatar_resumeBattle->aliasID = 7;
	pAvatar_resumeBattle->args = Avatar_resumeBattle_args;

	pAvatarModule->methods.Add(TEXT("resumeBattle"), pAvatar_resumeBattle); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_resumeBattle->aliasID, pAvatar_resumeBattle);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(resumeBattle / 18).");

	TArray<DATATYPE_BASE*> Avatar_startBattle_args;

	Method* pAvatar_startBattle = new Method();
	pAvatar_startBattle->name = TEXT("startBattle");
	pAvatar_startBattle->methodUtype = 16;
	pAvatar_startBattle->aliasID = 8;
	pAvatar_startBattle->args = Avatar_startBattle_args;

	pAvatarModule->methods.Add(TEXT("startBattle"), pAvatar_startBattle); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_startBattle->aliasID, pAvatar_startBattle);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(startBattle / 16).");

	TArray<DATATYPE_BASE*> Avatar_switchController_args;
	Avatar_switchController_args.Add(EntityDef::id2datatypes[2]);

	Method* pAvatar_switchController = new Method();
	pAvatar_switchController->name = TEXT("switchController");
	pAvatar_switchController->methodUtype = 17;
	pAvatar_switchController->aliasID = 9;
	pAvatar_switchController->args = Avatar_switchController_args;

	pAvatarModule->methods.Add(TEXT("switchController"), pAvatar_switchController); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_switchController->aliasID, pAvatar_switchController);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(switchController / 17).");

	TArray<DATATYPE_BASE*> Avatar_syncTimeInterval_args;
	Avatar_syncTimeInterval_args.Add(EntityDef::id2datatypes[36]);

	Method* pAvatar_syncTimeInterval = new Method();
	pAvatar_syncTimeInterval->name = TEXT("syncTimeInterval");
	pAvatar_syncTimeInterval->methodUtype = 15;
	pAvatar_syncTimeInterval->aliasID = 10;
	pAvatar_syncTimeInterval->args = Avatar_syncTimeInterval_args;

	pAvatarModule->methods.Add(TEXT("syncTimeInterval"), pAvatar_syncTimeInterval); 
	pAvatarModule->useMethodDescrAlias = true;
	pAvatarModule->idmethods.Add((uint16)pAvatar_syncTimeInterval->aliasID, pAvatar_syncTimeInterval);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(syncTimeInterval / 15).");

	TArray<DATATYPE_BASE*> Avatar_reqChangeSelectCard_args;
	Avatar_reqChangeSelectCard_args.Add(EntityDef::id2datatypes[1]);

	Method* pAvatar_reqChangeSelectCard = new Method();
	pAvatar_reqChangeSelectCard->name = TEXT("reqChangeSelectCard");
	pAvatar_reqChangeSelectCard->methodUtype = 6;
	pAvatar_reqChangeSelectCard->aliasID = -1;
	pAvatar_reqChangeSelectCard->args = Avatar_reqChangeSelectCard_args;

	pAvatarModule->methods.Add(TEXT("reqChangeSelectCard"), pAvatar_reqChangeSelectCard); 
	pAvatarModule->base_methods.Add(TEXT("reqChangeSelectCard"), pAvatar_reqChangeSelectCard);

	pAvatarModule->idbase_methods.Add(pAvatar_reqChangeSelectCard->methodUtype, pAvatar_reqChangeSelectCard);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(reqChangeSelectCard / 6).");

	TArray<DATATYPE_BASE*> Avatar_reqFinishSelectCards_args;

	Method* pAvatar_reqFinishSelectCards = new Method();
	pAvatar_reqFinishSelectCards->name = TEXT("reqFinishSelectCards");
	pAvatar_reqFinishSelectCards->methodUtype = 7;
	pAvatar_reqFinishSelectCards->aliasID = -1;
	pAvatar_reqFinishSelectCards->args = Avatar_reqFinishSelectCards_args;

	pAvatarModule->methods.Add(TEXT("reqFinishSelectCards"), pAvatar_reqFinishSelectCards); 
	pAvatarModule->base_methods.Add(TEXT("reqFinishSelectCards"), pAvatar_reqFinishSelectCards);

	pAvatarModule->idbase_methods.Add(pAvatar_reqFinishSelectCards->methodUtype, pAvatar_reqFinishSelectCards);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(reqFinishSelectCards / 7).");

	TArray<DATATYPE_BASE*> Avatar_reqUpdateSelectedCard_args;

	Method* pAvatar_reqUpdateSelectedCard = new Method();
	pAvatar_reqUpdateSelectedCard->name = TEXT("reqUpdateSelectedCard");
	pAvatar_reqUpdateSelectedCard->methodUtype = 8;
	pAvatar_reqUpdateSelectedCard->aliasID = -1;
	pAvatar_reqUpdateSelectedCard->args = Avatar_reqUpdateSelectedCard_args;

	pAvatarModule->methods.Add(TEXT("reqUpdateSelectedCard"), pAvatar_reqUpdateSelectedCard); 
	pAvatarModule->base_methods.Add(TEXT("reqUpdateSelectedCard"), pAvatar_reqUpdateSelectedCard);

	pAvatarModule->idbase_methods.Add(pAvatar_reqUpdateSelectedCard->methodUtype, pAvatar_reqUpdateSelectedCard);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Avatar), method(reqUpdateSelectedCard / 8).");

	ScriptModule* pGateModule = new ScriptModule("Gate", 3);
	EntityDef::moduledefs.Add(TEXT("Gate"), pGateModule);
	EntityDef::idmoduledefs.Add(3, pGateModule);

	Property* pGate_position = new Property();
	pGate_position->name = TEXT("position");
	pGate_position->properUtype = 40000;
	pGate_position->properFlags = 4;
	pGate_position->aliasID = 1;
	KBVar* pGate_position_defval = new KBVar(FVector());
	pGate_position->pDefaultVal = pGate_position_defval;
	pGateModule->propertys.Add(TEXT("position"), pGate_position); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_position->aliasID, pGate_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(position / 40000).");

	Property* pGate_direction = new Property();
	pGate_direction->name = TEXT("direction");
	pGate_direction->properUtype = 40001;
	pGate_direction->properFlags = 4;
	pGate_direction->aliasID = 2;
	KBVar* pGate_direction_defval = new KBVar(FVector());
	pGate_direction->pDefaultVal = pGate_direction_defval;
	pGateModule->propertys.Add(TEXT("direction"), pGate_direction); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_direction->aliasID, pGate_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(direction / 40001).");

	Property* pGate_spaceID = new Property();
	pGate_spaceID->name = TEXT("spaceID");
	pGate_spaceID->properUtype = 40002;
	pGate_spaceID->properFlags = 16;
	pGate_spaceID->aliasID = 3;
	KBVar* pGate_spaceID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("")));
	pGate_spaceID->pDefaultVal = pGate_spaceID_defval;
	pGateModule->propertys.Add(TEXT("spaceID"), pGate_spaceID); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_spaceID->aliasID, pGate_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(spaceID / 40002).");

	Property* pGate_modelID = new Property();
	pGate_modelID->name = TEXT("modelID");
	pGate_modelID->properUtype = 41006;
	pGate_modelID->properFlags = 4;
	pGate_modelID->aliasID = 4;
	KBVar* pGate_modelID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pGate_modelID->pDefaultVal = pGate_modelID_defval;
	pGateModule->propertys.Add(TEXT("modelID"), pGate_modelID); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_modelID->aliasID, pGate_modelID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(modelID / 41006).");

	Property* pGate_modelScale = new Property();
	pGate_modelScale->name = TEXT("modelScale");
	pGate_modelScale->properUtype = 41007;
	pGate_modelScale->properFlags = 4;
	pGate_modelScale->aliasID = 5;
	KBVar* pGate_modelScale_defval = new KBVar((uint8)FCString::Atoi64(TEXT("30")));
	pGate_modelScale->pDefaultVal = pGate_modelScale_defval;
	pGateModule->propertys.Add(TEXT("modelScale"), pGate_modelScale); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_modelScale->aliasID, pGate_modelScale);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(modelScale / 41007).");

	Property* pGate_name = new Property();
	pGate_name->name = TEXT("name");
	pGate_name->properUtype = 41003;
	pGate_name->properFlags = 4;
	pGate_name->aliasID = 6;
	KBVar* pGate_name_defval = new KBVar(FString());
	pGate_name->pDefaultVal = pGate_name_defval;
	pGateModule->propertys.Add(TEXT("name"), pGate_name); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_name->aliasID, pGate_name);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(name / 41003).");

	Property* pGate_uid = new Property();
	pGate_uid->name = TEXT("uid");
	pGate_uid->properUtype = 41004;
	pGate_uid->properFlags = 4;
	pGate_uid->aliasID = 7;
	KBVar* pGate_uid_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pGate_uid->pDefaultVal = pGate_uid_defval;
	pGateModule->propertys.Add(TEXT("uid"), pGate_uid); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_uid->aliasID, pGate_uid);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(uid / 41004).");

	Property* pGate_utype = new Property();
	pGate_utype->name = TEXT("utype");
	pGate_utype->properUtype = 41005;
	pGate_utype->properFlags = 4;
	pGate_utype->aliasID = 8;
	KBVar* pGate_utype_defval = new KBVar((uint32)FCString::Atoi64(TEXT("0")));
	pGate_utype->pDefaultVal = pGate_utype_defval;
	pGateModule->propertys.Add(TEXT("utype"), pGate_utype); 

	pGateModule->usePropertyDescrAlias = true;
	pGateModule->idpropertys.Add((uint16)pGate_utype->aliasID, pGate_utype);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Gate), property(utype / 41005).");

	pGateModule->useMethodDescrAlias = true;
	ScriptModule* pTestEntityModule = new ScriptModule("TestEntity", 4);
	EntityDef::moduledefs.Add(TEXT("TestEntity"), pTestEntityModule);
	EntityDef::idmoduledefs.Add(4, pTestEntityModule);

	Property* pTestEntity_position = new Property();
	pTestEntity_position->name = TEXT("position");
	pTestEntity_position->properUtype = 40000;
	pTestEntity_position->properFlags = 4;
	pTestEntity_position->aliasID = 1;
	KBVar* pTestEntity_position_defval = new KBVar(FVector());
	pTestEntity_position->pDefaultVal = pTestEntity_position_defval;
	pTestEntityModule->propertys.Add(TEXT("position"), pTestEntity_position); 

	pTestEntityModule->usePropertyDescrAlias = true;
	pTestEntityModule->idpropertys.Add((uint16)pTestEntity_position->aliasID, pTestEntity_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestEntity), property(position / 40000).");

	Property* pTestEntity_direction = new Property();
	pTestEntity_direction->name = TEXT("direction");
	pTestEntity_direction->properUtype = 40001;
	pTestEntity_direction->properFlags = 4;
	pTestEntity_direction->aliasID = 2;
	KBVar* pTestEntity_direction_defval = new KBVar(FVector());
	pTestEntity_direction->pDefaultVal = pTestEntity_direction_defval;
	pTestEntityModule->propertys.Add(TEXT("direction"), pTestEntity_direction); 

	pTestEntityModule->usePropertyDescrAlias = true;
	pTestEntityModule->idpropertys.Add((uint16)pTestEntity_direction->aliasID, pTestEntity_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestEntity), property(direction / 40001).");

	Property* pTestEntity_spaceID = new Property();
	pTestEntity_spaceID->name = TEXT("spaceID");
	pTestEntity_spaceID->properUtype = 40002;
	pTestEntity_spaceID->properFlags = 16;
	pTestEntity_spaceID->aliasID = 3;
	KBVar* pTestEntity_spaceID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("")));
	pTestEntity_spaceID->pDefaultVal = pTestEntity_spaceID_defval;
	pTestEntityModule->propertys.Add(TEXT("spaceID"), pTestEntity_spaceID); 

	pTestEntityModule->usePropertyDescrAlias = true;
	pTestEntityModule->idpropertys.Add((uint16)pTestEntity_spaceID->aliasID, pTestEntity_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestEntity), property(spaceID / 40002).");

	TArray<DATATYPE_BASE*> TestEntity_onReqTestProperty_args;
	TestEntity_onReqTestProperty_args.Add(EntityDef::id2datatypes[28]);

	Method* pTestEntity_onReqTestProperty = new Method();
	pTestEntity_onReqTestProperty->name = TEXT("onReqTestProperty");
	pTestEntity_onReqTestProperty->methodUtype = 10003;
	pTestEntity_onReqTestProperty->aliasID = 1;
	pTestEntity_onReqTestProperty->args = TestEntity_onReqTestProperty_args;

	pTestEntityModule->methods.Add(TEXT("onReqTestProperty"), pTestEntity_onReqTestProperty); 
	pTestEntityModule->useMethodDescrAlias = true;
	pTestEntityModule->idmethods.Add((uint16)pTestEntity_onReqTestProperty->aliasID, pTestEntity_onReqTestProperty);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestEntity), method(onReqTestProperty / 10003).");

	TArray<DATATYPE_BASE*> TestEntity_reqTestProperty_args;

	Method* pTestEntity_reqTestProperty = new Method();
	pTestEntity_reqTestProperty->name = TEXT("reqTestProperty");
	pTestEntity_reqTestProperty->methodUtype = 10001;
	pTestEntity_reqTestProperty->aliasID = -1;
	pTestEntity_reqTestProperty->args = TestEntity_reqTestProperty_args;

	pTestEntityModule->methods.Add(TEXT("reqTestProperty"), pTestEntity_reqTestProperty); 
	pTestEntityModule->base_methods.Add(TEXT("reqTestProperty"), pTestEntity_reqTestProperty);

	pTestEntityModule->idbase_methods.Add(pTestEntity_reqTestProperty->methodUtype, pTestEntity_reqTestProperty);

	//DEBUG_MSG("EntityDef::initScriptModules: add(TestEntity), method(reqTestProperty / 10001).");

	ScriptModule* pPlayerModule = new ScriptModule("Player", 5);
	EntityDef::moduledefs.Add(TEXT("Player"), pPlayerModule);
	EntityDef::idmoduledefs.Add(5, pPlayerModule);

	Property* pPlayer_position = new Property();
	pPlayer_position->name = TEXT("position");
	pPlayer_position->properUtype = 40000;
	pPlayer_position->properFlags = 4;
	pPlayer_position->aliasID = 1;
	KBVar* pPlayer_position_defval = new KBVar(FVector());
	pPlayer_position->pDefaultVal = pPlayer_position_defval;
	pPlayerModule->propertys.Add(TEXT("position"), pPlayer_position); 

	pPlayerModule->usePropertyDescrAlias = true;
	pPlayerModule->idpropertys.Add((uint16)pPlayer_position->aliasID, pPlayer_position);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Player), property(position / 40000).");

	Property* pPlayer_direction = new Property();
	pPlayer_direction->name = TEXT("direction");
	pPlayer_direction->properUtype = 40001;
	pPlayer_direction->properFlags = 4;
	pPlayer_direction->aliasID = 2;
	KBVar* pPlayer_direction_defval = new KBVar(FVector());
	pPlayer_direction->pDefaultVal = pPlayer_direction_defval;
	pPlayerModule->propertys.Add(TEXT("direction"), pPlayer_direction); 

	pPlayerModule->usePropertyDescrAlias = true;
	pPlayerModule->idpropertys.Add((uint16)pPlayer_direction->aliasID, pPlayer_direction);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Player), property(direction / 40001).");

	Property* pPlayer_spaceID = new Property();
	pPlayer_spaceID->name = TEXT("spaceID");
	pPlayer_spaceID->properUtype = 40002;
	pPlayer_spaceID->properFlags = 16;
	pPlayer_spaceID->aliasID = 3;
	KBVar* pPlayer_spaceID_defval = new KBVar((uint32)FCString::Atoi64(TEXT("")));
	pPlayer_spaceID->pDefaultVal = pPlayer_spaceID_defval;
	pPlayerModule->propertys.Add(TEXT("spaceID"), pPlayer_spaceID); 

	pPlayerModule->usePropertyDescrAlias = true;
	pPlayerModule->idpropertys.Add((uint16)pPlayer_spaceID->aliasID, pPlayer_spaceID);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Player), property(spaceID / 40002).");

	TArray<DATATYPE_BASE*> Player_onUpdateBattle_args;
	Player_onUpdateBattle_args.Add(EntityDef::id2datatypes[34]);

	Method* pPlayer_onUpdateBattle = new Method();
	pPlayer_onUpdateBattle->name = TEXT("onUpdateBattle");
	pPlayer_onUpdateBattle->methodUtype = 19;
	pPlayer_onUpdateBattle->aliasID = 1;
	pPlayer_onUpdateBattle->args = Player_onUpdateBattle_args;

	pPlayerModule->methods.Add(TEXT("onUpdateBattle"), pPlayer_onUpdateBattle); 
	pPlayerModule->useMethodDescrAlias = true;
	pPlayerModule->idmethods.Add((uint16)pPlayer_onUpdateBattle->aliasID, pPlayer_onUpdateBattle);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Player), method(onUpdateBattle / 19).");

	TArray<DATATYPE_BASE*> Player_onUpdateGridInfoList_args;
	Player_onUpdateGridInfoList_args.Add(EntityDef::id2datatypes[31]);

	Method* pPlayer_onUpdateGridInfoList = new Method();
	pPlayer_onUpdateGridInfoList->name = TEXT("onUpdateGridInfoList");
	pPlayer_onUpdateGridInfoList->methodUtype = 20;
	pPlayer_onUpdateGridInfoList->aliasID = 2;
	pPlayer_onUpdateGridInfoList->args = Player_onUpdateGridInfoList_args;

	pPlayerModule->methods.Add(TEXT("onUpdateGridInfoList"), pPlayer_onUpdateGridInfoList); 
	pPlayerModule->useMethodDescrAlias = true;
	pPlayerModule->idmethods.Add((uint16)pPlayer_onUpdateGridInfoList->aliasID, pPlayer_onUpdateGridInfoList);

	//DEBUG_MSG("EntityDef::initScriptModules: add(Player), method(onUpdateGridInfoList / 20).");

}

void EntityDef::initDefTypes()
{
	{
		uint16 utype = 2;
		FString typeName = TEXT("ENTITY_SUBSTATE");
		FString name = TEXT("UINT8");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 3;
		FString typeName = TEXT("UINT16");
		FString name = TEXT("UINT16");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 5;
		FString typeName = TEXT("UID");
		FString name = TEXT("UINT64");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 4;
		FString typeName = TEXT("ENTITY_UTYPE");
		FString name = TEXT("UINT32");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 6;
		FString typeName = TEXT("ENTITY_STATE");
		FString name = TEXT("INT8");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 7;
		FString typeName = TEXT("INT16");
		FString name = TEXT("INT16");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 8;
		FString typeName = TEXT("ENTITY_FORBIDS");
		FString name = TEXT("INT32");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 9;
		FString typeName = TEXT("INT64");
		FString name = TEXT("INT64");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 1;
		FString typeName = TEXT("STRING");
		FString name = TEXT("STRING");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 12;
		FString typeName = TEXT("UNICODE");
		FString name = TEXT("UNICODE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 13;
		FString typeName = TEXT("FLOAT");
		FString name = TEXT("FLOAT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 14;
		FString typeName = TEXT("DOUBLE");
		FString name = TEXT("DOUBLE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		FString typeName = TEXT("UID1");
		FString name = TEXT("PYTHON");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		FString typeName = TEXT("PY_DICT");
		FString name = TEXT("PY_DICT");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		FString typeName = TEXT("PY_TUPLE");
		FString name = TEXT("PY_TUPLE");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 10;
		FString typeName = TEXT("PY_LIST");
		FString name = TEXT("PY_LIST");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 20;
		FString typeName = TEXT("ENTITYCALL");
		FString name = TEXT("ENTITYCALL");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 11;
		FString typeName = TEXT("BLOB");
		FString name = TEXT("BLOB");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 15;
		FString typeName = TEXT("VECTOR2");
		FString name = TEXT("VECTOR2");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 16;
		FString typeName = TEXT("DIRECTION3D");
		FString name = TEXT("VECTOR3");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 17;
		FString typeName = TEXT("VECTOR4");
		FString name = TEXT("VECTOR4");
		DATATYPE_BASE** fPtr = EntityDef::datatypes.Find(name);
		DATATYPE_BASE* pVal = fPtr != NULL ? *fPtr : NULL;
		EntityDef::datatypes.Add(typeName, pVal);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 22;
		FString typeName = TEXT("ENTITY_FORBID_COUNTER");
		DATATYPE_ENTITY_FORBID_COUNTER* pDatatype = new DATATYPE_ENTITY_FORBID_COUNTER();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 23;
		FString typeName = TEXT("ENTITYID_LIST");
		DATATYPE_ENTITYID_LIST* pDatatype = new DATATYPE_ENTITYID_LIST();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 24;
		FString typeName = TEXT("BAG");
		DATATYPE_BAG* pDatatype = new DATATYPE_BAG();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 27;
		FString typeName = TEXT("EXAMPLES");
		DATATYPE_EXAMPLES* pDatatype = new DATATYPE_EXAMPLES();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 28;
		FString typeName = TEXT("TEST_ENTITY_PROPS");
		DATATYPE_TEST_ENTITY_PROPS* pDatatype = new DATATYPE_TEST_ENTITY_PROPS();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 29;
		FString typeName = TEXT("PLAYER_ACTION");
		DATATYPE_PLAYER_ACTION* pDatatype = new DATATYPE_PLAYER_ACTION();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 30;
		FString typeName = TEXT("UPDATE_GRID_INFO");
		DATATYPE_UPDATE_GRID_INFO* pDatatype = new DATATYPE_UPDATE_GRID_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 31;
		FString typeName = TEXT("UPDATE_GRID_INFO_LIST");
		DATATYPE_UPDATE_GRID_INFO_LIST* pDatatype = new DATATYPE_UPDATE_GRID_INFO_LIST();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 33;
		FString typeName = TEXT("BATTLE_GRID_INFO");
		DATATYPE_BATTLE_GRID_INFO* pDatatype = new DATATYPE_BATTLE_GRID_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 34;
		FString typeName = TEXT("CORE_UPDATE_BATLLE_INFO");
		DATATYPE_CORE_UPDATE_BATLLE_INFO* pDatatype = new DATATYPE_CORE_UPDATE_BATLLE_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 36;
		FString typeName = TEXT("SYNC_BATTLE_TIME_INFO");
		DATATYPE_SYNC_BATTLE_TIME_INFO* pDatatype = new DATATYPE_SYNC_BATTLE_TIME_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 37;
		FString typeName = TEXT("SYNC_CARD_INFO");
		DATATYPE_SYNC_CARD_INFO* pDatatype = new DATATYPE_SYNC_CARD_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 39;
		FString typeName = TEXT("SYNC_PLAYER_BATTLE_INFO");
		DATATYPE_SYNC_PLAYER_BATTLE_INFO* pDatatype = new DATATYPE_SYNC_PLAYER_BATTLE_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	{
		uint16 utype = 42;
		FString typeName = TEXT("PLAYER_PERSIST_INFO");
		DATATYPE_PLAYER_PERSIST_INFO* pDatatype = new DATATYPE_PLAYER_PERSIST_INFO();
		EntityDef::datatypes.Add(typeName, (DATATYPE_BASE*)pDatatype);
		EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[typeName]);
		EntityDef::datatype2id.Add(typeName, utype);
	}

	for(auto& Elem : EntityDef::datatypes)
	{
		if(Elem.Value)
		{
			Elem.Value->bind();
		}
	}
}


}