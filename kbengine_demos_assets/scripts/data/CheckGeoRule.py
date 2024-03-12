import random
import GlobalConst

def Move(stateList, x, y, targetX, targetY):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		offsetX = targetX - x
		offsetY = targetY - y
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		obstacle = False
		if offsetX > 0:
			for i in range(1, offsetX, 1):
				if stateList[y][x + i] != "--":
					obstacle = True
					break
		elif offsetX < 0:
			for i in range(-1, offsetX, -1):
				if stateList[y][x + i] != "--":
					obstacle = True
					break
		elif offsetY > 0:
			for i in range(1, offsetY, 1):
				if stateList[y + i][x] != "--":
					obstacle = True
					break
		elif offsetY < 0:
			for i in range(-1, offsetY, -1):
				if stateList[y + i][x] != "--":
					obstacle = True
					break
		
		if obstacle == True:
			return False
		return True




def Seperated(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if offsetX != 0 and offsetY != 0:
			return False

		if abs(offsetX) > distance or abs(offsetY) > distance:
			return False

		if targetY > y:
			obstacle = 0
			for i in range(y + 1, targetY, 1):
				if stateList[i][x] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetY < y:
			obstacle = 0
			for i in range(y - 1, targetY, -1):
				if stateList[i][x] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetX > x:
			obstacle = 0
			for i in range(x + 1, targetX, 1):
				if stateList[y][i] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetX < x:
			obstacle = 0
			for i in range(x - 1, targetX, -1):
				if stateList[y][i] != "--":
					obstacle += 1
			if obstacle != 1:
				return False
		
		return True

def PointOnDiagonal(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY) or abs(offsetX) > distance:
			# which means target is not on diagonal
			return False

		if stateList[targetY][targetX] == "--":
			return False

		return True

def DiagonalSweep(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY):
			# which means target is not on diagonal
			return False
		else:
			return True



def HornDiagonal(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY) or abs(offsetX) > distance:
			return False
		
		if targetX < x:
			if targetY > y:
				# top left
				if y >= GlobalConst.maxRow - 2 or x < 2:
					return False
				else:
					if stateList[y][x - 1] == "--" or stateList[y + 1][x] == "--":
						return False
					else:
						if stateList[targetY][targetX] == "--":
							return False
						else:
							return True
			else:
				# bottom left
				if y < 2 or x < 2:
					return False
				else:
					if stateList[y][x - 1] == "--" or stateList[y - 1][x] == "--":
						return False
					else:
						if stateList[targetY][targetX] == "--":
							return False
						else:
							return True
		else:
			if targetY > y:
				# top right
				if y >= GlobalConst.maxRow - 2 or x >= GlobalConst.maxCol - 2:
					return False
				else:
					if stateList[y][x + 1] == "--" or stateList[y + 1][x] == "--":
						return False
					else:
						if stateList[targetY][targetX] == "--":
							return False
						else:
							return True
			else:
				# bottom right
				if y < 2 or x >= GlobalConst.maxCol - 2:
					return False
				else:
					if stateList[y][x + 1] == "--" or stateList[y - 1][x] == "--":
						return False
					else:
						if stateList[targetY][targetX] == "--":
							return False
						else:
							return True


def HornDiagonalArbitrary(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY) or abs(offsetX) > distance:
			return False
		
		if targetX < x:
			if targetY > y:
				# top left
				if y >= GlobalConst.maxRow - 2 or x < 2:
					return False
				else:
					if stateList[y][x - 1] == "--" or stateList[y + 1][x] == "--":
						return False
					else:
						return True
			else:
				# bottom left
				if y < 2 or x < 2:
					return False
				else:
					if stateList[y][x - 1] == "--" or stateList[y - 1][x] == "--":
						return False
					else:
						return True
		else:
			if targetY > y:
				# top right
				if y >= GlobalConst.maxRow - 2 or x >= GlobalConst.maxCol - 2:
					return False
				else:
					if stateList[y][x + 1] == "--" or stateList[y + 1][x] == "--":
						return False
					else:
						return True
			else:
				# bottom right
				if y < 2 or x >= GlobalConst.maxCol - 2:
					return False
				else:
					if stateList[y][x + 1] == "--" or stateList[y - 1][x] == "--":
						return False
					else:
						return True


def HornLocal(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or x != targetX or y != targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		
		return True


def NormalCrossLocal(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or x != targetX or y != targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False

		if x == 0 or x == GlobalConst.maxCol - 1 or y == 0 or y == GlobalConst.maxRow - 1:
			return False

		if stateList[y][x - 1] == "--" or stateList[y][x + 1] == "--" or stateList[y - 1][x] == "--" or stateList[y + 1][x] == "--":
			return False

		return True


def ObliqueCrossPointOnLine(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y

		if abs(offsetX) > distance or abs(offsetY) > distance:
			return False

		if abs(offsetX) != 0 and abs(offsetY) != 0:
			return False

		if x == 0 or x == GlobalConst.maxCol - 1 or y == 0 or y == GlobalConst.maxRow - 1:
			return False

		if stateList[x - 1][y - 1] == "--" or stateList[x - 1][y + 1] == "--" or stateList[x + 1][y - 1] == "--" or stateList[x + 1][y + 1] == "--":
			return False

		if stateList[targetY][targetX] == "--":
			return False

		return True


def ObliqueCrossLineSweep(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		offsetX = targetX - x
		offsetY = targetY - y

		if abs(offsetX) != 0 and abs(offsetY) != 0:
			return False

		if x == 0 or x == GlobalConst.maxCol - 1 or y == 0 or y == GlobalConst.maxRow - 1:
			return False

		if stateList[x - 1][y - 1] == "--" or stateList[x - 1][y + 1] == "--" or stateList[x + 1][y - 1] == "--" or stateList[x + 1][y + 1] == "--":
			return False

		return True


def TriangleSeperated(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		offsetX = targetX - x
		offsetY = targetY - y
		if offsetX < 2 or offsetY < 2 or targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol or targetY > GlobalConst.maxRow:
			return False
		
		if abs(offsetX) > distance or abs(offsetY) > distance:
			return False
		
		if offsetX != 0 and offsetY != 0:
			return False
		
		if offsetX > 0:
			# target is located on right
			if y == 0 or y == GlobalConst.maxRow - 1 or x >= GlobalConst.maxCol - 2:
				return False
			if stateList[y + 1][x] == "--" or stateList[y - 1][x] == "--" or stateList[y][x + 1] == "--":
				return False
			else:
				targetStateStr = stateList[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					for i in range(x + 2, targetX, 1):
						if stateList[y][i] != "--":
							return False
					return True
		elif offsetX < 0:
			# target is located on left
			if y == 0 or y == GlobalConst.maxRow - 1 or x <= 1:
				return False
			if stateList[y + 1][x] == "--" or stateList[y - 1][x] == "--" or stateList[y][x - 1] == "--":
				return False
			else:
				targetStateStr = stateList[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					for i in range(x - 2, targetX, -1):
						if stateList[y][i] != "--":
							return False
					return True
		elif offsetY > 0:
			# target is located on up
			if x == 0 or x == GlobalConst.maxRow - 1 or y >= GlobalConst.maxRow - 2:
				return False
			if stateList[y][x + 1] == "--" or stateList[y][x - 1] == "--" or stateList[y + 1][x] == "--":
				return False
			else:
				targetStateStr = stateList[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					for i in range(y + 2, targetY, 1):
						if stateList[i][x] != "--":
							return False
					return True
		else:
			# target is located on down
			if x == 0 or x == GlobalConst.maxRow - 1 or y <= 1:
				return False
			if stateList[y][x + 1] == "--" or stateList[y][x - 1] == "--" or stateList[y - 1][x] == "--":
				return False
			else:
				targetStateStr = stateList[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					for i in range(y - 2, targetY, -1):
						if stateList[i][x] != "--":
							return False
					return True


def TriangleSeperatedArbitrary(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		offsetX = targetX - x
		offsetY = targetY - y

		if offsetX < 2 or offsetY < 2 or targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		
		if offsetX != 0 and offsetY != 0:
			return False
		
		if offsetX > 0:
			# target is located on right
			if y == 0 or y == GlobalConst.maxRow - 1 or x >= GlobalConst.maxCol - 2:
				return False
			if stateList[y + 1][x] == "--" or stateList[y - 1][x] == "--" or stateList[y][x + 1] == "--":
				return False
			else:
				return True
		elif offsetX < 0:
			# target is located on left
			if y == 0 or y == GlobalConst.maxRow - 1 or x <= 1:
				return False
			if stateList[y + 1][x] == "--" or stateList[y - 1][x] == "--" or stateList[y][x - 1] == "--":
				return False
			else:
				return True
		elif offsetY > 0:
			# target is located on up
			if x == 0 or x == GlobalConst.maxCol - 1 or y >= GlobalConst.maxRow - 2:
				return False
			if stateList[y][x + 1] == "--" or stateList[y][x - 1] == "--" or stateList[y + 1][x] == "--":
				return False
			else:
				return True
		else:
			# target is located on down
			if x == 0 or x == GlobalConst.maxCol - 1 or y <= 1:
				return False
			if stateList[y][x + 1] == "--" or stateList[y][x - 1] == "--" or stateList[y - 1][x] == "--":
				return False
			else:
				return True
		

def ThreeGridsPerpendicular(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (targetX == x and targetY == y):
		return False
	else:
		offsetX = targetX - X
		offsetY = targetY - y
		if offsetX != 0 and offsetY != 0:
			return False

		if x == 0 or x == GlobalConst.maxCol - 1:
			# which means three grids can only be aligned vertically
			if y == 0 or y == GlobalConst.maxRow - 1:
				return False
			else:
				if stateList[y - 1][x] == "--" or stateList[y + 1][x] == "--":
					return False
				else:
					if offsetY != 0:
						return False
					else:
						# check whether there are obstacles on the line
						if offsetX > 0:
							for i in range(x + 1, targetX, 1):
								if stateList[y][i] != "--":
									return False
						else:
							for i in range(x - 1, targetX,  -1):
								if stateList[y][i] != "--":
									return False
						return True
		elif y == 0 or y == GlobalConst.maxCol - 1:
			# which means three grids can only be aligned horizontally
			if stateList[y][x - 1] == "--" or stateList[y][x + 1] == "--":
				return False
			else:
				if offsetX != 0:
					return False
				else:
					# check whether there are obstacles on the line
					if offsetY > 0:
						for i in range(y + 1, targetY, 1):
							if stateList[i][x] != "--":
								return False
					else:
						for i in range(y - 1, targetY,  -1):
							if stateList[i][x] != "--":
								return False
					return True
		else:
			if stateList[y][x - 1] != "--" and stateList[y][x + 1] != "--":
				# check vertical
				if offsetX != 0:
					return False
				else:
					# check whether there are obstacles on the line
					if offsetY > 0:
						for i in range(y + 1, targetY, 1):
							if stateList[i][x] != "--":
								return False
					else:
						for i in range(y - 1, targetY,  -1):
							if stateList[i][x] != "--":
								return False
					return True
			if stateList[y - 1][x] != "--" and stateList[y + 1][x] != "--":
				# check horizontal
				if offsetY != 0:
					return False
				else:
					# check whether there are obstacles on the line
					if offsetX > 0:
						for i in range(x + 1, targetX, 1):
							if stateList[y][i] != "--":
								return False
					else:
						for i in range(x - 1, targetX,  -1):
							if stateList[y][i] != "--":
								return False
					return True


def threeGridsPerpendicularArbitrary(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (targetX == x and targetY == y):
		return False
	else:
		offsetX = targetX - X
		offsetY = targetY - y
		if offsetX != 0 and offsetY != 0:
			return False

		if x == 0 or x == GlobalConst.maxCol - 1:
			# which means three grids can only be aligned vertically
			if y == 0 or y == GlobalConst.maxRow - 1:
				return False
			else:
				if stateList[y - 1][x] == "--" or stateList[y + 1][x] == "--":
					return False
				else:
					if offsetY != 0:
						return False
					else:
						return True
		elif y == 0 or y == GlobalConst.maxCol - 1:
			# which means three grids can only be aligned horizontally
			if stateList[y][x - 1] == "--" or stateList[y][x + 1] == "--":
				return False
			else:
				if offsetX != 0:
					return False
				else:
					return True
		else:
			if stateList[y][x - 1] != "--" and stateList[y][x + 1] != "--":
				# check vertical
				if offsetX != 0:
					return False
				else:
					return True
			if stateList[y - 1][x] != "--" and stateList[y + 1][x] != "--":
				# check horizontal
				if offsetY != 0:
					return False
				else:
					return True



def SpawnForward(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		
		launchStateStr = state_list[y][x]
		launchStateStrs = launchStateStr.split('/')
		if launchStateStrs[0] == '0':
			if y == 7 or state_list[y + 1][x] != "--":
				return False
		else:
			if y == 0 or state_list[y - 1][x] != "--":
				return False
		return True
			


	
def CheckConnect(stateList, x, y, targetX, targetY, launchCamp, searchOppo, searchedList):
	targetYStr = str(targetY)
	targetXStr = str(targetX)
	targetGridCombineStr = targetYStr + "_" + targetXStr
	if y < GlobalConst.maxRow - 1 and stateList[y + 1][x] != "--":
		stateStrs = stateList[y + 1][x].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y + 1)
			colStr = str(x)
			combineStr = rowStr + "_" + colStr
			if combineStr == targetGridCombineStr:
				# which means we have found target Grid
				return True
			else:
				# keep searching
				if combineStr not in searchedList:
					searchedList.append(combineStr)
					searchResult = CheckConnect(stateList, x, y + 1, targetX, targetY, launchCamp, searchOppo, searchedList)
					if searchResult == True:
						return True
	if y > 0 and stateList[y - 1][x] != "--":
		stateStrs = stateList[y - 1][x].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y - 1)
			colStr = str(x)
			combineStr = rowStr + "_" + colStr
			if combineStr == targetGridCombineStr:
				return True
			else:
				if combineStr not in searchedList:
					searchedList.append(combineStr)
					searchResult = CheckConnect(stateList, x, y - 1, targetX, targetY, launchCamp, searchOppo, searchedList)
					if searchResult == True:
						return True
	if x < GlobalConst.maxCol - 1 and stateList[y][x + 1] != "--":
		stateStrs = stateList[y][x + 1].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y)
			colStr = str(x + 1)
			combineStr = rowStr + "_" + colStr
			if combineStr == targetGridCombineStr:
				return True
			else:
				if combineStr not in searchedList:
					searchedList.append(combineStr)
					searchResult = CheckConnect(stateList, x + 1, y, targetX, targetY, launchCamp, searchOppo, searchedList)
					if searchResult == True:
						return True
	if x > 0 and stateList[y][x - 1] != "--":
		stateStrs = stateList[y][x - 1].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y)
			colStr = str(x - 1)
			combineStr = rowStr + "_" + colStr
			if combineStr == targetGridCombineStr:
				return True
			else:
				if combineStr not in searchedList:
					searchedList.append(combineStr)
					searchResult = CheckConnect(stateList, x - 1, y, targetX, targetY, launchCamp, searchOppo, searchedList)
					if searchResult == True:
						return True
	return False
	
	

def SearchLink(stateList, x, y, launchCamp, searchOppo, searchedList):
	if y < GlobalConst.maxRow - 1 and stateList[y + 1][x] != "--":
		stateStrs = stateList[y + 1][x].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y + 1)
			colStr = str(x)
			combineStr = rowStr + "_" + colStr
			if combineStr not in searchedList:
				searchedList.append(combineStr)
				SearchLink(stateList, x, y + 1, searchedList)
	if y > 0 and stateList[y - 1][x] != "--":
		stateStrs = stateList[y - 1][x].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y - 1)
			colStr = str(x)
			combineStr = rowStr + "_" + colStr
			if combineStr not in searchedList:
				searchedList.append(combineStr)
				SearchLink(stateList, x, y - 1, searchedList)
	if x < GlobalConst.maxCol - 1 and stateList[y][x + 1] != "--":
		stateStrs = stateList[y][x + 1].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y)
			colStr = str(x + 1)
			combineStr = rowStr + "_" + colStr
			if combineStr not in searchedList:
				searchedList.append(combineStr)
				SearchLink(stateList, x + 1, y, searchedList)
	if x > 0 and stateList[y][x - 1] != "--":
		stateStrs = stateList[y][x - 1].split("/")
		if (searchOppo == True and stateStrs[2] != launchCamp) or (searchOppo == False and stateStrs[2] == launchCamp):
			rowStr = str(y)
			colStr = str(x - 1)
			combineStr = rowStr + "_" + colStr
			if combineStr not in searchedList:
				searchedList.append(combineStr)
				SearchLink(stateList, x - 1, y, searchedList)

def Teleport(stateList, x, y, targetX, targetY, distance):
	if stateList[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		offsetX = targetX - x
		offsetY = targetY - y
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False
		obstacle = False
		if offsetX > 0:
			for i in range(1, offsetX, 1):
				if stateList[y][x + i] != "--":
					obstacle = True
					break
		elif offsetX < 0:
			for i in range(-1, offsetX, -1):
				if stateList[y][x + i] != "--":
					obstacle = True
					break
		elif offsetY > 0:
			for i in range(1, offsetY, 1):
				if stateList[y + i][x] != "--":
					obstacle = True
					break
		elif offsetY < 0:
			for i in range(-1, offsetY, -1):
				if stateList[y + i][x] != "--":
					obstacle = True
					break
		
		if obstacle == True:
			return False

		targetStateStr = stateList[targetY][targetX]
		targetStateStrs = targetStateStr.split('/')
		linkPairNb = targetStateStrs[11]
		# find teleport grid
		for i in range(0, GlobalConst.maxRow, 1):
			for j in range(0, GlobalConst.maxCol, 1):
				if stateList[i][j] != "--" and (i != targetY or j != targetX):
					stateStrs = stateList[i][j].split('/')
					if stateStrs[11] == linkPairNb:
						# calculate grid offset
						offsetX = targetX - x
						offsetY = targetY - y
						teleportX = j - offsetX
						teleportY = i - offsetY
						if teleportX < 0 or teleportX > GlobalConst.maxCol - 1 or teleportY < 0 or teleportY > GlobalConst.maxRow - 1 or stateList[teleportY][teleportX] != "--":
							return False
						else:
							return True
		return False




def DevourNextSpawnFar(stateList, x, y, targetX, targetY, selfOrOppoCamp, distance):
	if stateList[y][x] == "--":
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False

		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) + abs(offsetY) > 1:
			return False

		launchStateStr = stateList[y][x]
		launchStateStrs = launchStateStr.split("/")
		if stateList[targetY][targetX] == "--":
			return False
		else:
			targetStateStr = stateList[targetY][targetX]
			targetStateStrs = targetStateStr.split('/')
			if selfOrOppoCamp == True:
				# which means it can only devours self camp card
				if targetStateStrs[2] != launchStateStrs[2]:
					return False
			else:
				# which means it only devours oppo camp card
				if targetStateStrs[2] == launchStateStrs[2]:
					return False
		
		
		if launchStateStrs[2] == 0:
			# which means launch card belongs to down section, it spawns card in up section
			spawnY = y + distance
			if spawnY > GlobalConst.maxRow - 1 or stateList[spawnY][x] != "--":
				return False
		else:
			# which means launching card belongs to up section, it spawns card in down section
			spawnY = y - distance
			if spawnY < 0 or stateList[spawnY][x] != "--":
				return False

		return True


def DevourNextSpawnArbitrary(stateList, x, y, targetX, targetY, selfOrOppoCamp, distance):
	if stateList[y][x] == "--":
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > GlobalConst.maxCol - 1 or targetY > GlobalConst.maxRow - 1:
			return False

		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) + abs(offsetY) > 1:
			return False

		launchStateStr = stateList[y][x]
		launchStateStrs = launchStateStr.split("/")
		if stateList[targetY][targetX] == "--":
			return False
		else:
			targetStateStr = stateList[targetY][targetX]
			targetStateStrs = targetStateStr.split('/')
			if selfOrOppoCamp == True:
				# which means it can only devours self camp card
				if targetStateStrs[2] != launchStateStrs[2]:
					return False
			else:
				# which means it only devours oppo camp card
				if targetStateStrs[2] == launchStateStrs[2]:
					return False
		
		
		if launchStateStrs[2] == 0:
			# which means launch card belongs to down section, it spawns card in up section
			spawnY = y + distance
			if spawnY > GlobalConst.maxRow - 1:
				return False
		else:
			# which means launching card belongs to up section, it spawns card in down section
			spawnY = y - distance
			if spawnY < 0:
				return False

		return True




rule_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

