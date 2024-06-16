import random
import GlobalConst

def Three(stateList, x, y):
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

