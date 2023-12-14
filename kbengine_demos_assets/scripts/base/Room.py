# -*- coding: utf-8 -*-
import KBEngine
import random
import math
import time
from KBEDebug import *
import GlobalConst
from d_all_cards import allDatas

class Room(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
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
			DEBUG_MSG("Room::hallReqAccountsRoomCreated: tell account room created and entityID=%i" % (k))
			v.syncRoomCreated(self.roomKey)
		
		# at this point we could countdown for avatar entering room
		self.battleState = GlobalConst.g_battleState.WAIT_AVATAR_ENTER_ROOM
		self.addTimer(0.0, 1.0, 1.0)

	def playAction(self, entityCall, actionInfo):
		if actionInfo["actionType"] == "playCard":
			if self.gridInfoDict.has_key(actionInfo["targetGridNb"]):
        # this action is not allowed, grid already occupied
				return
			else:
        # add this card to dictionary
        # calculate card effect
				playCardAction()
				self.avatars[entityCall.id].playCardCallback()
		elif actionInfo["actionType"] == "assignEffectTarget":
			# calcualte effect in this place
			return

	def avatarEnterRoom(self, avatarEntityCall):
		if avatarEntityCall.id not in self.avatars:
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
					"stateTags": []
				}
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
				self.curTimeClockInterval = 0.0

	def avatarReqHeartBeat(self, avatarEntityCall):
		# avatar should send heart beat to room to keep alive
		if avatarEntityCall.id in self.avatarsHeartBeatCount:
			self.avatarsHeartBeatCount[avatarEntityCall.id] = 0

	def avatarReqLatestBattleInfo(self, avatarEntityCall):
		if avatarEntityCall.id in self.avatars:
			allGridInfo = []
			for k,v in self.gridInfoDict.items():
				battleGridInfo = {
					"gridNb": k,
					"cardUid": v["cardKey"],
					"hp": v["curHp"],
					"defence": v["defence"],
					"agility": v["agility"],
					"tags": v["tags"],
					"stateTags": v["stateTags"]
				}
				allGridInfo.append(battleGridInfo)

			coreUpdateBattleInfo = {
				"curActionSequence": self.curActionSequence,
				"updateList": allGridInfo,
				"playerInfo": {},
				"curSwitchControllerSequence": self.curSwitchNb,
				"curControllerNb": self.curControlNb,
				"curControllerAvatarId": self.inBattleAvatarList[self.curControlNb].id
			}
			avatarEntityCall.roomReqUpdateLatestBattleInfo(coreUpdateBattleInfo)

	def avatarReqPlayCardAction(self, avatarEntityCall, avatarClientActionSequence, cardUid, gridNb):
		actionSequenceLatency = self.curActionSequence - avatarClientActionSequence
		if actionSequenceLatency == 1:
			# which means there's no information lost on client side
			# find corresponding card info from card dictionary, including effects attached to this card


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
					self.curTimeClockInterval = 0.0
				else:
					self.curTimeClockInterval += 1
		elif self.battleState == GlobalConst.g_battleState.SELECT_CARD:
			if self.curTimeClockInterval >= self.maxSelectCardTimeInterval:
				# force all players stopping card selection
				for avatar in self.avatars:
					self.avatars[avatar].roomReqSelectCardInterlude()
				self.battleState = GlobalConst.g_battleState.SELECT_CARD_INTERLUDE
				random.shuffle(self.inBattleAvatarList)
				self.curTimeClockInterval = 0.0
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
				self.curTimeClockInterval = 0.0
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
				self.curTimeClockInterval = 0.0
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


	def playCardAction(self, targetGrid, playCardInfoDict):
		if self.gridInfoDict.has_key(targetGrid):
			# which means this grid is occupied
			return 0
		else:
			if playCardInfoDict.has_key("autoEffects"):
				for effectKey in playCardInfoDict["autoEffects"]:
					# launch every auto effect
					launchEffect(-1, targetGrid, playCardInfoDict[effectKey])
				return 1
			elif playCardInfoDict.has_key("assignEffects"):
				# which means this effect requires play to assign target
				return 2

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


	def getGridRowAndCol(self, gridNb):
		col = gridNb % GlobalConst.g_boardColumn
		row = gridNb / GlobalConst.g_boardColumn
		rc = [row, col]
		return rc


	# effect dictionary should be something shown below
	'''
	"effects":{
		"Burst": {"launchType": "auto", "countDown": 3, "once": True, "selfTarget": False, "prereqs":{"beingHurt": [1]}, "effectValues": {"values":[1], "type":"cross"}},
		"FenceDevour": {"launchType": "assign", "countDown": 1, "once": True, "selfTarget": False, "prereqs":{}, "effectValues": {"values":[0]}}
	}
	'''
	def launchEffect(self, launchAvatarId, targetGrid, launchGrid, effectName, effectInfo):
		if effectName == "FormationVShoot":
			# if card occupied target grid is not enermy, action fails
			cardInfo = self.uniqueCardDict[self.gridInfoDict[targetGrid]["cardUid"]]
			# effect is only allowed on oppo side cards
			if cardInfo["avatarId"] != launchAvatarId:
				launchGridRC = getGridRowAndCol(launchGrid)
				targetGridRC = getGridRowAndCol(targetGrid)
				rowOffset = targetGridRC[0] - launchGridRC[0]
				colOffset = targetGridRC[1] - launchGridRC[1]
				direction = [rowOffset, colOffset]
				if direction[0] != 0 and direction[1] != 0:
					# which means target grid is not the lined up to launch grid, which is not permitted
					distance = abs(rowOffset) + abs(colOffset)
					if effectInfo["effectValues"]["distance"] <= distance:
						# target grid is within attack distance, which allows skill launch
						# now we should tell whether formation


		if effectInfo["auto"] == True:
			if effectInfo["selfTarget"] == False:
				for gridNb in self.gridInfoDict:
					if launchGrid == gridNb:
						continue

					allPrereqsSatisfied = True
					if effectInfo.has_key("prereqs"):
						# which means this effect requires prereqs
						for prereqKey in effectInfo["prereqs"]:
							satisfied= checkPrerequisites(gridNb, launchGrid, effectInfo)
							if satisfied == False:
								allPrereqsSatisfied = False
								break
					if allPrereqsSatisfied == True:
						calculateEffect(gridNb, launchGrid, effectInfo)
			else:
				sdf
		else:
			allPrereqsSatisfied = True
			if effectInfo.has_key("prereqs"):
				for prereqKey in effectInfo["prereqs"]:
					satisfied = checkPrerequisites(targetGrid, launchGrid, effectInfo)
					if satisfied == False:
						allPrereqsSatisfied = False
						break
			if allPrereqsSatisfied == True:
				calculateEffect(targetGrid, launchGrid, effectInfo)

	def checkPrerequisites(self, targetGrid, launchGrid, effectInfo):
		if effectInfo["effectName"] == "injury":
			if self.gridInfoDict[targetGrid]["curHp"] < self.gridInfoDict[targetGrid]["originHp"]:
				return true
			else:
				return false
		elif effectInfo["effectName"] == "beingHurt":
			if effectInfo["hurtValues"][0] > self.gridInfoDict[targetGrid]["prereqs"]["beingHurt"]["triggerValue"]:
				return true
			else:
				return false
	
	def calculateEffect(self, targetGrid, launchGrid, effectInfo):
		if effectInfo["effectName"] == "fenceDevour":
			self.gridInfoDict[targetGrid]["dead"] = True
			self.gridInfoDict[targetGrid] = self.gridInfoDict[launchGrid]
			del self.gridInfoDict[launchGrid]

