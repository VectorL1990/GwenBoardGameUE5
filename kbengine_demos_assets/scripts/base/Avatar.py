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
		self.changeSelectCardNb = 0

	def reqTellRoomSelectCardDone(self):
		# notify room that player has cards selected
		return
		
	def shuffleCardList(self, allCardList):
		randCardList = allCardList
		random.shuffle(randCardList)
		cardListLen = len(randCardList)
		for i in range(cardListLen):
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
	
	# ---
	#
	# ---
	def roomReqStartBattle(self):
		# card selection finished, start battle now
		self.client.onSyncRoomStartBattle()
		return

	#---
	#
	#---
	def reqChangeSelectCard(self, cardKey):
		leftChangeCardNb = GlobalConst.g_maxChangeCardNb - self.changeSelectCardNb
		if leftChangeCardNb <= 1:
			# which means player can not change any card no more
			self.client.onSyncExhaustCardReplacement()
		else:
			handCardListLen = len(self.handCardList)
			if cardKey in self.handCardList:
				# switch hand cards
				for i in range(handCardListLen):
					if self.handCardList[i] == cardKey:
						# which means this is the card to be switched
						# put the first pile card into this position
						switchCard = self.handCardList[i]
						self.handCardList[i] = self.pileCardList[0]
						switchCardKey = self.pileCardList[0]
						self.pileCardList.pop(0)
						self.pileCardList.append(switchCard)
						self.changeSelectCardNb += 1
						self.client.onSyncChangeHandCardSuccess(self.changeSelectCardNb, cardKey, switchCardKey)
						break

	def reqUpdateSelectedCard(self):
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
		self.client.onSyncUpdateSelectedCards(self.changeSelectCardNb, syncPlayerBattleInfo)
		return
	
	def reqFinishSelectCards(self):
		
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

	def onSyncExhaustCardReplacement(self):
		return
	
	def onSyncRoomStartBattle(self):
		return



