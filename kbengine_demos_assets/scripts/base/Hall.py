import KBEngine
from KBEDebug import *
import traceback

FIND_ROOM_NOT_FOUND = 0
FIND_ROOM_CREATING = 1

class Hall(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		KBEngine.globalData["Hall"] = self
		self.rooms = {}
		self.applyMatchDict = {}
		self.matcherNb = 0
		self.createMatcher()
		

	def createMatcher(self):
		matcher = KBEngine.createEntityAnywhere("Matcher", {}, self.onMatcherCreated)
		self.matcherNb += 1
		matcher.registerGlobalMatcher(self.matcherNb)
		return

	def applyMatch(self, accountEntityCall):
		self.applyMatchDict[accountEntityCall.id] = accountEntityCall


	def createRoom(self, playerEntityList):
		return
		
	def findRoom(self, roomKey, notFoundCreate = False):
		roomDatas = self.rooms.get(roomKey)
		
		if not roomDatas:
			if not notFoundCreate:
				return FIND_ROOM_NOT_FOUND
			
			roomDatas = self.rooms.get(self.roomKey)
			if roomDatas is not None:
				return roomDatas

			self.lastNewRoomKey = KBEngine.genUUID64()
			
			KBEngine.createEntityAnywhere("Room", \
									{
									"roomKey" : self.lastNewRoomKey,	\
									}, \
									Functor.Functor(self.onRoomCreatedCB, self.lastNewRoomKey))
			
			roomDatas = {"roomEntityCall" : None, "PlayerCount": 0, "enterRoomReqs" : [], "roomKey" : self.lastNewRoomKey}
			self.rooms[self.lastNewRoomKey] = roomDatas
			return roomDatas

		return roomDatas
	
	def onMatcherCreated(self):
		return


