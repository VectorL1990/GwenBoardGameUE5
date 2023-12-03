# -*- coding: utf-8 -*-
import KBEngine
import math
import time
from KBEDebug import *
from d_all_cards import allDatas

class Room(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.accountEntityDict = {}
		self.accountEntityIdList = []
		self.gridInfoDict = {}
		self.runLoop = False
		self.curControlNb = 0
		self.roomPlayerNb = 2
		self.curTimeClockInterval = 0
		self.maxSelectCardTimeInterval = 20000
		self.maxSelectCardDoneAnimTime = 4000
		self.maxLaunchActionTimeInterval = 10000
		self.maxSwitchControllerAnimTime = 4000
		self.lastTimeClock = -1
		self.curTimeClock = 0.0
		self.startTimeCount = False
		self.receiveConfirmNb = 0

		self.timerState = 0
		self.avatars = {}
		self.enterRoomAccountList = []
		self.finishSelectCardAvatarList = []

		self.uniqueCardDict = {}

	def tellAccountsRoomCreated(self):
		for k,v in self.accountEntityDict.items():
			DEBUG_MSG("Room::tellAccountsRoomCreated: tell account room created and entityID=%i" % (k))
			self.accountEntityIdList.append(k)
			v.syncRoomCreated(self.roomKey)

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
		if avatarEntityCall not in self.enterRoomAccountList:
			self.enterRoomAccountList.append(avatarEntityCall.accountEntity)
			self.avatars[avatarEntityCall.id] = avatarEntityCall
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
		if avatarEntityCall not in self.finishSelectCardAvatarList:
			self.finishSelectCardAvatarList.append(avatarEntityCall)
			finishSelectCardAvatarNb = len(self.finishSelectCardAvatarList)
			if (len(self.enterRoomAccountList) == finishSelectCardAvatarNb):
				# which means all avatars finish card selection
				return
		

	def leaveRoom(self, entityID):
		self.onLeave(entityID)

	def switchControllerConfirm(self, entityCall, turnNb, roundNb):
		self.receiveConfirmNb += 1
		
	def onTimer(self, tid, userArg):
		if self.keepCounting == False:
			return
		
		# room should tell which stage it's in right now
		# 1. which including wait for players cards selection
		#  if players do not finish cards selection in limited time
		#  force them start right now
		# 2. wait for a moment to receive informations about all clients have played prepared animations
		#  if some confirm information is missing, force start counting
		# 3. counting for real battle
		#  3.1 when it comes to switch controller between players
		#   it should stop for a moment to wait for animations playing for all clients
		#   if animation confirm information is missing of some clients
		#   force keep counting
		if self.timerState == 0:
			return
		elif self.timerState == 1:
			# which means room is waiting for card selection
			if self.curTimeClockInterval >= self.maxSelectCardTimeInterval:
				# force all players stopping card selection
				for avatar in self.avatars:
					self.avatars[avatar].stopCardSelection()
				self.curTimeClockInterval = 0.0
				# make timer state to 2 for players to play animations
				self.timerState = 2
			else:
				self.curTimeClockInterval += 1
				for avatar in self.avatars:
					self.avatars[avatar].syncTimeInterval(self.curTimeClockInterval, self.timerState)
		elif self.timerState == 2:
			if self.curTimeClockInterval >= self.maxSelectCardDoneAnimTime:
				for avatar in self.avatars:
					self.avatars[avatar].startBattle()
				self.curTimeClockInterval = 0.0
				self.timerState = 3
			else:
				self.curTimeClockInterval += 1
		elif self.timerState == 3:
			if self.curTimeClockInterval >= self.maxLaunchActionTimeInterval:
				# which means we should modify controller to another player
				self.curControlNb += 1
				if self.curControlNb >= self.roomPlayerNb:
					self.curControlNb = 0
				# send messages to all proxys to switch controller
				for avatar in self.avatars:
					self.avatars[avatar].switchController(self.curControlNb)
				self.curTimeClockInterval = 0.0
				self.timerState = 4
			else:
				self.curTimeClockInterval += 1
				for avatar in self.avatars:
					self.avatars[avatar].syncTimeInterval(self.curTimeClockInterval, self.timerState)
		elif self.timerState == 4:
			if self.curTimeClockInterval >= self.maxSwitchControllerAnimTime:
				for avatar in self.avatars:
					self.avatars[avatar].resumeBattle()
				self.curTimeClockInterval = 0.0
				self.timerState = 3
			else:
				self.curTimeClockInterval += 1

			

		
		
	def onLeave(self, entityID):
		"""
		defined method.
		离开场景
		"""
		if entityID in self.avatars:
			del self.avatars[entityID]

	def onLoseCell(self):
		"""
		KBEngine method.
		entity的cell部分实体丢失
		"""
		KBEngine.globalData["Hall"].onRoomLoseCell(self.roomKey)
		
		self.avatars = {}
		self.destroy()

	def onGetCell(self):
		"""
		KBEngine method.
		entity的cell部分实体被创建成功
		"""
		DEBUG_MSG("Room::onGetCell: %i" % self.id)
		KBEngine.globalData["Hall"].onRoomGetCell(self, self.roomKey)

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

