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
		if y + up > GlobalConst.boardEndRow:
			break
		if stateList[y + up][x] == "--":
			possibleGrids.append([x, y + up])

	for down in range(1, agility + 1, 1):
		if y - down < GlobalConst.boardStartRow:
			break
		if stateList[y - down][x] == "--":
			possibleGrids.append([x, y - down])
	return possibleGrids

def MoveDiagonal(stateList, x, y, agility):
	possibleGrids = []
	for rightUp in range(1, agility + 1, 1):
		if x + rightUp >= GlobalConst.maxCol or y + rightUp > GlobalConst.boardEndRow:
			break
		if stateList[y + rightUp][x + rightUp] == "--":
			possibleGrids.append([x + rightUp, y + rightUp])

	for leftUp in range(1, agility + 1, 1):
		if x - leftUp < 0 or y + leftUp > GlobalConst.boardEndRow:
			break
		if stateList[y + leftUp][x - leftUp] == "--":
			possibleGrids.append([x - leftUp, y + leftUp])

	for rightDown in range(1, agility + 1, 1):
		if x + rightDown >= GlobalConst.maxCol or y - rightDown < GlobalConst.boardStartRow:
			break
		if stateList[y - rightDown][x + rightDown] == "--":
			possibleGrids.append([x + rightDown, y - rightDown])

	for leftDown in range(1, agility + 1, 1):
		if x - leftDown < 0 or y - leftDown < GlobalConst.boardStartRow:
			break
		if stateList[y - leftDown][x - leftDown] == "--":
			possibleGrids.append([x - leftDown, y - leftDown])
	return possibleGrids

def MoveSeperate(stateList, x, y, agility):
	possibleGrids = []
	for right in range(1, agility + 1, 1):
		if x + right >= GlobalConst.maxCol:
			break
		rightBarbette = False
		if rightBarbette == False:
			if stateList[y][x + right] != "--":
				rightBarbette = True
		else:
			if stateList[y][x + right] != "--":
				possibleGrids.append([x + right, y])

	for left in range(1, agility + 1, 1):
		if x - left < 0:
			break
		leftBarbette = False
		if leftBarbette == False:
			if stateList[y][x - left] != "--":
				leftBarbette = True
		else:
			if stateList[y][x - left] != "--":
				possibleGrids.append([x - left, y])

	for up in range(1, agility + 1, 1):
		if y + up > GlobalConst.boardEndRow:
			break
		upBarbette = False
		if upBarbette == False:
			if stateList[y + up][x] != "--":
				upBarbette = True
		else:
			if stateList[y + up][x] != "--":
				possibleGrids.append([x, y + up])

	for down in range(1, agility + 1, 1):
		if y - down < GlobalConst.boardStartRow:
			break
		downBarbette = False
		if downBarbette == False:
			if stateList[y - down][x] != "--":
				downBarbette = True
		else:
			if stateList[y - down][x] != "--":
				possibleGrids.append([x, y - down])

	return possibleGrids

def MoveDiagonalSeperate(stateList, x, y, agility):
	possibleGrids = []
	for rightUp in range(1, agility + 1, 1):
		if x + rightUp >= GlobalConst.maxCol or y + rightUp > GlobalConst.boardEndRow:
			break
		rightUpBarbette = False
		if rightUpBarbette == False:
			if stateList[y + rightUp][x + rightUp] != "--":
				rightUpBarbette = True
		else:
			if stateList[y + rightUp][x + rightUp] != "--":
				possibleGrids.append([x + rightUp, y + rightUp])

	for leftUp in range(1, agility + 1, 1):
		if x - leftUp < 0 or y + leftUp > GlobalConst.boardEndRow:
			break
		leftUpBarbette = False
		if leftUpBarbette == False:
			if stateList[y + leftUp][x - leftUp] != "--":
				leftUpBarbette = True
		else:
			if stateList[y + leftUp][x - leftUp] != "--":
				possibleGrids.append([x - leftUp, y + leftUp])

	for rightDown in range(1, agility + 1, 1):
		if x + rightDown >= GlobalConst.maxCol or y - rightDown < GlobalConst.boardStartRow:
			break
		rightDownBarbette = False
		if rightDownBarbette == False:
			if stateList[y - rightDown][x + rightDown] != "--":
				rightDownBarbette = True
		else:
			if stateList[y - rightDown][x + rightDown] != "--":
				possibleGrids.append([x + rightDown, y - rightDown])

	for leftDown in range(1, agility + 1, 1):
		if x - leftDown < 0 or y - leftDown < GlobalConst.boardStartRow:
			break
		leftDownBarbette = False
		if leftDownBarbette == False:
			if stateList[y - leftDown][x - leftDown] != "--":
				leftDownBarbette = True
		else:
			if stateList[y - leftDown][x - leftDown] != "--":
				possibleGrids.append([x - leftDown, y - leftDown])

	return possibleGrids

checkPossibleMove = {
	"MoveLine": MoveLine,
	"MoveDiagonal": MoveDiagonal,
	"MoveDiagonal": MoveSeperate,
	"MoveDiagonalSeperate": MoveDiagonalSeperate
}
