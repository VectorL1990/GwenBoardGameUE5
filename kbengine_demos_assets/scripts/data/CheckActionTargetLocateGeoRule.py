import random
import GlobalConst

def Three(stateList, targetX, targetY):
	if targetX == 0 or targetX == GlobalConst.maxCol - 1:
		# only vertical is possible
		if targetY != 0 and targetY != GlobalConst.maxRow - 1:
			if stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--":
				return True
	else:
		if targetY == 0 or targetY == GlobalConst.maxRow - 1:
			if stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--":
				return True
		else:
			if (stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--") or (stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--"):
				return True
	return False


def Horn(stateList, targetX, targetY):
	if targetX == 0:
		if targetY == 0:
			if stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--":
				return True
		elif targetY == GlobalConst.maxRow - 1:
			if stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--":
				return True
		else:
			if (stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
				return True
	elif targetX == GlobalConst.maxCol - 1:
		if targetY == 0:
			if stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--":
				return True
		elif targetY == GlobalConst.maxRow - 1:
			if stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--":
				return True
		else:
			if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--"):
					return True
	else:
		if targetY == 0:
			if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--"):
				return True
		elif targetY == GlobalConst.maxRow - 1:
			if (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
				return True
		else:
			if (stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or \
				(stateList[targetY][targetX + 1] != "--" and stateList[targetY + 1][targetX] != "--") or (stateList[targetY][targetX + 1] != "--" and stateList[targetY - 1][targetX] != "--"):
				return True
	return False

def Triangle(stateList, targetX, targetY):
	if targetX == 0:
		if targetY != 0 and targetY != GlobalConst.maxRow - 1:
			if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX + 1] != "--":
				return True
	elif targetX == GlobalConst.maxCol - 1:
		if targetY != 0 and targetY != GlobalConst.maxRow - 1:
			if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--":
				return True
	else:
		if targetY == 0:
			if stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--":
				return True
		elif targetY == GlobalConst.maxRow - 1:
			if stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--":
				return True
		else:
			if (stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY - 1][targetX] != "--") or \
				(stateList[targetY][targetX + 1] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY + 1][targetX] != "--") or \
				(stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--") or \
				(stateList[targetY - 1][targetX] != "--" and stateList[targetY + 1][targetX] != "--" and stateList[targetY][targetX + 1] != "--"):
				return True
	return False

def NormalCross(stateList, targetX, targetY):
	if targetX != 0 and targetX != GlobalConst.maxCol - 1 and targetY != 0 and targetY != GlobalConst.maxRow - 1:
		if stateList[targetY + 1][targetX] != "--" and stateList[targetY - 1][targetX] != "--" and stateList[targetY][targetX - 1] != "--" and stateList[targetY][targetX + 1] != "--":
			return True
	return False


def ObliqueCross(stateList, targetX, targetY):
	if targetX != 0 and targetX != GlobalConst.maxCol - 1 and targetY != 0 and targetY != GlobalConst.maxRow - 1:
		if stateList[targetY + 1][targetX + 1] != "--" and stateList[targetY - 1][targetX + 1] != "--" and stateList[targetY + 1][targetX - 1] != "--" and stateList[targetY - 1][targetX - 1] != "--":
			return True
	return False

checkActionTargetLocationGeoRuleDict = {
	'Three': Three,
	'Horn': Horn,
	'Triangle': Triangle,
	"NormalCross": NormalCross,
	"ObliqueCross": ObliqueCross
}