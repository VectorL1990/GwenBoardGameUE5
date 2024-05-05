import random
import GlobalConst

def CheckTagOnBoard(stateList, x, y, targetX, targetY, targetTag, prereqTagNb, selfOppoCamp):
	targetTagNb = 0
	launchStateStrs = stateList[y][x].split('/')
	for i in range(len(stateList)):
		if stateList[i] == "--":
			continue

		stateStrs = stateList[i].split('/')
		tags = stateStrs[24].split('&')
		if targetTag in tags:
			if selfOppoCamp == "self":
				if stateStrs[2] == launchStateStrs[2]:
					targetTagNb += 1
			elif selfOppoCamp == "oppo":
				if stateStrs[2] != launchStateStrs[2]:
					targetTagNb += 1
			else:
				targetTagNb += 1

			if targetTagNb >= prereqTagNb:
				return True
	return False

def CheckTagOnTarget(stateList, x, y, targetX, targetY, targetTag, prereqTagNb, selfOppoCamp):
	targetStateStrs = stateList[targetY][targetX].split('/')
	if targetTag in targetStateStrs:
		return True
	else:
		return False
	
def CheckTagNext(stateList, x, y, targetX, targetY, targetTag, prereqTagNb, selfOppoCamp):
	targetTagNb = 0
	if selfOppoCamp == "self":
		if x == 0:
			if y == 0:
				stateStrs = stateList[y + 1][x].split('/')
				tags = stateStrs[24].split('&')
				if targetTag in tags:
					targetTagNb += 1



checkPrereqTagRuleDict = {
	'CheckTagOnBoard': CheckTagOnBoard,
	'Seperate': Seperate,
	'Diagonal': Diagonal,
	"DiagonalSeperate": DiagonalSeperate
}
