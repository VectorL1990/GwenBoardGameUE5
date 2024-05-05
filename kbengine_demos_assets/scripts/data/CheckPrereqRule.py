import random
import GlobalConst

def SelfBeHeal(stateList, x, y, targetX, targetY, selfOppoCampFlag):
    stateStrs = stateList[y][x].split('/')
    
	
def sameRowSelfHpMoreOppo(stateList, x, y, targetX, targetY, launchCampNb, selfOppoCampFlag):
	selfCampHp = 0
	oppoCampHp = 0
	for col in range(GlobalConst.maxCol):
		if stateList[y][col] != "--":
			gridStateStrs = stateList[y][col].split('/')
			if gridStateStrs[2] == launchCampNb:
				selfCampHp += stateList[17]
			else:
				oppoCampHp += stateList[17]
	if selfCampHp > oppoCampHp:
		return True
	else:
		return False
	

def sameRowSelfHpLessOppo(stateList, x, y, targetX, targetY, launchCampNb, selfOppoCampFlag):
	selfCampHp = 0
	oppoCampHp = 0
	for col in range(GlobalConst.maxCol):
		if stateList[y][col] != "--":
			gridStateStrs = stateList[y][col].split('/')
			if gridStateStrs[2] == launchCampNb:
				selfCampHp += stateList[17]
			else:
				oppoCampHp += stateList[17]
	if selfCampHp < oppoCampHp:
		return True
	else:
		return False
	

def sameColSelfHpMoreOppo(stateList, x, y, targetX, targetY, launchCampNb, selfOppoCampFlag):
	selfCampHp = 0
	oppoCampHp = 0
	for row in range(GlobalConst.maxRow):
		if stateList[x][row] != "--":
			gridStateStrs = stateList[y][row].split('/')
			if gridStateStrs[2] == launchCampNb:
				selfCampHp += stateList[17]
			else:
				oppoCampHp += stateList[17]
	if selfCampHp > oppoCampHp:
		return True
	else:
		return False
	

def sameColSelfHpLessOppo(stateList, x, y, targetX, targetY, launchCampNb, selfOppoCampFlag):
	selfCampHp = 0
	oppoCampHp = 0
	for row in range(GlobalConst.maxRow):
		if stateList[x][row] != "--":
			gridStateStrs = stateList[y][row].split('/')
			if gridStateStrs[2] == launchCampNb:
				selfCampHp += stateList[17]
			else:
				oppoCampHp += stateList[17]
	if selfCampHp < oppoCampHp:
		return True
	else:
		return False

checkPrereqRuleDict = {
	'Line': Line,
	'Seperate': Seperate,
	'Diagonal': Diagonal,
	"DiagonalSeperate": DiagonalSeperate
}
