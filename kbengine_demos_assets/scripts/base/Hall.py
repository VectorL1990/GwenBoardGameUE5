import KBEngine
import Functor
from KBEDebug import *
import traceback

FIND_ROOM_NOT_FOUND = 0
FIND_ROOM_CREATING = 1

class Hall(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		KBEngine.globalData["Hall"] = self
		self.rooms = {}
		self.matcherDict = {}
		self.applyMatchPlayerDict = {}
		self.matcherNb = 0
		self.createMatcher()
		

	def createMatcher(self):
		matcher = KBEngine.createEntityAnywhere("Matcher", {}, self.onMatcherCreated)
		self.matcherNb += 1
		# add this matcher to dictionary
		self.matcherDict[self.matcherNb] = matcher
		matcher.registerGlobalMatcher(self.matcherNb)
		return

	def applyMatch(self, accountEntityCall):
		for k,v in self.matcherDict.items():
			if v.curMatchingPlayerNb < v.maxMatchingPlayerNb :
				v.applyMatch(accountEntityCall)
			else:
				continue
		self.applyMatchPlayerDict[accountEntityCall.id] = accountEntityCall


	def createRoom(self, playerEntityIdList):
		newRoomKey = KBEngine.genUUID64()
		KBEngine.createEntityAnywhere("Room", \
			{
				"roomKey":newRoomKey, \
			}, \
			Functor.Functor(self.onRoomCreated, newRoomKey, playerEntityIdList))
	
	def onMatcherCreated(self):
		return

	def onRoomCreated(self, roomKey, playerEntityIdList, roomEntityCall):
		DEBUG_MSG("Halls::onRoomCreatedCB: space %i. entityID=%i" % (roomKey, roomEntityCall.id))
		for playerEntityId in playerEntityIdList:
			roomEntityCall.accountEntityDict[playerEntityId] = self.applyMatchPlayerDict[playerEntityId]
		# delete playerId in dictionary
		for playerId in playerEntityList:
			self.applyMatchPlayerDict.pop(playerId)
		roomEntityCall.tellAccountsRoomCreated()



