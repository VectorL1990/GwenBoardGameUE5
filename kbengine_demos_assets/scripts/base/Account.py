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
	"""
	账号实体
	客户端登陆到服务端后，服务端将自动创建这个实体，通过这个实体与客户端进行交互
	"""
	def __init__(self):
		INFO_MSG("create Account!!!")
		KBEngine.Proxy.__init__(self)
		self.activeAvatar = None
		self.relogin = time.time()

	def reqTest(self, param):
		INFO_MSG("receive msg from ue!!!")
		self.client.onReqTest(100)
		return
	
	def reqAvatarList(self):
		return
				
	def reqCreateAvatar(self, roleType, name):
		return
		
	def reqRemoveAvatar(self, name):
		return
		
	def reqRemoveAvatarDBID(self, dbid):
		return

	def selectAvatarGame(self, dbid):
		return
		
	#--------------------------------------------------------------------------------------------
	#                              Callbacks
	#--------------------------------------------------------------------------------------------
	def onClientEnabled(self):
		"""
		KBEngine method.
		该entity被正式激活为可使用， 此时entity已经建立了client对应实体， 可以在此创建它的
		cell部分。
		"""
		INFO_MSG("Account[%i]::onClientEnabled:entities enable. entityCall:%s, clientType(%i), clientDatas=(%s), hasAvatar=%s, accountName=%s" % \
			(self.id, self.client, self.getClientType(), self.getClientDatas(), self.activeAvatar, self.__ACCOUNT_NAME__))
			
	def onLogOnAttempt(self, ip, port, password):
		"""
		KBEngine method.
		客户端登陆失败时会回调到这里
		"""
		INFO_MSG("Account[%i]::onLogOnAttempt: ip=%s, port=%i, selfclient=%s" % (self.id, ip, port, self.client))
		"""
		if self.activeAvatar != None:
			return KBEngine.LOG_ON_REJECT

		if ip == self.lastClientIpAddr and password == self.password:
			return KBEngine.LOG_ON_ACCEPT
		else:
			return KBEngine.LOG_ON_REJECT
		"""
		
		# 如果一个在线的账号被一个客户端登陆并且onLogOnAttempt返回允许
		# 那么会踢掉之前的客户端连接
		# 那么此时self.activeAvatar可能不为None， 常规的流程是销毁这个角色等新客户端上来重新选择角色进入
		if self.activeAvatar:
			if self.activeAvatar.client is not None:
				self.activeAvatar.giveClientTo(self)

			self.relogin = time.time()
			self.activeAvatar.destroySelf()
			self.activeAvatar = None
			
		return KBEngine.LOG_ON_ACCEPT
		
	def onClientDeath(self):
		"""
		KBEngine method.
		客户端对应实体已经销毁
		"""
		if self.activeAvatar:
			self.activeAvatar.accountEntity = None
			self.activeAvatar = None

		DEBUG_MSG("Account[%i].onClientDeath:" % self.id)
		self.destroy()		
		
	def onDestroy(self):
		"""
		KBEngine method.
		entity销毁
		"""
		DEBUG_MSG("Account::onDestroy: %i." % self.id)
		
		if self.activeAvatar:
			self.activeAvatar.accountEntity = None

			try:
				self.activeAvatar.destroySelf()
			except:
				pass
				
			self.activeAvatar = None
			
	def __onAvatarCreated(self, baseRef, dbid, wasActive):
		"""
		选择角色进入游戏时被调用
		"""
		if wasActive:
			ERROR_MSG("Account::__onAvatarCreated:(%i): this character is in world now!" % (self.id))
			return
		if baseRef is None:
			ERROR_MSG("Account::__onAvatarCreated:(%i): the character you wanted to created is not exist!" % (self.id))
			return
			
		avatar = KBEngine.entities.get(baseRef.id)
		if avatar is None:
			ERROR_MSG("Account::__onAvatarCreated:(%i): when character was created, it died as well!" % (self.id))
			return
		
		if self.isDestroyed:
			ERROR_MSG("Account::__onAvatarCreated:(%i): i dead, will the destroy of Avatar!" % (self.id))
			avatar.destroy()
			return
			
		info = self.characters[dbid]
		avatar.cellData["modelID"] = d_avatar_inittab.datas[info[2]]["modelID"]
		avatar.cellData["modelScale"] = d_avatar_inittab.datas[info[2]]["modelScale"]
		avatar.cellData["moveSpeed"] = d_avatar_inittab.datas[info[2]]["moveSpeed"]
		avatar.accountEntity = self
		self.activeAvatar = avatar
		self.giveClientTo(avatar)
		
	def _onAvatarSaved(self, success, avatar):
		return
