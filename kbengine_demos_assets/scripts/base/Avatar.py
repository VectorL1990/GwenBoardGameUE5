# -*- coding: utf-8 -*-
import KBEngine
import random
import SCDefine
import time
import GlobalConst
import d_spaces
import d_avatar_inittab
from KBEDebug import *
from interfaces.GameObject import GameObject

class Avatar(KBEngine.Proxy,
			GameObject):
	def __init__(self):
		KBEngine.Proxy.__init__(self)
		GameObject.__init__(self)
		
		self.accountEntity = None
		self.cardList = []
		self.handCardList = []
		self.pileCardList = []
		self.graveCardList = []

		self.room = None

	def reqTellRoomSelectCardDone(self):
		# notify room that player has cards selected
		return

	def onClientEnabled(self):
		INFO_MSG("Avatar[%i-%s] entities enable. spaceUTypeB=%s, entityCall:%s" % (self.id, self.nameB, self.spaceUTypeB, self.client))
		# generate rand data for card pile initialization
		self.getShuffleCardList()
		self.client.onInitShuffleCardList()
		
	def getShuffleCardList(self):
		randCardList = self.cardList
		random.shuffle(randCardList)
		for i in range(10):
			self.handCardList.append(randCardList[i])
		del randCardList[0:10]
		self.pileCardList = randCardList

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
		
	def onClientDeath(self):
		DEBUG_MSG("Avatar[%i].onClientDeath:" % self.id)
		
	def onDestroy(self):
		DEBUG_MSG("Avatar::onDestroy: %i." % self.id)
		
		if self.accountEntity != None:
			self.accountEntity.activeAvatar = None
			self.accountEntity = None



