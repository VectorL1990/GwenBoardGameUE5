
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
	# return dead or not
	if hurt <= uniqueCardDict[cardUid]["defence"]:
		uniqueCardDict[cardUid]["defence"] -= hurt
		return False
	else:
		overflow = hurt - uniqueCardDict[cardUid]["defence"]
		if overflow < uniqueCardDict[cardUid]["hp"]:
			uniqueCardDict[cardUid]["hp"] -= overflow
			return False
		else:
			uniqueCardDict[cardUid]["hp"] = 0
			return True


def FormationVShoot(uniqueCardDict, gridDictInfo, launchAvatarId, targetGrid, launchGrid, effectInfo):
	returnDict = {
		"success": False,
		"assitCardUidList": []
	}
	# if card occupied target grid is not enermy, action fails
	cardInfo = uniqueCardDict[gridDictInfo[targetGrid]["cardUid"]]
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
						if gridDictInfo[checkLDGrid]["cardUid"] != "" and gridDictInfo[checkLTGrid]["cardUid"] != "":
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
						if gridDictInfo[checkRDGrid]["cardUid"] != "" and gridDictInfo[checkRTGrid]["cardUid"] != "":
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
						if gridDictInfo[checkLDGrid]["cardUid"] != "" and gridDictInfo[checkRDGrid]["cardUid"] != "":
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
						if gridDictInfo[checkLTGrid]["cardUid"] != "" and gridDictInfo[checkRTGrid]["cardUid"] != "":
							assitCardUidList.append(gridInfoDict[checkLTGrid]["cardUid"])
							assitCardUidList.append(gridInfoDict[checkRTGrid]["cardUid"])
						else:
							isFormed = False
				
				if isFormed == True:
					# which means v formation is formed
					self.calculateCardHp(gridDictInfo[targetGrid]["cardUid"], effectInfo["effectValues"]["values"])
					returnDict["success"] = True
					returnDict["assitCardUidList"] = assitCardUidList
	return returnDict

def LineObstacleSwap(uniqueCardDict, gridInfoDict, launchAvatarId, targetGrid, launchGrid, effectInfo):
	returnDict = {
		"success": False,
		"assitCardUidList": []
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
		if effectInfo["effectValues"]["distance"] == 0 or effectInfo["effectValues"]["distance"] <= distance
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



effect_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap
}

#result = test_dict['test']('Alice',1,1)

