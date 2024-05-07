import random
import GlobalConst
from CheckGeoRule import geoRuleDict
from GetAffix import getAffixDict


def Hurt(stateList, targetX, targetY, hurtVal):
	sdf

def GetLaunchSkillActionId(state_list, x, y, targetX, targetY):
	gridId = 8 * y + x
	actionId = 64 + 64*gridId + 8*targetY + targetX
	return actionId

def Move(stateList, x, y, targetX, targetY):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	returnDict["success"] = True
	originGridInfo = {
		"grid": [x, y],
		"modifyType": "MoveLaunch",
		"modifyValues": []
	}
	targetGridInfo = {
		"grid": [targetX, targetY],
		"modifyType": "MoveTarget",
		"modifyValues": []
	}
	returnDict["modifyGrids"].append(originGridInfo)
	returnDict["modifyGrids"].append(targetGridInfo)
	stateList[targetY][targetX] = stateList[y][x]
	stateList[y][x] = "--"
	return returnDict

def IncreaseDefence(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "increaseDefence",
		"modifyGrids": []
	}
	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]
	
	modifyGrids = []
	if effectInfo["aoeType"] == "H3":
		if targetX > 0 and stateList[targetY][targetX - 1] != "--":
			modifyGrids.append([targetX - 1, targetY])
		if targetX < GlobalConst.maxCol and stateList[targetY][targetX + 1] != "--":
			modifyGrids.append([targetX + 1, targetY])
	elif effectInfo["aoeType"] == "V3":
		if targetY > 0 and stateList[targetY - 1][targetX] != "--":
			targetDownStateStrs = stateList[targetY - 1][targetX1].split('/')
			targetDownStateStrs[19] = targetDownStateStrs[19] + effectValue
			combineStr = '/'.join(targetDownStateStrs)
			stateList[targetY - 1][targetX] = combineStr
			returnDict["modifyGrids"].append([targetX, targetY - 1])
		if targetY < GlobalConst.maxRow and stateList[targetY + 1][targetX] != "--":
			targetUpStateStrs = stateList[targetY + 1][targetX].split('/')
			targetUpStateStrs[19] = targetUpStateStrs[19] + effectValue
			combineStr = '/'.join(targetUpStateStrs)
			stateList[targetY + 1][targetX] = combineStr
			returnDict["modifyGrids"].append([targetX, targetY + 1])
	elif effectInfo["aoeType"] == "sweep":
		xOffset = targetX - x
		yOffset = targetY - y
		if xOffset > 0:
			# goes right
			if targetX + 1 < GlobalConst.maxCol and stateList[targetY][targetX + 1] != "--":
				targetRightStateStrs = stateList[targetY][targetX + 1].split('/')
				targetRightStateStrs[19] = targetRightStateStrs[19] + effectValue
				combineStr = '/'.join(targetRightStateStrs)
				stateList[targetY][targetX + 1] = combineStr
				returnDict["modifyGrids"].append([targetX + 1, targetY])
			if targetX + 2 < GlobalConst.maxCol and stateList[targetY][targetX + 2] != "--":
				targetRightStateStrs = stateList[targetY][targetX + 2].split('/')
				targetRightStateStrs[19] = targetRightStateStrs[19] + effectValue
				combineStr = '/'.join(targetRightStateStrs)
				stateList[targetY][targetX + 2] = combineStr
				returnDict["modifyGrids"].append([targetX + 2, targetY])
		elif xOffset < 0:
			# geos left
		elif yOffset > 0:
			# goes up
		elif yOffset < 0:
			# goes down
	elif effectInfo["aoeType"] == "normalCross":
		sdf
	elif effectInfo["aoeType"] == "obliqueCross":
		sdf

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		targetStateStrs[19] = targetStateStrs[19] + effectValue
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict

def ReplaceDefence(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "replaceDefence",
		"modifyGrids": []
	}
	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]
	targetStateStrs = stateList[targetY][targetX].split('/')
	targetStateStrs[19] = effectValue
	combineStr = '/'.join(targetStateStrs)
	stateList[targetY][targetX] = combineStr
	returnDict["modifyGrids"].append([targetX, targetY])
	return returnDict

def IncreaseSelfDefence(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "replaceDefence",
		"modifyGrids": []
	}
	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]
	targetStateStrs = stateList[targetY][targetX].split('/')
	targetStateStrs[19] = effectValue
	combineStr = '/'.join(targetStateStrs)
	stateList[targetY][targetX] = combineStr
	returnDict["modifyGrids"].append([targetX, targetY])
	return returnDict

def Devour(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "devour",
		"modifyGrids": []
	}
	
	legality = geoRuleDict["DevourNextSpawnFar"](stateList, launchX, launchY, targetX, targetY, effectInfo["selfOrOppoCamp"], effectInfo["distance"])
	if legality == True:
		launchStateStr = stateList[launchY][launchX]
		launchStateStrs = launchStateStr.split("/")
		# devour target card
		stateList[targetY][targetX] = "--"
		devourGridInfo = {
			"grid": [targetX, targetY],
			"modifyType": "devour",
			"modifyValues": []
		}
		returnDict["modifyGrids"].append(devourGridInfo)
		
		spawnDistance = effectInfo["effectValues"]["distance"]
		spawnX = launchX
		if launchStateStrs[2] == 0:
			# which means spawn upward
			spawnY = launchY + spawnDistance
		else:
			spawnY = launchY - spawnDistance

		# get spawn card info and generate new card id
		spawnGridInfo = {
			"grid": [spawnX, spawnY],
			"modifyType": "spawn",
			"modifyValues": []
		}
		returnDict["modifyGrids"].append(spawnGridInfo)
		returnDict["success"] = True
	
	return returnDict


def SpawnBulletForward(state_list, x, y, effectInfo):
	sdf


		


def FormationVShoot(stateList, launchX, launchY, targetX, targetY, effectInfo, latestGenerateId):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	legality = geoRuleDict["FormationVShoot"](stateList, launchX, launchY, targetX, targetY, effectInfo)
	if legality == True:
		hurtVal = effectInfo["effectValues"]["values"][0]
		# modify hp of this grid 
		Hurt(stateList, targetX, targetY, hurtVal)
		hurtGridInfo = {
			"grid": [targetX, targetY],
			"modifyType": "Hurt",
			"modifyValues": []
		}
		returnDict["modifyGrids"].append(hurtGridInfo)

	return returnDict



def LineObstacleSwap(stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	legality = geoRuleDict["LineObstacleSwap"](stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId)
	if legality == True:
		tempTargetState = stateList[targetY][targetX]
		stateList[targetY][targetX] = stateList[launchY][launchX]
		stateList[launchY][launchX] = tempTargetState
		returnDict["success"] = True
		swapLaunchGridInfo = {
			"grid": [launchX, launchY],
			"modifyType": "Swap",
			"modifyValues": [targetX, targetY]
		}
		swapTargetGridInfo = {
			"grid": [targetX, targetY],
			"modifyType": "Swap",
			"modifyValues": [launchX, launchY]
		}
		returnDict["modifyGrids"].append(swapLaunchGridInfo)
		returnDict["modifyGrids"].append(swapTargetGridInfo)
	
	return returnDict


def LinkGrid(stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	launchStateStrs = stateList[launchY][launchX].split('/')
	launchCamp = launchStateStrs[0]
	searchOppoFlag = effectInfo["effectValues"]["values"][1]
	searchedList = []
	legality = False
	if searchOppoFlag == 1:
		legality = geoRuleDict["CheckConnect"](stateList, launchX, launchY, launchCamp, True, searchedList)
	else:
		legality = geoRuleDict["CheckConnect"](stateList, launchX, launchY, launchCamp, False, searchedList)
	if legality == True:
		# which means target grid is linked with grid launching
		if effectInfo["effectValues"]["values"][0] == "0":
			# which means launched grid hurts target grid
			Hurt(stateList, targetX, targetY, effectInfo["effectValues"]["values"][2])
		else:
			sdf


def HurtRandomOppoFirstRow(state_list, x, y, effectInfo):
	if state_list[y][x] == "--":
		return False
	else:
		launchStateStrs = state_list[y][x].split('/')
		camp = launchStateStrs[0]
		candidates = []
		for i in range(0, 8):
			if camp == 0:
				if state_list[3][i] != "--":
					targetStateStrs = state_list[3][i]
					if targetStateStrs[0] != camp:
						candidates.append()


def HornHurtDiagonal(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == '--':
		return False, actionIds
	else:
		for i in range(1, effectInfo["effectValues"]["distance"] + 1, 1):
			# top left direction
			TLLegality = geoRuleDict["HornHurtDiagonal"](state_list, x, y, x - i, y + i, effectInfo)
			# bottom left direciton
			BLLegality = geoRuleDict["HornHurtDiagonal"](state_list, x, y, x - i, y - i, effectInfo)
			# top right direciton
			TRLegality = geoRuleDict["HornHurtDiagonal"](state_list, x, y, x + i, y + i, effectInfo)
			# bottom right direciton
			BRLegality = geoRuleDict["HornHurtDiagonal"](state_list, x, y, x + i, y - i, effectInfo)

			if TLLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x - i, y + i)
				actionIds.append(actionId)
			if BLLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x - i, y - i)
				actionIds.append(actionId)
			if TRLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x + i, y + i)
				actionIds.append(actionId)
			if BRLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x + i, y - i)
				actionIds.append(actionId)
		return True, actionIds


		

launchSkillActionDict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

