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


def HornHurtDiagonal(state_list, x, y, targetX, targetY, effectInfo):
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

