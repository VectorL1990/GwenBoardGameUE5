# -*- coding: utf-8 -*-
import KBEngine
import random
import math
import time
from KBEDebug import *
import GlobalConst
from GetPossibleActions import actionIdConverterDict
from AllCards import allCards
from d_effects import effect_dict
from d_passive_effects import passive_effect_dict

class PvPRoom(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.board = Board()
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
				cardInfo = allCards["allCards"][strs[2]]
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


	def avatarReqLaunchCardSkillAction(self, avatarEntityCall, clientActionSequence, cardUid, skillName, launchGridNb, targetGridNb):
		actionSequenceLatency = self.curActionSequence - clientActionSequence
		if actionSequenceLatency == 1:
			actionId = actionIdConverterDict["GetLaunchSkillActionId"]
			self.board.DoMove(actionId)
	
	def avatarReqEndRound(self, avatarEntityCall, clientActionSequence):
		self.roomReqEndRound()

	def roomReqEndRound(self):
		self.curSwitchNb += 1
		self.curControlNb += 1
		if self.curControlNb >= len(self.inBattleAvatarList):
			self.curControlNb = 0
		# send messages to all proxys to switch controller
		for avatar in self.avatars:
			self.avatars[avatar].roomReqSwitchController(self.curSwitchNb, self.inBattleAvatarList[self.curControlNb].id)
		self.curTimeClockInterval = 0
		self.battleState = GlobalConst.g_battleState.BATTLE_INTERLUDE

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
				self.roomReqEndRound()
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



