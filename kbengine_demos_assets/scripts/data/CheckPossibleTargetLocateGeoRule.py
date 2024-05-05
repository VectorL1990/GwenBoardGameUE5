import random
import GlobalConst

def Three(stateList, checkGrids):
	resultGrids = []
	for i in range(len(checkGrids)):
		targetX = checkGrids[i][0]
		targetY = checkGrids[i][1]
		if targetX == 0 or targetX == GlobalConst.maxCol - 1:
			# only vertical is possible
			if targetY != 0 and targetY != GlobalConst.maxRow - 1:
				if stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
		else:
			if targetY == 0 or targetY == GlobalConst.maxRow - 1:
				if stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--":
					resultGrids.append(checkGrids[i])
			else:
				if (stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--") or (stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
	return resultGrids


def Horn(stateList, checkGrids):
	resultGrids = []
	for i in range(len(checkGrids)):
		targetX = checkGrids[i][0]
		targetY = checkGrids[i][1]
		if targetX == 0:
			if targetY == 0:
				if stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			elif targetY == GlobalConst.maxRow - 1:
				if stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			else:
				if (stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
		elif targetX == GlobalConst.maxCol - 1:
			if targetY == 0:
				if stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			elif targetY == GlobalConst.maxRow - 1:
				if stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			else:
				if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
		else:
			if targetY == 0:
				if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
			elif targetY == GlobalConst.maxRow - 1:
				if (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
			else:
				if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or \
					(stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
					resultGrids.append(checkGrids[i])
	return resultGrids

def Triangle(stateList, checkGrids):
	resultGrids = []
	for i in range(len(checkGrids)):
		targetX = checkGrids[i][0]
		targetY = checkGrids[i][1]
		if targetX == 0:
			if targetY != 0 and targetY != GlobalConst.maxRow - 1:
				if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX + 1] != "--":
					resultGrids.append(checkGrids[i])
		elif targetX == GlobalConst.maxCol - 1:
			if targetY != 0 and targetY != GlobalConst.maxRow - 1:
				if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--":
					resultGrids.append(checkGrids[i])
		else:
			if targetY == 0:
				if stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			elif targetY == GlobalConst.maxRow - 1:
				if stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--":
					resultGrids.append(checkGrids[i])
			else:
				if (stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or \
					(stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or \
					(stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--") or \
					(stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--" and stateList[targetY][targetX + 1] != "--"):
					resultGrids.append(checkGrids[i])
	return resultGrids

def NormalCross(stateList, checkGrids):
	resultGrids = []
	for i in range(len(checkGrids)):
		targetX = checkGrids[i][0]
		targetY = checkGrids[i][1]
		if targetX != 0 and targetX != GlobalConst.maxCol - 1 and targetY != 0 and targetY != GlobalConst.maxRow - 1:
			if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--":
				resultGrids.append(checkGrids[i])


def ObliqueCross(stateList, checkGrids):
	resultGrids = []
	for i in range(len(checkGrids)):
		targetX = checkGrids[i][0]
		targetY = checkGrids[i][1]
		if targetX != 0 and targetX != GlobalConst.maxCol - 1 and targetY != 0 and targetY != GlobalConst.maxRow - 1:
			if stateList[targetY + 1][targetX + 1] != "--" and stateList[targetY - 1][targetX + 1] != "--" and stateList[targetY + 1][targetX - 1] != "--" and stateList[targetY - 1][targetX - 1] != "--":
				resultGrids.append(checkGrids[i])

checkPossibleTargetLocationGeoRuleDict = {
	'Three': Three,
	'Horn': Horn,
	'Triangle': Triangle,
	"NormalCross": NormalCross,
	"ObliqueCross": ObliqueCross
}
