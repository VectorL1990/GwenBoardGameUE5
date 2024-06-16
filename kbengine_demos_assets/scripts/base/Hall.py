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
		KBEngine.createEntityAnywhere("Matcher", {}, self.onMatcherCreated)

	def applyMatch(self, accountEntityCall):
		DEBUG_MSG("Halls::applyMatch: accountEntityCall entityID=%i" % (accountEntityCall.id))
		if accountEntityCall.id not in self.applyMatchPlayerDict:
			for k,v in self.matcherDict.items():
				if v.curMatchingPlayerNb < v.maxMatchingPlayerNb :
					v.applyMatch(accountEntityCall)
				else:
					continue
			self.applyMatchPlayerDict[accountEntityCall.id] = accountEntityCall


	def createRoom(self, playerEntityIdList):
		newRoomKey = KBEngine.genUUID64()
		KBEngine.createEntityAnywhere("PvPRoom", \
			{
				"roomKey":newRoomKey, \
			}, \
			Functor.Functor(self.onRoomCreated, newRoomKey, playerEntityIdList))
	
	def onMatcherCreated(self, matcherEntityCall):
		self.matcherNb += 1
		# add this matcher to dictionary
		self.matcherDict[self.matcherNb] = matcherEntityCall
		matcherEntityCall.registerGlobalMatcher(self.matcherNb)
		return

	def onRoomCreated(self, roomKey, playerEntityIdList, roomEntityCall):
		DEBUG_MSG("Halls::onRoomCreatedCB: space %i. entityID=%i" % (roomKey, roomEntityCall.id))
		for playerEntityId in playerEntityIdList:
			roomEntityCall.accountEntityDict[playerEntityId] = self.applyMatchPlayerDict[playerEntityId]
		# delete playerId in dictionary
		for playerId in playerEntityIdList:
			DEBUG_MSG("Halls::onRoomCreatedCB: delete player in apply match dict and entityID=%i" % (playerId))
			self.applyMatchPlayerDict.pop(playerId)
		self.rooms[roomKey] = roomEntityCall
		roomEntityCall.hallReqAccountsRoomCreated()

	def roomReqDelete(self, roomKey):
		self.rooms.pop(roomKey)



