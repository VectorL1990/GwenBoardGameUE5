# -*- coding: utf-8 -*-
import KBEngine
import random
import time
import d_spaces
import GlobalConst
#from AVATAR_INFOS import TAvatarInfos
#from AVATAR_INFOS import TAvatarInfosList
#from AVATAR_DATA import TAvatarData
from KBEDebug import *
import d_avatar_inittab

class Account(KBEngine.Proxy):
	def __init__(self):
		INFO_MSG("create Account!!!")
		KBEngine.Proxy.__init__(self)
		self.activeAvatar = None
		self.relogin = time.time()
		self.syntheticMatchScore = 0
		self.roomKey = 0

	def reqTest(self, param):
		INFO_MSG("receive msg from ue!!!")
		self.client.onReqTest(100)
		return

	def reqDispatchRoom(self):
		return

	def reqEnterRoom(self):
		# spawn entity for
		KBEngine.createEntityAnywhere("Avatar", {}, self.onAvatarCreated)

	def reqMatch(self):
		DEBUG_MSG("Account::reqMatch: entityID=%i" % (self.id))
		KBEngine.globalData["Hall"].applyMatch(self)

	def syncRoomCreated(self, roomKey):
		self.roomKey = roomKey
		self.onSyncRoomCreated(roomKey)
	
		
	#--------------------------------------------------------------------------------------------
	#                              Callbacks
	#--------------------------------------------------------------------------------------------
	def onClientEnabled(self):
		INFO_MSG("Account[%i]::onClientEnabled:entities enable. entityCall:%s, clientType(%i), clientDatas=(%s), hasAvatar=%s, accountName=%s" % \
			(self.id, self.client, self.getClientType(), self.getClientDatas(), self.activeAvatar, self.__ACCOUNT_NAME__))
			
	def onLogOnAttempt(self, ip, port, password):
		INFO_MSG("Account[%i]::onLogOnAttempt: ip=%s, port=%i, selfclient=%s" % (self.id, ip, port, self.client))
		
		# if it log on successfully in case account is already on line on another platform
		# previous log on account will be kicked out
		# in this case we should kill avatar for relogin
		if self.activeAvatar:
			if self.activeAvatar.client is not None:
				self.activeAvatar.giveClientTo(self)

			self.relogin = time.time()
			self.activeAvatar.destroySelf()
			self.activeAvatar = None
			
		return KBEngine.LOG_ON_ACCEPT
		
	def onClientDeath(self):
		if self.activeAvatar:
			self.activeAvatar.accountEntity = None
			self.activeAvatar = None

		DEBUG_MSG("Account[%i].onClientDeath:" % self.id)
		self.destroy()		
		
	def onDestroy(self):
		DEBUG_MSG("Account::onDestroy: %i." % self.id)
		
		if self.activeAvatar:
			self.activeAvatar.accountEntity = None

			try:
				self.activeAvatar.destroySelf()
			except:
				pass
				
			self.activeAvatar = None

	def onSyncRoomCreated(self, roomKey):
		self.client.onSyncRoomCreated(roomKey)
			
	def onAvatarCreated(self, baseRef):
		if baseRef is None:
			ERROR_MSG("Account::onAvatarCreated:(%i): create avatar failed!" % (self.id))
			return
			
		avatar = KBEngine.entities.get(baseRef.id)
		if avatar is None:
			ERROR_MSG("Account::onAvatarCreated:(%i): avatar created is none!" % (self.id))
			return
		
		if self.isDestroyed:
			ERROR_MSG("Account::onAvatarCreated:(%i): account is destroyed, kill avatar as well!" % (self.id))
			avatar.destroy()
			return
		
		avatar.accountEntity = self
		avatar.roomEntityCall = KBEngine.globalData["Halls"].rooms[self.roomKey]
		#avatar.persistPlayerInfo = self.persistPlayerInfo
		avatar.persistPlayerInfo = {
			"persistCardList" : ["Adam", "Bale", "Charles"]
		}
		self.activeAvatar = avatar
		self.giveClientTo(avatar)
		
