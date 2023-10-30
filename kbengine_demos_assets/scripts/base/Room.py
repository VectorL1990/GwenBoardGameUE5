# -*- coding: utf-8 -*-
import KBEngine
import math
import time
from KBEDebug import *

class Room(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.avatars = {}
		self.gridInfoDict = {}
		self.runLoop = False
		self.curControlNb = 0
		self.roomPlayerNb = 2
		self.curTimeClockInterval = 0
		self.maxTimeClockInterval = 10000
		self.lastTimeClock = -1
		self.curTimeClock = 0.0
		self.startTimeCount = False
		self.receiveConfirmNb = 0

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

	def enterRoom(self, entityCall, position, direction):
		"""
		defined method.
		请求进入某个space中
		"""
		entityCall.createCell(self.cell, self.roomKey)
		self.onEnter(entityCall)

	def leaveRoom(self, entityID):
		"""
		defined method.
		某个玩家请求退出这个space
		"""
		self.onLeave(entityID)

	def switchControllerConfirm(self, entityCall, turnNb, roundNb):
		self.receiveConfirmNb += 1
		
	def onTimer(self, tid, userArg):
		if self.keepCounting == False:
			return
		
		#DEBUG_MSG("%s::onTimer: %i, tid:%i, arg:%i" % (self.getScriptName(), self.id, tid, userArg))
		if self.curTimeClockInterval >= self.maxTimeClockInterval and self.startTimeCount == True:
			# which means we should modify controller to another player
			self.curControlNb += 1
			if self.curControlNb >= self.roomPlayerNb:
				self.curControlNb = 0
			self.curTimeClockInterval = 0.0
			# send messages to all proxys to switch controller
			for avatar in self.avatars:
				self.avatars[avatar].onSwitchController(self.curControlNb)

			# stop counting right now, until every player send msg about recovering
			self.keepCounting = False
		else:
			self.curTimeClockInterval += 1
			if self.startTimeCount == False:
				self.startTimeCount = True
			else:
				# send messages to all clients to sync counting time
				for avatar in self.avatars:
					self.avatars[avatar].onSyncTimeInterval(self.curTimeClockInterval)
		
	def onEnter(self, entityCall):
		"""
		defined method.
		进入场景
		"""
		self.avatars[entityCall.id] = entityCall
		
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
		KBEngine.globalData["Halls"].onRoomLoseCell(self.roomKey)
		
		self.avatars = {}
		self.destroy()

	def onGetCell(self):
		"""
		KBEngine method.
		entity的cell部分实体被创建成功
		"""
		DEBUG_MSG("Room::onGetCell: %i" % self.id)
		KBEngine.globalData["Halls"].onRoomGetCell(self, self.roomKey)

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

