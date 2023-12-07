# -*- coding: utf-8 -*-
import KBEngine
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
					"CardName": strs[1],
					"Hp": cardInfo["Hp"],
					"Defence": cardInfo["Defence"],
					"Agility": cardInfo["Agility"],
					"Tags": cardInfo["Tags"]
				}
				self.uniqueCardDict[cardKey] = avatarEntityCall.allCardDict[cardKey]


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
					"agility": v["agility"]
				}
				allGridInfo.append(battleGridInfo)

			coreUpdateBattleInfo = {
				"curTick": self.curBattleTick,
				"updateList": allGridInfo,
				"playerInfo": {}
			}
			avatarEntityCall.roomReqUpdateLatestBattleInfo(coreUpdateBattleInfo)


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


	def launchEffect(self, targetGrid, launchGrid, effectInfo):
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
					astisfied = checkPrerequisites(targetGrid, launchGrid, effectInfo)
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

