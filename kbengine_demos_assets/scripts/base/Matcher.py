import KBEngine
from KBEDebug import *
import traceback

FIND_ROOM_NOT_FOUND = 0
FIND_ROOM_CREATING = 1

class Matcher(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.applyMatchDict = {}
		self.addTimer(1, 0.1, 1)
		self.maxMatchingPlayerNb = 20
		self.curMatchingPlayerNb = 0

	def applyMatch(self, accountEntityCall):
		DEBUG_MSG("Matcher::applyMatch: accountEntityCall entityID=%i" % (accountEntityCall.id))
		self.applyMatchDict[accountEntityCall.id] = accountEntityCall

	def registerGlobalMatcher(self, matcherNb):
		matcherNbStr = "Matcher_" + str(matcherNb)
		KBEngine.globalData[matcherNbStr] = self
		DEBUG_MSG("Matcher::registerGlobalMatcher: register as global var entityID=%i" % (self.id))
		
	def onTimer(self, id, userArg):
		for k1, v1 in self.applyMatchDict.items():
			for k2, v2 in self.applyMatchDict.items():
				if k1 == k2:
					continue
				else:
					if abs(v1.syntheticMatchScore - v2.syntheticMatchScore) <= 10:
						playerEntityIdList = [k1, k2]
						KBEngine.globalData["Hall"].createRoom(playerEntityIdList)
						self.applyMatchDict.pop(k1)
						self.applyMatchDict.pop(k2)
						return
					else:
						DEBUG_MSG("Matcher::onTimer: match nothing")
						