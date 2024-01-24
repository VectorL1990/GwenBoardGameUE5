import random
import GlobalConst


def FormationVShoot(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--" or (x == targetX and y == targetY):
		return False
	else:
		offsetX = targetX - x
		offsetY = targetY - y
		if offsetX < 2 or offsetY < 2 or targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		
		if abs(offsetX) > effectInfo["effectValues"]["distance"] or abs(offsetY) > effectInfo["effectValues"]["distance"]:
			return False
		
		if offsetX != 0 and offsetY != 0:
			return False
		
		launchStateStr = state_list[y][x]
		launchStateStrs = launchStateStr.split("/")
		if offsetX > 0:
			# target is located on right
			if y == 0 or y == 7 or x >= 6:
				return False
			if state_list[y + 1][x] == "--" or state_list[y - 1][x] == "--" or state_list[y][x + 1] == "--":
				return False
			else:
				targetStateStr = state_list[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					targetStateStrs = targetStateStr.split("/")
					if targetStateStrs[0] == launchStateStrs[0]:
						return False
					else:
						return True
		elif offsetX < 0:
			# target is located on left
			if y == 0 or y == 7 or x <= 1:
				return False
			if state_list[y + 1][x] == "--" or state_list[y - 1][x] == "--" or state_list[y][x - 1] == "--":
				return False
			else:
				targetStateStr = state_list[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					targetStateStrs = targetStateStr.split("/")
					if targetStateStrs[0] == launchStateStrs[0]:
						return False
					else:
						return True
		elif offsetY > 0:
			# target is located on up
			if x == 0 or x == 7 or y >= 6:
				return False
			if state_list[y][x + 1] == "--" or state_list[y][x - 1] == "--" or state_list[y + 1][x] == "--":
				return False
			else:
				targetStateStr = state_list[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					targetStateStrs = targetStateStr.split("/")
					if targetStateStrs[0] == launchStateStrs[0]:
						return False
					else:
						return True
		else:
			# target is located on down
			if x == 0 or x == 7 or y <= 1:
				return False
			if state_list[y][x + 1] == "--" or state_list[y][x - 1] == "--" or state_list[y - 1][x] == "--":
				return False
			else:
				targetStateStr = state_list[targetY][targetX]
				if targetStateStr == "--":
					return False
				else:
					targetStateStrs = targetStateStr.split("/")
					if targetStateStrs[0] == launchStateStrs[0]:
						return False
					else:
						return True

def DiamondCenterLaunchLocally(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		if x == 0 or x == 7 or y == 0 or y == 7:
			return False
		if state_list[y + 1][x] == "--" or state_list[y - 1][x] == "--" or state_list[y][x + 1] == "--" or state_list[y][x - 1] == "--":
			return False
		else:
			return True

def ThreeGridCenterLaunchLocally(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		if x == 0 or x == 7:
			# which means three grids can only be aligned vertically
			if y == 0 or y == 7:
				return False
			else:
				if state_list[y - 1][x] == "--" or state_list[y + 1][x] == "--":
					return False
				else:
					return True
		elif y == 0 or y == 7:
			# which means three grids can only be aligned horizontally
			if state_list[y][x - 1] == "--" or state_list[y][x + 1] == "--":
				return False
			else:
				return True
		else:
			if state_list[y][x - 1] != "--" and state_list[y][x + 1] != "--":
				return True
			if state_list[y - 1][x] != "--" and state_list[y + 1][x] != "--":
				return True
			return False

def ThreeGridCenterVerticalLine(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--" or (targetX == x and targetY == y):
		return False
	else:
		offsetX = targetX - X
		offsetY = targetY - y
		if offsetX != 0 and offsetY != 0:
			return False

		if x == 0 or x == 7:
			# which means three grids can only be aligned vertically
			if y == 0 or y == 7:
				return False
			else:
				if state_list[y - 1][x] == "--" or state_list[y + 1][x] == "--":
					return False
				else:
					if offsetY != 0:
						return False
					else:
						# check whether there are obstacles on the line
						if offsetX > 0:
							for i in range(x + 1, targetX, 1):
								if state_list[y][i] != "--":
									return False
						else:
							for i in range(x - 1, targetX,  -1):
								if state_list[y][i] != "--":
									return False
						return True
		elif y == 0 or y == 7:
			# which means three grids can only be aligned horizontally
			if state_list[y][x - 1] == "--" or state_list[y][x + 1] == "--":
				return False
			else:
				if offsetX != 0:
						return False
					else:
						# check whether there are obstacles on the line
						if offsetY > 0:
							for i in range(y + 1, targetY, 1):
								if state_list[i][x] != "--":
									return False
						else:
							for i in range(y - 1, targetY,  -1):
								if state_list[i][x] != "--":
									return False
						return True
		else:
			if state_list[y][x - 1] != "--" and state_list[y][x + 1] != "--":
				# check vertical
				if offsetX != 0:
					return False
				else:
					# check whether there are obstacles on the line
						if offsetY > 0:
							for i in range(y + 1, targetY, 1):
								if state_list[i][x] != "--":
									return False
						else:
							for i in range(y - 1, targetY,  -1):
								if state_list[i][x] != "--":
									return False
						return True
			if state_list[y - 1][x] != "--" and state_list[y + 1][x] != "--":
				# check horizontal
				if offsetY != 0:
					return False
				else:
					# check whether there are obstacles on the line
						if offsetX > 0:
							for i in range(x + 1, targetX, 1):
								if state_list[y][i] != "--":
									return False
						else:
							for i in range(x - 1, targetX,  -1):
								if state_list[y][i] != "--":
									return False
						return True


def HornCenterLaunchLocally(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		if x == 0:
			if y == 0:
				if state_list[y + 1][x] != "--" and state_list[y][x + 1] != "--":
					return True
				else:
					return False
			elif y == 7:
				if state_list[y - 1][x] != "--" and state_list[y][x + 1] != "--":
					return True
				else:
					return False
			else:
				if (state_list[y + 1][x] != "--" and state_list[y][x + 1] != "--") or (state_list[y - 1][x] != "--" and state_list[y][x + 1] != "--"):
					return True
				else:
					return False
		elif x == 7:
			if y == 0:
				if state_list[y + 1][x] != "--" and state_list[y][x - 1] != "--":
					return True
				else:
					return False
			elif y == 7:
				if state_list[y - 1][x] != "--" and state_list[y][x - 1] != "--":
					return True
				else:
					return False
			else:
				if (state_list[y + 1][x] != "--" and state_list[y][x - 1] != "--") or (state_list[y - 1][x] != "--" and state_list[y][x - 1] != "--"):
					return True
				else:
					return False
		else:
			if y == 0:
				if (state_list[y + 1][x] != "--" and state_list[y][x - 1] != "--") or (state_list[y + 1][x] != "--" and state_list[y][x + 1] != "--"):
					return True
				else:
					return False
			elif y == 7:
				if (state_list[y - 1][x] != "--" and state_list[y][x - 1] != "--") or (state_list[y - 1][x] != "--" and state_list[y][x + 1] != "--"):
					return True
				else:
					return False
			else:
				if (state_list[y + 1][x] != "--" and state_list[y][x - 1] != "--") or (state_list[y - 1][x] != "--" and state_list[y][x - 1] != "--") or (state_list[y + 1][x] != "--" and state_list[y][x + 1] != "--") or (state_list[y - 1][x] != "--" and state_list[y][x + 1] != "--"):
					return True
				else:
					return False

def FaceAcrossObstacle(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if offsetX != 0 and offsetY != 0:
			return False
		if targetY > y:
			obstacle = 0
			for i in range(y + 1, targetY, 1):
				if state_list[i][x] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetY < y:
			obstacle = 0
			for i in range(y - 1, targetY, -1):
				if state_list[i][x] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetX > x:
			obstacle = 0
			for i in range(x + 1, targetX, 1):
				if state_list[y][i] != "--":
					obstacle += 1
			if obstacle != 1:
				return False

		if targetX < x:
			obstacle = 0
			for i in range(x - 1, targetX, -1):
				if state_list[y][i] != "--":
					obstacle += 1
			if obstacle != 1:
				return False
		
		return True



def HornDiagonal(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY) or abs(offsetX) > effectInfo["effectValues"]["distance"]:
			return False
		
		LaunchStateStr = state_list[y][x]
		LaunchStateStrs = LaunchStateStr.split("/")
		if targetX < x:
			if targetY > y:
				# top left
				targetTLStateStr = state_list[y + 1][x - 1]
				if targetTLStateStr == "--":
					return False
				else:
					targetTLStateStrs = targetTLStateStr.split("/")
					if targetTLStateStrs[0] == LaunchStateStrs[0]:
						return False
					else:
						return True
			else:
				# bottom left
				targetBLStateStr = state_list[y - 1][x - 1]
				if targetBLStateStr == "--":
					return False
				else:
					targetBLStateStrs = targetBLStateStr.split("/")
					if targetBLStateStrs[0] == LaunchStateStrs[0]:
						return False
					else:
						return True
		else:
			if targetY > y:
				# top right
				targetTRStateStr = state_list[y + 1][x + 1]
				if targetTRStateStr == "--":
					return False
				else:
					targetTRStateStrs = targetTRStateStr.split("/")
					if targetTRStateStrs[0] == LaunchStateStrs[0]:
						return False
					else:
						return True
			else:
				# bottom right
				targetBRStateStr = state_list[y + 1][x + 1]
				if targetBRStateStr == "--":
					return False
				else:
					targetBRStateStrs = targetBRStateStr.split("/")
					if targetBRStateStrs[0] == LaunchStateStrs[0]:
						return False
					else:
						return True

def HornStraight(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == '--' or (x == targetX and y == targetY):
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False
		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) != abs(offsetY) or abs(offsetX) > effectInfo["effectValues"]["distance"]:
			return False
		
		LaunchStateStr = state_list[y][x]
		LaunchStateStrs = LaunchStateStr.split("/")

		if offsetX != 0 and offsetY != y:
			return False

		if targetX < x:
			# check whether horn face top right or bottom right
			if x > 6:
				return False
			else:
				if (y == 0 and (state_list[y + 1][x] == "--" or state_list[y][x + 1] == "--")) or \
					(y == 7 and (state_list[y - 1][x] == "--" or state_list[y][x + 1] == "--")) or \
					state_list[y][x - 1] == "--" or \
					(state_list[y - 1][x] == "--" and state_list[y + 1][x] == "--"):
						return False
				else:
					for i in range(x - 1, targetX, -1):
						if state_list[y][i] != "--":
							return False
					return True
		elif targetX < x:
			# check whether horn face top left or bottom left
			if x < 1:
				return False
			else:
				if (y == 0 and (state_list[y + 1][x] == "--" or state_list[y][x - 1] == "--")) or \
					(y == 7 and (state_list[y - 1][x] == "--" or state_list[y][x - 1] == "--")) or \
					state_list[y][x - 1] == "--" or \
					(state_list[y - 1][x] == "--" and state_list[y + 1][x] == "--"):
						return False
				else:
					for i in range(x - 1, targetX, 1):
						if state_list[y][i] != "--":
							return False
					return True
		elif targetY < y:
			# check whether horn face top right or bottom right
			if y > 6:
				return False
			else:
				if (x == 0 and (state_list[y + 1][x] == "--" or state_list[y][x + 1] == "--")) or \
					(x == 7 and (state_list[y + 1][x] == "--" or state_list[y][x - 1] == "--")) or \
					state_list[y + 1][x] == "--" or \
					(state_list[y][x - 1] == "--" and state_list[y][x + 1] == "--"):
						return False
				else:
					for i in range(y - 1, targetY, -1):
						if state_list[i][x] != "--":
							return False
					return True
		elif targetY > y:
			# check whether horn face top right or bottom right
			if y < 1:
				return False
			else:
				if (x == 0 and (state_list[y - 1][x] == "--" or state_list[y][x + 1] == "--")) or \
					(x == 7 and (state_list[y - 1][x] == "--" or state_list[y][x - 1] == "--")) or \
					state_list[y - 1][x] == "--" or \
					(state_list[y][x - 1] == "--" and state_list[y][x + 1] == "--"):
						return False
				else:
					for i in range(y - 1, targetY, 1):
						if state_list[i][x] != "--":
							return False
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
			

def DevourNextSpawnFar(state_list, x, y, targetX, targetY, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		if targetX < 0 or targetY < 0 or targetX > 7 or targetY > 7:
			return False

		offsetX = targetX - x
		offsetY = targetY - y
		if abs(offsetX) + abs(offsetY) > 1:
			return False

		launchStateStr = state_list[y][x]
		launchStateStrs = launchStateStr.split("/")
		if state_list[targetY][targetX] == "--":
			return False
		else:
			targetStateStr = state_list[targetY][targetX]
			targetStateStrs = targetStateStr.split('/')
			if targetStateStrs[0] != launchStateStrs[0]:
				return False
		
		
		spawnDistance = effectInfo["effectValues"]["distance"]
		if launchStateStrs[0] == 0:
			# which means spawn upward
			spawnY = y + spawnDistance
			if spawnY > 7 or state_list[spawnY][x] != "--":
				return False
		else:
			spawnY = y - spawnDistance
			if spawnY < 0 or state_list[spawnY][x] != "--":
				return False

		return True




rule_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

