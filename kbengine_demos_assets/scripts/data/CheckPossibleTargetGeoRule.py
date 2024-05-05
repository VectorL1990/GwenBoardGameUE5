import random
import GlobalConst

def Line(stateList, x, y, selfOppoCampFlag, distance):
	possibleGrids = []
	isRightBlock = False
	isLeftBlock = False
	isUpBlock = False
	isDownBlock = False
	for i in range(distance):
		if isRightBlock == False and x + i + 1 <= GlobalConst.maxCol - 1:
			if stateList[y][x + i + 1] != "--":
				possibleGrids.append([x+i+1, y])
				isRightBlock = True
		else:
			isRightBlock = True

		# left direction
		if isLeftBlock == False and x - i - 1 >= 0:
			if stateList[y][x - i - 1] != "":
				possibleGrids.append([x-i-1, y])
				isLeftBlock = True
		else:
			isLeftBlock = True

		# up direction
		if isUpBlock == False and y + i + 1 <= GlobalConst.maxRow - 1:
			if stateList[y + i + 1][x] != "":
				possibleGrids.append([x, y+i+1])
				isUpBlock = True
		else:
			isUpBlock = True

		# down direction
		if isDownBlock == False and y - i - 1 >= 0:
			if stateList[y - i - 1][x] != "":
				possibleGrids.append([x, y - i - 1])
				isDownBlock = True
		else:
			isDownBlock = True
	return possibleGrids

def Seperate(stateList, x, y, selfOppoCampFlag, distance):
	possibleGrids = []
	isRightEnd = False
	isLeftEnd = False
	isUpEnd = False
	isDownEnd = False

	isRightBarbetteSet = False
	isLeftBarbetteSet = False
	isUpBarbetteSet = False
	isDownBarbetteSet = False

	isRightBlock = False
	isLeftBlock = False
	isUpBlock = False
	isDownBlock = False
	for i in range(distance):
		if isRightEnd == False:
			if x + i + 1 <= GlobalConst.maxCol - 1:
				if isRightBlock == False:
					if isRightBarbetteSet == False:
						if stateList[y][x + i + 1] != "--":
							isRightBarbetteSet = True
					else:
						if stateList[y][x + i + 1] != "--":
							possibleGrids.append([x + i + 1, y])
							isRightBlock = True
			else:
				isRightEnd = True


		if isLeftEnd == False:
			if x - i - 1 >= 0:
				if isLeftBlock == False:
					if isLeftBarbetteSet == False:
						if stateList[y][x - i - 1] != "--":
							isLeftBarbetteSet = True
					else:
						if stateList[y][x - i - 1] != "--":
							possibleGrids.append([x - i - 1, y])
							isLeftBlock = True
			else:
				isLeftEnd = True


		if isUpEnd == False:
			if y + i + 1 <= GlobalConst.maxRow - 1:
				if isUpBlock == False:
					if isUpBarbetteSet == False:
						if stateList[y + i + 1][x] != "--":
							isUpBarbetteSet = True
					else:
						if stateList[y + i + 1][x] != "--":
							possibleGrids.append([x, y + i + 1])
							isUpBlock = True
			else:
				isUpEnd = True


		if isDownEnd == False:
			if y - i - 1 >= 0:
				if isDownBlock == False:
					if isDownBarbetteSet == False:
						if stateList[y - i - 1][x] != "--":
							isDownBarbetteSet = True
					else:
						if stateList[y - i - 1][x] != "--":
							possibleGrids.append([x, y - i - 1])
							isDownBlock = True
			else:
				isDownEnd = True

		
	return possibleGrids


def Diagonal(stateList, x, y, selfOppoCampFlag, distance):
	possibleGrids = []
	isRightUpBlock = False
	isLeftUpBlock = False
	isRightDownBlock = False
	isLeftDownBlock = False
	for i in range(distance):
		if isRightUpBlock == False and x + i + 1 <= GlobalConst.maxCol - 1 and y + i + 1 <= GlobalConst.maxCol - 1:
			if stateList[y + i + 1][x + i + 1] != "--":
				possibleGrids.append([x+i+1, y+i+1])
				isRightUpBlock = True
		else:
			isRightUpBlock = True

		# left direction
		if isLeftUpBlock == False and x - i - 1 >= 0 and y + i + 1 <= GlobalConst.maxCol - 1:
			if stateList[y +i +1][x - i - 1] != "":
				possibleGrids.append([x-i-1, y+i+1])
				isLeftUpBlock = True
		else:
			isLeftUpBlock = True

		# up direction
		if isRightDownBlock == False and y - i - 1 >= 0 and x + i + 1 <= GlobalConst.maxCol - 1:
			if stateList[y - i - 1][x + i + 1] != "":
				possibleGrids.append([x + i + 1, y - i - 1])
				isRightDownBlock = True
		else:
			isRightDownBlock = True

		# down direction
		if isLeftDownBlock == False and y - i - 1 >= 0 and x - i - 1 >= 0:
			if stateList[y - i - 1][x - i - 1] != "":
				possibleGrids.append([x - i - 1, y - i - 1])
				isLeftDownBlock = True
		else:
			isLeftDownBlock = True
	return possibleGrids


def DiagonalSeperate(stateList, x, y, selfOppoCampFlag, distance):
	possibleGrids = []
	isRightUpEnd = False
	isLeftUpEnd = False
	isRightDownEnd = False
	isLeftDownEnd = False

	isRightUpBarbetteSet = False
	isLeftUpBarbetteSet = False
	isRightDownBarbetteSet = False
	isLeftDownBarbetteSet = False

	isRightUpBlock = False
	isLeftUpBlock = False
	isRightDownBlock = False
	isLeftDownBlock = False
	for i in range(distance):
		if isRightUpEnd == False:
			if x + i + 1 <= GlobalConst.maxCol - 1 and y + i + 1 <= GlobalConst.maxRow - 1:
				if isRightUpBlock == False:
					if isRightUpBarbetteSet == False:
						if stateList[y + i + 1][x + i + 1] != "--":
							isRightUpBarbetteSet = True
					else:
						if stateList[y + i + 1][x + i + 1] != "--":
							possibleGrids.append([x + i + 1, y + i + 1])
							isRightUpBlock = True
			else:
				isRightUpEnd = True


		if isLeftUpEnd == False:
			if x - i - 1 >= 0 and y + i + 1 <= GlobalConst.maxRow - 1:
				if isLeftUpBlock == False:
					if isLeftUpBarbetteSet == False:
						if stateList[y + i + 1][x - i - 1] != "--":
							isLeftUpBarbetteSet = True
					else:
						if stateList[y + i + 1][x - i - 1] != "--":
							possibleGrids.append([x - i - 1, y + i + 1])
							isLeftUpBlock = True
			else:
				isLeftUpEnd = True


		if isRightDownEnd == False:
			if y - i - 1 >= 0 and x + i + 1 <= GlobalConst.maxCol - 1:
				if isRightDownBlock == False:
					if isRightDownBarbetteSet == False:
						if stateList[y - i - 1][x + i + 1] != "--":
							isRightDownBarbetteSet = True
					else:
						if stateList[y - i - 1][x + i + 1] != "--":
							possibleGrids.append([x + i + 1, y - i - 1])
							isRightDownBlock = True
			else:
				isRightDownEnd = True


		if isLeftDownEnd == False:
			if y - i - 1 >= 0 and x - i - 1 >= 0:
				if isLeftDownBlock == False:
					if isLeftDownBarbetteSet == False:
						if stateList[y - i - 1][x - i - 1] != "--":
							isLeftDownBarbetteSet = True
					else:
						if stateList[y - i - 1][x - i - 1] != "--":
							possibleGrids.append([x - i - 1, y - i - 1])
							isLeftDownBlock = True
			else:
				isLeftDownEnd = True

		
	return possibleGrids
	

checkPossibleTargetGeoRuleDict = {
	'Line': Line,
	'Seperate': Seperate,
	'Diagonal': Diagonal,
	"DiagonalSeperate": DiagonalSeperate
}
