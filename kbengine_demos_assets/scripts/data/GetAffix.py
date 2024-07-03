import random
import GlobalConst

def ColDiff(stateList, x, y, targetX, targetY):
	return abs(x - targetX)

def SameRowHurtCardNb(stateList, x, y, targetX, targetY):
	hurtCardNb = 0
	for i in range(0, GlobalConst.maxCol, 1):
		stateStr = stateList[y][i]
		stateStrs = stateStr.split('/')
		launchOriginHp = stateStrs[18]
		launchHp = stateStrs[17]
		if launchHp < launchOriginHp:
			hurtCardNb += 1
	return hurtCardNb

getAffixDict = {
	"ColDiff": ColDiff,
	'SamRowHurtCardNb': SameRowHurtCardNb
}
