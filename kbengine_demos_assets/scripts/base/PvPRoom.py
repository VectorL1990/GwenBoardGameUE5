# -*- coding: utf-8 -*-
import KBEngine
import random
import math
import time
from KBEDebug import *
import GlobalConst
from GetActions import actionIdConverterDict
from d_all_cards import allDatas
from d_effects import effect_dict
from d_passive_effects import passive_effect_dict

class PvPRoom(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.state_list = [['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--']]
		self.curBattleTick = 0
		self.accountEntityDict = {}
		self.gridInfoDict = {}
		self.curSwitchNb = 0
		self.curActionSequence = 0
		self.curControlNb = 0

		# --- time counting variables
		self.curTimeClockInterval = 0
		self.maxWaitAvatarsEnterRoom = 30
		self.maxSelectCardTimeInterval = 30
		self.maxSelectCardInterludeTime = 5
		self.maxLaunchActionTimeInterval = 20
		self.maxSwitchControllerInterludeTime = 4

		self.avatars = {}
		self.inBattleAvatarList = []
		self.avatarsHeartBeatCount = {}

		self.uniqueCardDict = {}
		self.battleState = GlobalConst.g_battleState.DEFAULT

	def roomDestroySelf(self):
		KBEngine.globalData["Hall"].roomReqDelete(self.roomKey)
		# tell all players that room is destroied
		self.accountEntityDict = {}
		self.avatars = {}
		self.uniqueCardDict = {}
		self.inBattleAvatarList = []
		self.destroy()
		return

	def hallReqAccountsRoomCreated(self):
		for k,v in self.accountEntityDict.items():
			DEBUG_MSG("PvPRoom::hallReqAccountsRoomCreated: tell account room created and entityID=%i" % (k))
			v.syncRoomCreated(self.roomKey)
		
		# at this point we could countdown for avatar entering room
		self.battleState = GlobalConst.g_battleState.WAIT_AVATAR_ENTER_ROOM
		self.addTimer(1.0, 1.0, 2)

	def avatarEnterRoom(self, avatarEntityCall):
		if avatarEntityCall.id not in self.avatars:
			DEBUG_MSG("PvPRoom::avatarEnterRoom: avatar enter room and entityID=%i" % (avatarEntityCall.id))
			self.avatars[avatarEntityCall.id] = avatarEntityCall
			self.avatarsHeartBeatCount[avatarEntityCall.id] = 0
			# get corresponding card info from data list
			for cardKey in avatarEntityCall.allCardDict.keys():
				strs = cardKey.split("_")
				cardInfo = allDatas["allCards"][strs[2]]
				#cardInfo = d_all_cards.datas.get(strs[1])
				avatarEntityCall.allCardDict[cardKey] = {
					"cardName": strs[1],
					"hp": cardInfo["hp"],
					"defence": cardInfo["defence"],
					"agility": cardInfo["agility"],
					"tags": cardInfo["tags"],
					"stateTags": [],
					"effects": []
				}
				for effectKey, effectValue in cardInfo["effects"].items():
					effectInfoDict = {
						"effectName": effectKey,
						"countDown": effectValue["countDown"],
						"availableTimes": effectValue["availableTimes"]
					}
					avatarEntityCall.allCardDict[cardKey]["effects"].append(effectInfoDict)
				self.uniqueCardDict[cardKey] = avatarEntityCall.allCardDict[cardKey]
				self.uniqueCardDict[cardKey]["avatarId"] = avatarEntityCall.id


	def avatarFinishSelectCards(self, avatarEntityCall):
		if avatarEntityCall not in self.inBattleAvatarList:
			self.inBattleAvatarList.append(avatarEntityCall)
			finishSelectCardAvatarNb = len(self.inBattleAvatarList)
			if (len(self.enterRoomAccountList) == finishSelectCardAvatarNb):
				# which means all avatars finish card selection
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSelectCardInterlude()
				self.battleState = GlobalConst.g_battleState.SELECT_CARD_INTERLUDE
				random.shuffle(self.inBattleAvatarList)
				self.curTimeClockInterval = 0

	def avatarReqHeartBeat(self, avatarEntityCall):
		# avatar should send heart beat to room to keep alive
		if avatarEntityCall.id in self.avatarsHeartBeatCount:
			self.avatarsHeartBeatCount[avatarEntityCall.id] = 0

	def avatarReqLatestBattleInfo(self, avatarEntityCall):
		if avatarEntityCall.id in self.avatars:
			coreUpdateBattleInfo = {
				"curActionSequence": self.curActionSequence,
				"updateList": self.state_list,
				"playerInfo": {},
				"curSwitchControllerSequence": self.curSwitchNb,
				"curControllerNb": self.curControlNb,
				"curControllerAvatarId": self.inBattleAvatarList[self.curControlNb].id
			}
			avatarEntityCall.roomReqUpdateLatestBattleInfo(coreUpdateBattleInfo)

	# launchEffect(self, launchAvatarId, targetGrid, launchGrid, effectName, effectInfo)
	def avatarReqPlayCardAction(self, avatarEntityCall, avatarClientActionSequence, cardUid, gridNb):
		actionSequenceLatency = self.curActionSequence - avatarClientActionSequence
		if actionSequenceLatency == 1:
			actionId = actionIdConverterDict["GetPlayCardActionId"]()
			self.board.DoMove(actionId)
			# which means there's no information lost on client side
			# find corresponding card info from card dictionary, including effects attached to this card
			#for k,v in self.uniqueCardDict[cardUid]["effects"].items():
			#	if v["launchType"] == "auto":
			#		self.launchEffect(avatarClientActionSequence, -1, gridNb, k, v)


	def avatarReqLaunchCardSkillAction(self, avatarEntityCall, clientActionSequence, cardUid, skillName, launchGridNb, targetGridNb):
		actionSequenceLatency = self.curActionSequence - clientActionSequence
		if actionSequenceLatency == 1:
			actionId = actionIdConverterDict["GetLaunchSkillActionId"]
			self.board.DoMove(actionId)
			# which means there's no information lost on client side
			#if skillName in self.uniqueCardDict[cardUid]["effects"]:
			#	if self.uniqueCardDict[cardUid]["effects"][skillName]["availableTimes"] >= 1:
			#		# which means this skill is still available to launch
			#		self.launchEffect(clientActionSequence, targetGridNb, launchGridNb, skillName, self.uniqueCardDict[cardUid]["effects"][skillName])


	def leaveRoom(self, entityID):
		self.onLeave(entityID)

		
	def onTimer(self, tid, userArg):
		if self.battleState == GlobalConst.g_battleState.DEFAULT:
			return

		if self.battleState == GlobalConst.g_battleState.WAIT_AVATAR_ENTER_ROOM:
			if self.curTimeClockInterval >= self.maxWaitAvatarsEnterRoom:
				# countdown for all avatars entering room
				# if there are still accounts out of room when clock comes to the end
				# room dismissed
				self.roomDestroySelf()
			else:
				if len(self.avatars) == len(self.accountEntityDict):
					# which means all avatars have entered room
					# dispatch informations to all players
					for avatar in self.avatars:
						self.avatars[avatar].roomReqDispatchCardInfos()
					# switch battle state to select card
					self.battleState = GlobalConst.g_battleState.SELECT_CARD
					self.curTimeClockInterval = 0
				else:
					self.curTimeClockInterval += 1
		elif self.battleState == GlobalConst.g_battleState.SELECT_CARD:
			if self.curTimeClockInterval >= self.maxSelectCardTimeInterval:
				# force all players stopping card selection
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSelectCardInterlude()
				self.battleState = GlobalConst.g_battleState.SELECT_CARD_INTERLUDE
				random.shuffle(self.inBattleAvatarList)
				self.curTimeClockInterval = 0
			else:
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSyncTimeInfo(self.curTimeClockInterval, self.battleState.value)
				self.curTimeClockInterval += 1
		elif self.battleState == GlobalConst.g_battleState.SELECT_CARD_INTERLUDE:
			if self.curTimeClockInterval >= self.maxSelectCardInterludeTime:
				# start battle
				for avatar in self.avatars:
					self.avatars[avatar].roomReqStartBattle()
				self.battleState = GlobalConst.g_battleState.BATTLE
				self.curTimeClockInterval = 0
			else:
				self.curTimeClockInterval += 1
		elif self.battleState == GlobalConst.g_battleState.BATTLE:
			self.curBattleTick += 1
			self.updateAvatarHeartBeat()
			if self.curTimeClockInterval >= self.maxLaunchActionTimeInterval:
				# which means we should modify controller to another player
				self.curSwitchNb += 1
				self.curControlNb += 1
				if self.curControlNb >= len(self.inBattleAvatarList):
					self.curControlNb = 0
				# send messages to all proxys to switch controller
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSwitchController(self.curSwitchNb, self.inBattleAvatarList[self.curControlNb].id)
				self.curTimeClockInterval = 0
				self.battleState = GlobalConst.g_battleState.BATTLE_INTERLUDE
			else:
				self.curTimeClockInterval += 1
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSyncTimeInfo(self.curTimeClockInterval, self.battleState.value)
		elif self.battleState == GlobalConst.g_battleState.BATTLE_INTERLUDE:
			self.curBattleTick += 1
			self.updateAvatarHeartBeat()
			if self.curTimeClockInterval >= self.maxSwitchControllerInterludeTime:
				for avatar in self.avatars:
					self.avatars[avatar].roomReqResumeBattle(self.curSwitchNb)
				self.curTimeClockInterval = 0.0
				self.battleState = GlobalConst.g_battleState.BATTLE
			else:
				self.curTimeClockInterval += 1

			

		
		
	def onLeave(self, entityID):
		"""
		defined method.
		离开场景
		"""
		if entityID in self.avatars:
			del self.avatars[entityID]


	def updateAvatarHeartBeat(self):
		lostAvatars = []
		for avatar in self.avatarsHeartBeatCount:
			if self.avatarsHeartBeatCount[avatar] > GlobalConst.g_maxHeartBeatCount:
				# which means this avatar is loss
				# in this case it judges this player lose
				lostAvatars.append(avatar)
			else:
				self.avatarsHeartBeatCount[avatar] += 1
		
		# sync result to all active avatars 
		for avatar in self.avatars:
			self.avatars[avatar].roomReqSyncBattleResult(lostAvatars)
			self.avatars[avatar].roomReqAvatarDie()

		# notify hall to record battle result

	
	def getGridNbByRowAndCol(self, row, col):
		if row < 0 or row > 2*GlobalConst.g_boardHalfRow:
			return -1
		elif col < 0 or col > GlobalConst.g_boardColumn:
			return -1
		else:
			return row*GlobalConst.g_boardColumn + col

	def calculateCardHp(self, cardUid, hurt):
		# return dead or not
		if hurt <= self.uniqueCardDict[cardUid]["defence"]:
			self.uniqueCardDict[cardUid]["defence"] -= hurt
			return False
		else:
			overflow = hurt - self.uniqueCardDict[cardUid]["defence"]
			if overflow < self.uniqueCardDict[cardUid]["hp"]:
				self.uniqueCardDict[cardUid]["hp"] -= overflow
				return False
			else:
				self.uniqueCardDict[cardUid]["hp"] = 0
				return True
		
	# effect dictionary should be something shown below
	'''
	"effects":{
		"FormationV": {"launchType": "assign", "countDown": 1, "once": True, "selfTarget": False, "prereqs":{}, "effectValues": {"value": 2, "distance": 0}}
	}
	'''
	def launchEffect(self, clientActionSequence, targetX, targetY, launchX, launchY, effectName, effectInfo):
		if effectName in effect_dict:
			resultDict = effect_dict[effectName](self.uniqueCardDict, self.gridInfoDict, self.inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo)
			# if launch effect succesfully, settlement has been done
			# broadcast latest operation result to all clients
			if resultDict["success"] == True:
				modifyGridIds = resultDict["modifyGrids"]
				# traverse all target cards to trigger their passive effects
				for targetGridXY in resultDict["modifyGrids"]:
					targetGridState = self.state_list[targetGridXY[0], targetGridXY[1]]
					targetGridStateStrs = targetGridState.split('/')
					for targetPassiveEffectKey,targetPassiveEffectValue in self.uniqueCardDict[targetGridStateStrs[0]]["effects"]:
						if targetPassiveEffectValue["launchType"] == "targetPassive":
							if targetPassiveEffectValue["prereqs"]["triggerEffectType"] == resultDict["triggerEffectType"]:
								if targetPassiveEffectKey in passive_effect_dict:
									passiveEffectResultDict = passive_effect_dict[targetPassiveEffectKey]
									(
										self.state_list,
										targetGridXY[0],
										targetGridXY[1],
										launchX,
										launchY,
										targetPassiveEffectValue
									)
									if passiveEffectResultDict["success"] == True:
										targetPassiveEffectValue["effectValues"]["isRoundEnd"] = True
										for passiveModifyGrid in passiveEffectResultDict["modifyGrids"]:
											if passiveModifyGrid not in modifyGridIds:
												modifyGridIds.append(passiveModifyGrid)
				# assemble all modification and notify all clients
				syncModifyGridInfos = []
				for modifyGrid in modifyGridIds:
					syncModifyGridInfos.append(self.gridInfoDict[modifyGrid])
				
				# only necessary when it's pvp battle
				for avatar in self.avatars:
					self.avatars[avatar].roomReqUpdateActionModification(syncModificationInfo)
			else:
				# if effect lauching fails, notify corresponding client
				self.avatars[launchAvatarId].roomReqNotifyLaunchSkillFailed(self.curActionSequence, clientActionSequence)


