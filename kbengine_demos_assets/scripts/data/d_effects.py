import random
import GlobalConst

def getGridRowAndCol(gridNb):
	col = gridNb % GlobalConst.g_boardColumn
	row = gridNb / GlobalConst.g_boardColumn
	rc = [row, col]
	return rc
	
def getGridNbByRowAndCol(row, col):
	if row < 0 or row > 2*GlobalConst.g_boardHalfRow:
		return -1
	elif col < 0 or col > GlobalConst.g_boardColumn:
		return -1
	else:
		return row*GlobalConst.g_boardColumn + col

def calculateCardHp(uniqueCardDict, cardUid, hurt):
	hurtDict = {
		"defenceLoss": 0,
		"hpLoss": 0
	}
	# return dead or not
	if hurt <= uniqueCardDict[cardUid]["defence"]:
		uniqueCardDict[cardUid]["defence"] -= hurt
		hurtDict["defenceLoss"] = hurt
		return hurtDict
	else:
		overflow = hurt - uniqueCardDict[cardUid]["defence"]
		hurtDict["defenceLoss"] = uniqueCardDict[cardUid]["defence"]
		if overflow < uniqueCardDict[cardUid]["hp"]:
			uniqueCardDict[cardUid]["hp"] -= overflow
			hurtDict["hpLoss"] = overflow
			return hurtDict
		else:
			hurtDict["hpLoss"] = uniqueCardDict[cardUid]["hp"]
			uniqueCardDict[cardUid]["hp"] = 0
			return hurtDict


def FormationVShoot(uniqueCardDict, gridInfoDict, inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo):
	returnDict = {
		"success": False,
		"assitCardUidList": [],
		"assistType": "formation",
		"triggerEffectType": "hurt",
		"triggerEffectValues": [0]
	}
	# if card occupied target grid is not enermy, action fails
	cardInfo = uniqueCardDict[gridInfoDict[targetGrid]["cardUid"]]
	# effect is only allowed on oppo side cards
	if cardInfo["avatarId"] != launchAvatarId:
		launchGridRC = getGridRowAndCol(launchGrid)
		targetGridRC = getGridRowAndCol(targetGrid)
		rowOffset = targetGridRC[0] - launchGridRC[0]
		colOffset = targetGridRC[1] - launchGridRC[1]
		direction = [rowOffset, colOffset]
		if direction[0] != 0 and direction[1] != 0:
			# which means target grid is not the lined up to launch grid, which is not permitted
			distance = abs(rowOffset) + abs(colOffset)
			if effectInfo["effectValues"]["distance"] <= distance:
				# target grid is within attack distance, which allows skill launch
				# now we should tell whether formation is formed
				isFormed = True
				assitCardUidList = []
				if colOffset < 0:
					# which means it goes to negative x direction, we should search negative x
					checkLDGrid = getGridNbByRowAndCol(launchGridRC[0] - 1, launchGridRC[1] - 1)
					checkLTGrid = getGridNbByRowAndCol(launchGridRC[0] + 1, launchGridRC[1] - 1)
					if checkLDGrid == -1 or checkLTGrid == -1:
						isFormed = False
					else:
						if gridInfoDict[checkLDGrid]["cardUid"] != "" and gridInfoDict[checkLTGrid]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkLDGrid]["cardUid"])
							assitCardUidList.append(gridInfoDict[checkLTGrid]["cardUid"])
						else:
							isFormed = False
				elif colOffset > 0:
					checkRDGrid = getGridNbByRowAndCol(launchGridRC[0] - 1, launchGridRC[1] + 1)
					checkRTGrid = getGridNbByRowAndCol(launchGridRC[0] + 1, launchGridRC[1] + 1)
					if checkRDGrid == -1 or checkRTGrid == -1:
						isFormed = False
					else:
						if gridInfoDict[checkRDGrid]["cardUid"] != "" and gridInfoDict[checkRTGrid]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkRDGrid]["cardUid"])
							assitCardUidList.append(gridInfoDict[checkRTGrid]["cardUid"])
						else:
							isFormed = False
				elif rowOffset < 0:
					checkLDGrid = getGridNbByRowAndCol(launchGridRC[0] - 1, launchGridRC[1] - 1)
					checkRDGrid = getGridNbByRowAndCol(launchGridRC[0] - 1, launchGridRC[1] + 1)
					if checkLDGrid == -1 or checkRDGrid == -1:
						isFormed = False
					else:
						if gridInfoDict[checkLDGrid]["cardUid"] != "" and gridInfoDict[checkRDGrid]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkLDGrid]["cardUid"])
							assitCardUidList.append(gridInfoDict[checkRDGrid]["cardUid"])
						else:
							isFormed = False
				elif rowOffset > 0:
					checkLTGrid = getGridNbByRowAndCol(launchGridRC[0] + 1, launchGridRC[1] - 1)
					checkRTGrid = getGridNbByRowAndCol(launchGridRC[0] + 1, launchGridRC[1] + 1)
					if checkLTGrid == -1 or checkRTGrid == -1:
						isFormed = False
					else:
						if gridInfoDict[checkLTGrid]["cardUid"] != "" and gridInfoDict[checkRTGrid]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkLTGrid]["cardUid"])
							assitCardUidList.append(gridInfoDict[checkRTGrid]["cardUid"])
						else:
							isFormed = False
				
				if isFormed == True:
					# which means v formation is formed
					hurtDict = calculateCardHp(gridInfoDict[targetGrid]["cardUid"], effectInfo["effectValues"]["values"])
					returnDict["success"] = True
					returnDict["assitCardUidList"] = assitCardUidList
					returnDict["triggerEffectValues"][0] = hurtDict["hpLoss"]
	return returnDict

def LineObstacleSwap(uniqueCardDict, gridInfoDict, inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo):
	returnDict = {
		"success": False,
		"assitCardUidList": [],
		"assistType": "formation",
		"triggerEffectType": "hurt",
		"triggerEffectValues": [0]
	}
	cardInfo = uniqueCardDict[gridInfoDict[targetGrid]["cardUid"]]
	launchGridRC = getGridRowAndCol(launchGrid)
	targetGridRC = getGridRowAndCol(targetGrid)
	rowOffset = targetGridRC[0] - launchGridRC[0]
	colOffset = targetGridRC[1] - launchGridRC[1]
	direction = [rowOffset, colOffset]
	if direction[0] != 0 and direction[1] != 0:
		# which means target grid is not lined up to launch grid, which is not permitted
		distance = abs(rowOffset) + abs(colOffset)
		if effectInfo["effectValues"]["distance"] == 0 or effectInfo["effectValues"]["distance"] <= distance:
			# which means target grid is within effect distance
			isFormed = True
			isObstacle = False
			assitCardUidList = []
			if colOffset < 0:
				# which means it's checking negative x direction
				# go through all grids between launch and target grids, check whether there's only one occupied grid between them
				checkLen = launchGridRC[1] - targetGridRC[1]
				if checkLen < 2:
					isFormed = False
				else:
					assitCardUidList = []
					for i in range(1, checkLen):
						checkGridNb = getGridNbByRowAndCol(targetGridRC[0], targetGridRC[1] + i)
						if gridInfoDict[checkGridNb]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkGridNb]["cardUid"])
							# which means this grid is occupied
							if isObstacle == False:
								isObstacle = True
							else:
								# which means there are over 2 obstacles between target grid and launch grid
								isFormed = False
								break
			elif colOffset > 0:
				# which means it's checking negative x direction
				# go through all grids between launch and target grids, check whether there's only one occupied grid between them
				checkLen = targetGridRC[1] - launchGridRC[1]
				if checkLen < 2:
					isFormed = False
				else:
					assitCardUidList = []
					for i in range(1, checkLen):
						checkGridNb = getGridNbByRowAndCol(launchGridRC[0], launchGridRC[1] + i)
						if gridInfoDict[checkGridNb]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkGridNb]["cardUid"])
							# which means this grid is occupied
							if isObstacle == False:
								isObstacle = True
							else:
								# which means there are over 2 obstacles between target grid and launch grid
								isFormed = False
								break
			elif rowOffset < 0:
				# which means it's checking negative y direction
				# go through all grids between launch and target grids, check whether there's only one occupied grid between them
				checkLen = launchGridRC[0] - targetGridRC[0]
				if checkLen < 2:
					isFormed = False
				else:
					assitCardUidList = []
					for i in range(1, checkLen):
						checkGridNb = getGridNbByRowAndCol(targetGridRC[0] + 1, targetGridRC[1])
						if gridInfoDict[checkGridNb]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkGridNb]["cardUid"])
							# which means this grid is occupied
							if isObstacle == False:
								isObstacle = True
							else:
								# which means there are over 2 obstacles between target grid and launch grid
								isFormed = False
								break
			elif rowOffset > 0:
				# which means it's checking negative y direction
				# go through all grids between launch and target grids, check whether there's only one occupied grid between them
				checkLen = targetGridRC[0] - launchGridRC[0]
				if checkLen < 2:
					isFormed = False
				else:
					assitCardUidList = []
					for i in range(1, checkLen):
						checkGridNb = getGridNbByRowAndCol(launchGridRC[0] + 1, launchGridRC[1])
						if gridInfoDict[checkGridNb]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkGridNb]["cardUid"])
							# which means this grid is occupied
							if isObstacle == False:
								isObstacle = True
							else:
								# which means there are over 2 obstacles between target grid and launch grid
								isFormed = False
								break
			if isFormed == True:
				# swap location between launch grid and target grid
				# and then try to trigger passive effects attached on assit cards(well actually checking cards)
				returnDict["success"] = True
				returnDict["assitCardUidList"] = assitCardUidList
	return returnDict


def HurtRandomOppoFirstRow(uniqueCardDict, gridInfoDict, inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo):
	returnDict = {
		"success": False,
		"assitCardUidList": [],
		"assistType": "",
		"triggerEffectType": "hurt",
		"triggerEffectValues": [0]
	}
	# find out which player launches this effect, so that we can find the oppo one
	avatarIndex = inBattleAvatarList.index(launchAvatarId)
	startGridIndex = 0
	if avatarIndex == 0:
		# which means target avatar index is 1
		startGridIndex = GlobalConst.g_boardHalfRow * GlobalConst.g_boardColumn
	else:
		# which means target avatar index is 0
		startGridIndex = (GlobalConst.g_boardHalfRow - 1) * GlobalConst.g_boardColumn

	candidates = []
	# traverse this row to find out which grid is occupied by adversary
	for i in range(0, GlobalConst.g_boardColumn):
		gridNb = startGridIndex + i
		if gridInfoDict[gridNb]["avatarId"] != launchAvatarId:
			# which means this grid belongs to opponent side
			candidates.append(gridNb)

	if len(candidates) == 0:
		# which means there are no available targets for launching
		returnDict["success"] = True
		return returnDict
	else:
		# then choose candidate randomly
		randomGrid = random.choice(candidates)
		hurtDict = calculateCardHp(uniqueCardDict, gridInfoDict[randomGrid]["cardUid"], effectInfo["effectValues"]["value"])
		returnDict["triggerEffectValues"][0] = hurtDict["hpLoss"]
	returnDict["success"] = True
	return returnDict


effect_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

