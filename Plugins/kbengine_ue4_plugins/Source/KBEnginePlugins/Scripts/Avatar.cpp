#include "Avatar.h"
#include "Engine/Entity.h"
#include "LogicEvents.h"
#include "BattleEvents.h"

namespace KBEngine
{

				Avatar::Avatar() :
								AvatarBase()
				{
				}

				Avatar::~Avatar()
				{
				}

				void Avatar::__init__()
				{
								// 注册事件
								if (isPlayer())
								{
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqEnterRoom", "ReqEnterRoom", [this](const UKBEventData* pEventData)
																{
																				ReqEnterRoom();
																});
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqChangeSelectCard", "ReqChangeSelectCard", [this](const UKBEventData* pEventData)
																{
																				const UKBEventData_reqChangeSelectCard* reqChangeSelectCardData = Cast<UKBEventData_reqChangeSelectCard>(pEventData);
																				ReqChangeSelectCard(reqChangeSelectCardData->changeCardKey);
																});
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqFinishSelectCards", "ReqFinishSelectCards", [this](const UKBEventData* pEventData)
																{
																				ReqFinishSelectCards();
																});
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqUpdateSelectedCard", "ReqUpdateSelectedCard", [this](const UKBEventData* pEventData)
																{
																				ReqUpdateSelectedCard();
																});
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqSyncHeartBeat", "ReqSyncHeartBeat", [this](const UKBEventData* pEventData)
																{
																				ReqSyncHeartBeat();
																});
												KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("ReqLatestBattleInfo", "ReqLatestBattleInfo", [this](const UKBEventData* pEventData)
																{
																				ReqLatestBattleInfo();
																});
								}
				}

				void Avatar::onDestroy()
				{
								AvatarBase::onDestroy();

								// 注销注册的所有事件
								KBENGINE_DEREGISTER_ALL_EVENT();
				}

				void Avatar::onEnterWorld()
				{
								AvatarBase::onEnterWorld();
				}

				void Avatar::onLeaveWorld()
				{
								AvatarBase::onLeaveWorld();
				}

				void Avatar::onEnterSpace()
				{
								AvatarBase::onEnterSpace();
				}

				void Avatar::onLeaveSpace()
				{
								AvatarBase::onLeaveSpace();
				}

				void Avatar::onSyncPlayerBattleInfo(const SYNC_PLAYER_BATTLE_INFO& param)
				{
								UKBEventData_onSyncPlayerBattleInfo* eventData = NewObject<UKBEventData_onSyncPlayerBattleInfo>();
								for (int32 i = 0; i < param.cardList.Num(); i++)
								{
												FSYNC_CARD_INFO syncCardInfo;
												syncCardInfo.cardKey = param.cardList[i].cardKey;
												syncCardInfo.cardName = param.cardList[i].cardName;
												syncCardInfo.hp = param.cardList[i].hp;
												syncCardInfo.defence = param.cardList[i].defence;
												syncCardInfo.agility = param.cardList[i].agility;
												syncCardInfo.tags = param.cardList[i].tags;
												for (int32 j = 0; j < param.cardList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = param.cardList[i].stateTags[j].state;
																stateTagInfo.stipulation = param.cardList[i].stateTags[j].stipulation;
																stateTagInfo.curCount = param.cardList[i].stateTags[j].curCount;
																syncCardInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < param.cardList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = param.cardList[i].effectInfos[j].effectName;
																syncEffectInfo.availableTimes = param.cardList[i].effectInfos[j].availableTimes;
																syncEffectInfo.countDown = param.cardList[i].effectInfos[j].countDown;
																syncCardInfo.effectInfos.Add(syncEffectInfo);
												}
												eventData->cardList.Add(syncCardInfo);
								}
								eventData->handCardList = param.handCardList;

								KBENGINE_EVENT_FIRE("onSyncPlayerBattleInfo", eventData);
				}

				void Avatar::onSyncReceiveEnterRoom(uint8 result)
				{
								UKBEventData_onSyncReceiveEnterRoom* eventData = NewObject<UKBEventData_onSyncReceiveEnterRoom>();
								eventData->result = result;
								KBENGINE_EVENT_FIRE("onSyncReceiveEnterRoom", eventData);
				}

				void Avatar::onSyncReceiveFinishCardSelection()
				{
								UKBEventData* eventData = NewObject<UKBEventData>();
								KBENGINE_EVENT_FIRE("onSyncReceiveFinishCardSelection", eventData);
				}

				void Avatar::onSyncResumeBattle(int32 controllerNb)
				{
								UKBEventData_onSyncResumeBattle* eventData = NewObject<UKBEventData_onSyncResumeBattle>();
								eventData->controllerNb = controllerNb;
								KBENGINE_EVENT_FIRE("onSyncResumeBattle", eventData);
				}

				void Avatar::onSyncBattleResult(const STRING_LIST& losePlayerList)
				{
								UKBEventData_onSyncBattleResult* eventData = NewObject<UKBEventData_onSyncBattleResult>();
								eventData->losePlayerList = losePlayerList.stringList;
								KBENGINE_EVENT_FIRE("onSyncBattleResult", eventData);
				}

				void Avatar::onSyncChangeHandCardSuccess(uint8 changeCardNb, const FString& changeHandCardKey, const FString& pileCardKey)
				{
								UKBEventData_onSyncChangeHandCardSuccess* eventData = NewObject<UKBEventData_onSyncChangeHandCardSuccess>();
								eventData->changeSelectCardNb = changeCardNb;
								eventData->changeHandCardKey = changeHandCardKey;
								eventData->changePileCardKey = pileCardKey;
								KBENGINE_EVENT_FIRE("onSyncChangeHandCardSuccess", eventData);
				}

				void Avatar::onSyncExhaustCardReplacement()
				{
								UKBEventData* eventData = NewObject<UKBEventData>();
								KBENGINE_EVENT_FIRE("onSyncExhaustCardReplacement", eventData);
				}

				void Avatar::onSyncHeartBeat(int32 curBattleTick)
				{
								UKBEventData_onSyncHeartBeat* eventData = NewObject<UKBEventData_onSyncHeartBeat>();
								eventData->curBattleTick = curBattleTick;
								KBENGINE_EVENT_FIRE("onSyncHeartBeat", eventData);
				}

				void Avatar::onSyncLatestBattleState(const CORE_UPDATE_BATLLE_INFO& battleInfo)
				{
								UKBEventData_onSyncLatestBattleState* eventData = NewObject<UKBEventData_onSyncLatestBattleState>();
								eventData->curSwitchControllerSequence = battleInfo.curSwitchControllerSequence;
								eventData->curControllerNb = battleInfo.curControllerNb;
								eventData->curControllerAvatarId = FString::Printf(TEXT("%lld"), battleInfo.curControllerAvatarId);
								eventData->curActionSequence = battleInfo.curActionSequence;
								for (int32 i = 0; i < battleInfo.updateList.Num(); i++)
								{
												FBATTLE_GRID_INFO gridInfo;
												gridInfo.gridNb = battleInfo.updateList[i].gridNb;
												gridInfo.cardUid = battleInfo.updateList[i].cardUid;
												gridInfo.avatarId = FString::Printf(TEXT("%lld"), battleInfo.updateList[i].avatarId);
												eventData->updateGridInfos.Add(gridInfo);
								}
								for (int32 i = 0; i < battleInfo.playerInfo.cardList.Num(); i++)
								{
												FSYNC_CARD_INFO cardInfo;
												cardInfo.cardKey = battleInfo.playerInfo.cardList[i].cardKey;
												cardInfo.cardName = battleInfo.playerInfo.cardList[i].cardName;
												cardInfo.hp = battleInfo.playerInfo.cardList[i].hp;
												cardInfo.defence = battleInfo.playerInfo.cardList[i].defence;
												cardInfo.agility = battleInfo.playerInfo.cardList[i].agility;
												cardInfo.tags = battleInfo.playerInfo.cardList[i].tags;
												for (int32 j = 0; j < battleInfo.playerInfo.cardList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = battleInfo.playerInfo.cardList[i].stateTags[j].state;
																stateTagInfo.curCount = battleInfo.playerInfo.cardList[i].stateTags[j].curCount;
																stateTagInfo.stipulation = battleInfo.playerInfo.cardList[i].stateTags[j].stipulation;
																cardInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < battleInfo.playerInfo.cardList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = battleInfo.playerInfo.cardList[i].effectInfos[j].effectName;
																syncEffectInfo.availableTimes = battleInfo.playerInfo.cardList[i].effectInfos[j].availableTimes;
																syncEffectInfo.countDown = battleInfo.playerInfo.cardList[i].effectInfos[j].countDown;
																cardInfo.effectInfos.Add(syncEffectInfo);
												}
												eventData->cardList.Add(cardInfo);
								}
								eventData->handCardList = battleInfo.playerInfo.handCardList;
								KBENGINE_EVENT_FIRE("onSyncLatestBattleState", eventData);
				}

				void Avatar::onSyncLaunchSkillFailed(int32 curActionSequence, int32 clientActionSequence)
				{
								UKBEventData_onSyncLaunchSkillFailed* eventData = NewObject<UKBEventData_onSyncLaunchSkillFailed>();
								eventData->curActionSequence = curActionSequence;
								eventData->clientActionSequence = clientActionSequence;
								KBENGINE_EVENT_FIRE("onSyncLaunchSkillFailed", eventData);
				}

				void Avatar::onSyncUpdateSelectedCards(uint8 changeNb, const SYNC_PLAYER_BATTLE_INFO& allCardInfos)
				{
								UKBEventData_onSyncUpdateSelectedCards* eventData = NewObject<UKBEventData_onSyncUpdateSelectedCards>();
								eventData->changeNb = changeNb;
								for (int32 i = 0; i < allCardInfos.cardList.Num(); i++)
								{
												FSYNC_CARD_INFO syncCardInfo;
												syncCardInfo.cardKey = allCardInfos.cardList[i].cardKey;
												syncCardInfo.cardName = allCardInfos.cardList[i].cardName;
												syncCardInfo.hp = allCardInfos.cardList[i].hp;
												syncCardInfo.defence = allCardInfos.cardList[i].defence;
												syncCardInfo.agility = allCardInfos.cardList[i].agility;
												syncCardInfo.tags = allCardInfos.cardList[i].tags;
												for (int32 j = 0; j < allCardInfos.cardList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = allCardInfos.cardList[i].stateTags[j].state;
																stateTagInfo.curCount = allCardInfos.cardList[i].stateTags[j].curCount;
																stateTagInfo.stipulation = allCardInfos.cardList[i].stateTags[j].stipulation;
																syncCardInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < allCardInfos.cardList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = allCardInfos.cardList[i].effectInfos[j].effectName;
																syncEffectInfo.countDown = allCardInfos.cardList[i].effectInfos[j].countDown;
																syncEffectInfo.availableTimes = allCardInfos.cardList[i].effectInfos[j].availableTimes;
																syncCardInfo.effectInfos.Add(syncEffectInfo);
												}
												eventData->cardList.Add(syncCardInfo);
								}
								eventData->handCardList = allCardInfos.handCardList;
								KBENGINE_EVENT_FIRE("onSyncUpdateSelectedCards", eventData);
				}

				void Avatar::onSyncRoomStartBattle()
				{
								UKBEventData* eventData = NewObject<UKBEventData>();
								KBENGINE_EVENT_FIRE("onSyncRoomStartBattle", eventData);
				}

				void Avatar::onSyncSelectCardInterlude(const SYNC_PLAYER_BATTLE_INFO& playerInfo)
				{
								UKBEventData_onSyncSelectCardInterlude* eventData = NewObject<UKBEventData_onSyncSelectCardInterlude>();
								for (int32 i = 0; i < playerInfo.cardList.Num(); i++)
								{
												FSYNC_CARD_INFO syncCardInfo;
												syncCardInfo.cardKey = playerInfo.cardList[i].cardKey;
												syncCardInfo.cardName = playerInfo.cardList[i].cardName;
												syncCardInfo.hp = playerInfo.cardList[i].hp;
												syncCardInfo.defence = playerInfo.cardList[i].defence;
												syncCardInfo.agility = playerInfo.cardList[i].agility;
												syncCardInfo.tags = playerInfo.cardList[i].tags;
												for (int32 j = 0; j < playerInfo.cardList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = playerInfo.cardList[i].stateTags[j].state;
																stateTagInfo.curCount = playerInfo.cardList[i].stateTags[j].curCount;
																stateTagInfo.stipulation = playerInfo.cardList[i].stateTags[j].stipulation;
																syncCardInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < playerInfo.cardList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = playerInfo.cardList[i].effectInfos[j].effectName;
																syncEffectInfo.availableTimes = playerInfo.cardList[i].effectInfos[j].availableTimes;
																syncEffectInfo.countDown = playerInfo.cardList[i].effectInfos[j].countDown;
																syncCardInfo.effectInfos.Add(syncEffectInfo);
												}
								}
								eventData->handCardList = playerInfo.handCardList;
								KBENGINE_EVENT_FIRE("onSyncSelectCardInterlude", eventData);
				}

				void Avatar::onSyncSwitchController(int32 controllerNb, uint64 avatarId)
				{
								UKBEventData_onSyncSwitchController* eventData = NewObject<UKBEventData_onSyncSwitchController>();
								eventData->controllerNb = controllerNb;
								eventData->avatarId = FString::Printf(TEXT("%lld"), avatarId);
								KBENGINE_EVENT_FIRE("onSyncSwitchController", eventData);
				}

				void Avatar::onSyncTimeInterval(const SYNC_BATTLE_TIME_INFO& syncTimeInfo)
				{
								UKBEventData_onSyncTimeInterval* eventData = NewObject<UKBEventData_onSyncTimeInterval>();
								eventData->curTime = syncTimeInfo.curTime;
								eventData->battleState = syncTimeInfo.battleState;
								KBENGINE_EVENT_FIRE("onSyncTimeInterval", eventData);
				}

				void Avatar::onSyncUpdateActionInfo(const SYNC_MODIFICATION_INFO& modificationInfo)
				{
								UKBEventData_onSyncUpdateActionInfo* eventData = NewObject<UKBEventData_onSyncUpdateActionInfo>();
								eventData->actionSequence = modificationInfo.actionSequence;
								for (int32 i = 0; i < modificationInfo.updateCardList.Num(); i++)
								{
												FSYNC_CARD_INFO syncCardInfo;
												syncCardInfo.cardKey = modificationInfo.updateCardList[i].cardKey;
												syncCardInfo.cardName = modificationInfo.updateCardList[i].cardName;
												syncCardInfo.hp = modificationInfo.updateCardList[i].hp;
												syncCardInfo.defence = modificationInfo.updateCardList[i].defence;
												syncCardInfo.agility = modificationInfo.updateCardList[i].agility;
												syncCardInfo.tags = modificationInfo.updateCardList[i].tags;
												for (int32 j = 0; j < modificationInfo.updateCardList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = modificationInfo.updateCardList[i].stateTags[j].state;
																stateTagInfo.curCount = modificationInfo.updateCardList[i].stateTags[j].curCount;
																stateTagInfo.stipulation = modificationInfo.updateCardList[i].stateTags[j].stipulation;
																syncCardInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < modificationInfo.updateCardList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = modificationInfo.updateCardList[i].effectInfos[j].effectName;
																syncEffectInfo.countDown = modificationInfo.updateCardList[i].effectInfos[j].countDown;
																syncEffectInfo.availableTimes = modificationInfo.updateCardList[i].effectInfos[j].availableTimes;
																syncCardInfo.effectInfos.Add(syncEffectInfo);
												}
												eventData->updateCardInfos.Add(syncCardInfo);
								}

								for (int32 i = 0; i < modificationInfo.updateGridList.Num(); i++)
								{
												FBATTLE_GRID_INFO gridInfo;
												gridInfo.gridNb = modificationInfo.updateGridList[i].gridNb;
												gridInfo.cardUid = modificationInfo.updateGridList[i].cardUid;
												gridInfo.cardName = modificationInfo.updateGridList[i].cardName;
												gridInfo.hp = modificationInfo.updateGridList[i].hp;
												gridInfo.defence = modificationInfo.updateGridList[i].defence;
												gridInfo.agility = modificationInfo.updateGridList[i].agility;
												gridInfo.tags = modificationInfo.updateGridList[i].tags;
												for (int32 j = 0; j < modificationInfo.updateGridList[i].stateTags.Num(); j++)
												{
																FSTATE_TAG_INFO stateTagInfo;
																stateTagInfo.stateName = modificationInfo.updateGridList[i].stateTags[j].state;
																stateTagInfo.curCount = modificationInfo.updateGridList[i].stateTags[j].curCount;
																stateTagInfo.stipulation = modificationInfo.updateGridList[i].stateTags[j].stipulation;
																gridInfo.stateTags.Add(stateTagInfo);
												}
												for (int32 j = 0; j < modificationInfo.updateGridList[i].effectInfos.Num(); j++)
												{
																FSYNC_EFFECT_INFO syncEffectInfo;
																syncEffectInfo.effectName = modificationInfo.updateGridList[i].effectInfos[j].effectName;
																syncEffectInfo.availableTimes = modificationInfo.updateGridList[i].effectInfos[j].availableTimes;
																syncEffectInfo.countDown = modificationInfo.updateGridList[i].effectInfos[j].countDown;
																gridInfo.effectInfos.Add(syncEffectInfo);
												}
												gridInfo.avatarId = FString::Printf(TEXT("%lld"), modificationInfo.updateGridList[i].avatarId);
												eventData->gridInfos.Add(gridInfo);
								}
								KBENGINE_EVENT_FIRE("onSyncUpdateActionInfo", eventData);
				}

				void Avatar::ReqChangeSelectCard(FString changeCardKey)
				{
								pBaseEntityCall->reqChangeSelectCard(changeCardKey);
				}

				void Avatar::ReqFinishSelectCards()
				{
								pBaseEntityCall->reqFinishSelectCards();
				}

				void Avatar::ReqUpdateSelectedCard()
				{
								pBaseEntityCall->reqUpdateSelectedCard();
				}

				void Avatar::ReqSyncHeartBeat()
				{
								pBaseEntityCall->reqSyncHeartBeat();
				}

				void Avatar::ReqLatestBattleInfo()
				{
								pBaseEntityCall->reqLatestBattleInfo();
				}

				void Avatar::ReqEnterRoom()
				{
								pBaseEntityCall->reqEnterRoom();
				}
}