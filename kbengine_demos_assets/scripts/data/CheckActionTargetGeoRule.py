import random
import GlobalConst

def Line(stateList, x, y, targetX, targetY, selfOppoCampFlag, distance):
	if x < 0 or x >= GlobalConst.maxCol or y < 0 or y >= GlobalConst.maxRow or x==targetX or y==targetY:
		return False
	if targetX < 0 or targetX >= GlobalConst.maxCol or targetY < 0 or targetY >= GlobalConst.maxRow:
		return False
	
	offsetX = targetX - x
	offsetY = targetY - y
	if offsetX != 0 and offsetY != 0:
		return False
	
	if abs(offsetX) > distance or abs(offsetY) > distance:
		return False
	
	if offsetX != 0:
		if targetX < x:
			for left in range(1, abs(offsetX), 1):
				if stateList[y][x - left] != "--":
					return False
		else:
			for right in range(1, abs(offsetX), 1):
				if stateList[y][x + right] != "--":
					return False
	else:
		if targetY < y:
			for down in range(1, abs(offsetY), 1):
				if stateList[y - down][x] != "--":
					return False
		else:
			for up in range(1, abs(offsetX), 1):
				if stateList[y + up][x] != "--":
					return False
	
	return True


def Seperate(stateList, x, y, targetX, targetY, selfOppoCampFlag, distance):
	if x < 0 or x >= GlobalConst.maxCol or y < 0 or y >= GlobalConst.maxRow or x==targetX or y==targetY:
		return False
	if targetX < 0 or targetX >= GlobalConst.maxCol or targetY < 0 or targetY >= GlobalConst.maxRow:
		return False
	
	offsetX = targetX - x
	offsetY = targetY - y
	if offsetX != 0 and offsetY != 0:
		return False
	
	if abs(offsetX) > distance or abs(offsetY) > distance:
		return False
	
	barbetteSet = False
	if offsetX != 0:
		if targetX < x:
			for left in range(1, abs(offsetX), 1):
				if barbetteSet == True:
					if stateList[y][x - left] != "--":
						return False
				else:
					if stateList[y][x - left] != "--":
						barbetteSet = True
		else:
			for right in range(1, abs(offsetX), 1):
				if barbetteSet == True:
					if stateList[y][x + right] != "--":
						return False
				else:
					if stateList[y][x + right] != "--":
						barbetteSet = True
	else:
		if targetY < y:
			for down in range(1, abs(offsetY), 1):
				if barbetteSet == True:
					if stateList[y - down][x] != "--":
						return False
				else:
					if stateList[y - down][x] != "--":
						barbetteSet = True
		else:
			for up in range(1, abs(offsetY), 1):
				if barbetteSet == True:
					if stateList[y + up][x] != "--":
						return False
				else:
					if stateList[y + up][x] != "--":
						barbetteSet = True
	if barbetteSet == True:
		return True
	else:
		return False


def Diagonal(stateList, x, y, targetX, targetY, selfOppoCampFlag, distance):
	if x < 0 or x >= GlobalConst.maxCol or y < 0 or y >= GlobalConst.maxRow or x==targetX or y==targetY:
		return False
	if targetX < 0 or targetX >= GlobalConst.maxCol or targetY < 0 or targetY >= GlobalConst.maxRow:
		return False
	
	offsetX = targetX - x
	offsetY = targetY - y
	if abs(offsetX) != abs(offsetY):
		return False
	
	if abs(offsetX) > distance or abs(offsetY) > distance:
		return False
	
	if targetX < x:
		if targetY < y:
			# leftDown
			for leftDown in range(1, abs(offsetX), 1):
				if stateList[y - leftDown][x - leftDown] != "--":
					return False
		else:
			# leftUp
			for leftUp in range(1, abs(offsetX), 1):
				if stateList[y + leftUp][x - leftUp] != "--":
					return False
	else:
		if targetY < y:
			# rightDown
			for rightDown in range(1, abs(offsetX), 1):
				if stateList[y - rightDown][x + rightDown] != "--":
					return False
		else:
			for rightUp in range(1, abs(offsetX), 1):
				if stateList[y + rightUp][x + rightUp] != "--":
					return False
	
	return True


def DiagonalSeperate(stateList, x, y, targetX, targetY, selfOppoCampFlag, distance):
	if x < 0 or x >= GlobalConst.maxCol or y < 0 or y >= GlobalConst.maxRow or x==targetX or y==targetY:
		return False
	if targetX < 0 or targetX >= GlobalConst.maxCol or targetY < 0 or targetY >= GlobalConst.maxRow:
		return False
	
	offsetX = targetX - x
	offsetY = targetY - y
	if abs(offsetX) != abs(offsetY):
		return False
	
	if abs(offsetX) > distance or abs(offsetY) > distance:
		return False
	
	barbetteSet = False
	if targetX < x:
		if targetY < y:
			# leftDown
			for leftDown in range(1, abs(offsetX), 1):
				if stateList[y - leftDown][x - leftDown] != "--":
					if barbetteSet == True:
						return False
					else:
						barbetteSet = True
		else:
			# leftUp
			for leftUp in range(1, abs(offsetX), 1):
				if stateList[y + leftUp][x - leftUp] != "--":
					if barbetteSet == True:
						return False
					else:
						barbetteSet = True
	else:
		if targetY < y:
			# rightDown
			for rightDown in range(1, abs(offsetX), 1):
				if stateList[y - rightDown][x + rightDown] != "--":
					if barbetteSet == True:
						return False
					else:
						barbetteSet = True
		else:
			for rightUp in range(1, abs(offsetX), 1):
				if stateList[y + rightUp][x + rightUp] != "--":
					if barbetteSet == True:
						return False
					else:
						barbetteSet = True
	if barbetteSet == True:
		return True
	else:
		return False
	

checkActionTargetGeoRuleDict = {
	'Line': Line,
	'Seperate': Seperate,
	'Diagonal': Diagonal,
	"DiagonalSeperate": DiagonalSeperate
}
