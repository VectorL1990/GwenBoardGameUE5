import KBEngine
from KBEDebug import *
import traceback

FIND_ROOM_NOT_FOUND = 0
FIND_ROOM_CREATING = 1

class Matcher(KBEngine.Entity):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		self.applyMatchDict = {}
		self.addTimer(1, 0.1)

	def applyMatch(self, accountEntityCall):
		self.applyMatchDict[accountEntityCall.id] = accountEntityCall

	def registerGlobalMatcher(self, matcherNb):
		matcherNbStr = "Matcher_" + str(matcherNb)
		KBEngine.globalData[matcherNbStr] = self
		
	def onTimer(self, id, userArg):
		for k1, v1 in self.applyMatchDict.values():
			for k2, v2 in self.applyMatchDict.values():
				if k1 == k2:
					continue
				else:
					if abs(v1.syntheticMatchScore - v2.syntheticMatchScore) <= 10:
						playerEntityList = [v1, v2]
						KBEngine.globalData["Hall"].createRoom(playerEntityList)
						