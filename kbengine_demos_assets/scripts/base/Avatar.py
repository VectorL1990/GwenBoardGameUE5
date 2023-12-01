# -*- coding: utf-8 -*-
import KBEngine
import random
import SCDefine
import time
import GlobalConst
from KBEDebug import *

from interfaces.GameObject import GameObject

class Avatar(KBEngine.Proxy,
			GameObject):
	def __init__(self):
		KBEngine.Proxy.__init__(self)
		GameObject.__init__(self)
		
		self.accountEntity = None
		self.persistPlayerInfo = None
		self.allCardDict = {}
		self.handCardList = []
		self.pileCardList = []
		self.graveCardList = []
		self.roomEntityCall = None

	def reqTellRoomSelectCardDone(self):
		# notify room that player has cards selected
		return
		
	def shuffleCardList(self, allCardList):
		randCardList = allCardList
		random.shuffle(randCardList)
		cardListLen = len(randCardList)
		for i in range(1, cardListLen):
			cardKey = str(self.id) + '_' + str(i) + '_' + randCardList[i]
			self.allCardDict[cardKey] = {}
			# append g_handCardAmount cards into hand holding pile
			if i < GlobalConst.g_handCardAmount:
				self.handCardList.append(cardKey)
			else:
				self.pileCardList.append(cardKey)

	def destroySelf(self):
		"""
		"""
		if self.client is not None:
			return
		
		# if account entity exists, notify destruction
		if self.accountEntity != None:
			if time.time() - self.accountEntity.relogin > 1:
				self.accountEntity.destroy()
			else:
				DEBUG_MSG("Avatar[%i].destroySelf: relogin =%i" % (self.id, time.time() - self.accountEntity.relogin))
		
		if not self.isDestroyed:
			self.destroy()

	def stopCardSelection(self):
		return

	def syncTimeInterval(self, curTime, state):
		return

	def startBattle(self):
		return

	def switchController(self, controlNb):
		return

	def resumeBattle(self):
		return

	#--------------------------------------------------------------------------------------------
	#                              Callbacks
	#--------------------------------------------------------------------------------------------
	def onClientEnabled(self):
		cardList = self.persistPlayerInfo["persistCardList"]
		playerBattleInfo = {
			"cardList" : cardList,
		}
		# shuffle and encode all cards
		self.shuffleCardList(playerBattleInfo["cardList"])

		# tell room avatar is ready
		self.roomEntityCall.avatarEnterRoom(self)

		allCardList = []
		for k, v in self.allCardDict.items():
			cardInfo = {
				"cardKey": k,
				"cardName": v["CardName"],
				"hp": v["Hp"],
				"defence": v["Defence"],
				"agility": v["Agility"],
				"tags": v["Tags"],
			}
			allCardList.append(cardInfo)

		syncPlayerBattleInfo = {
			"cardList" : allCardList,
			"handCardList" : self.handCardList
		}
		self.client.onSyncPlayerBattleInfo(syncPlayerBattleInfo)
		
	def onClientDeath(self):
		DEBUG_MSG("Avatar[%i].onClientDeath:" % self.id)
		
	def onDestroy(self):
		DEBUG_MSG("Avatar::onDestroy: %i." % self.id)
		
		if self.accountEntity != None:
			self.accountEntity.activeAvatar = None
			self.accountEntity = None



