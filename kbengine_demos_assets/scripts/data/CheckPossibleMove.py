import random
import GlobalConst

def MoveLine(stateList, x, y, agility):
	possibleGrids = []
	for right in range(1, agility + 1, 1):
		if x + right >= GlobalConst.maxCol:
			break
		if stateList[y][x + right] == "--":
			possibleGrids.append([x + right, y])

	for left in range(1, agility + 1, 1):
		if x - left < 0:
			break
		if stateList[y][x - left] == "--":
			possibleGrids.append([x - left, y])

	for up in range(1, agility + 1, 1):
		if y + up >= GlobalConst.maxRow:
			break
		if stateList[y + up][x] == "--":
			possibleGrids.append([x, y + up])

	for down in range(1, agility + 1, 1):
		if y - down < 0:
			break
		if stateList[y - down][x] == "--":
			possibleGrids.append([x, y - down])
	return possibleGrids

def MoveDiagonal(stateList, x, y, agility):
	possibleGrids = []
	for rightUp in range(1, agility + 1, 1):
		if x + rightUp >= GlobalConst.maxCol or y + rightUp >= GlobalConst.maxRow:
			break
		if stateList[y + rightUp][x + rightUp] == "--":
			possibleGrids.append([x + rightUp, y + rightUp])

	for leftUp in range(1, agility + 1, 1):
		if x - leftUp < 0 or y + leftUp >= GlobalConst.maxRow:
			break
		if stateList[y + leftUp][x - leftUp] == "--":
			possibleGrids.append([x - leftUp, y + leftUp])

	for rightDown in range(1, agility + 1, 1):
		if x + rightDown >= GlobalConst.maxCol or y - rightDown < 0:
			break
		if stateList[y - rightDown][x + rightDown] == "--":
			possibleGrids.append([x + rightDown, y - rightDown])

	for leftDown in range(1, agility + 1, 1):
		if x - leftDown < 0 or y - leftDown < 0:
			break
		if stateList[y - leftDown][x - leftDown] == "--":
			possibleGrids.append([x - leftDown, y - leftDown])
	return possibleGrids

checkPossibleMove = {
	'MoveLine': MoveLine,
	'MoveDiagonal': MoveDiagonal
}
