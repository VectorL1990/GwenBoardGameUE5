import random
import GlobalConst
from CheckGeoRule import geo_rule_dict


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

def Teleport(stateList, x, y, targetX, targetY):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	targetStateStr = stateList[targetY][targetX]
	targetStateStrs = targetStateStr.split('/')
	linkPairNb = targetStateStrs[9]
	# find teleport grid
	for i in range(0, GlobalConst.maxRow, 1):
		for j in range(0, GlobalConst.maxCol, 1):
			if stateList[i][j] != "--" and (i != targetY or j != targetX):
				stateStrs = stateList[i][j].split('/')
				if stateStrs[9] == linkPairNb:
					
					originGridInfo = {
						"grid": [x, y],
						"modifyType": "TeleportLaunch",
						"modifyValues": []
					}
					targetGridInfo = {
						"grid": [targetX, targetY],
						"modifyType": "TeleportTarget",
						"modifyValues": []
					}
					returnDict["success"] = True
					returnDict["modifyGrids"].append(originGridInfo)
					returnDict["modifyGrids"].append(targetGridInfo)
					offsetX = targetX - x
					offsetY = targetY - y
					teleportX = j - offsetX
					teleportY = i - offsetY
					stateList[teleportY][teleportX] = stateList[y][x]
					stateList[y][x] = "--"
					break
	return returnDict


def SpawnBulletForward(state_list, x, y, effectInfo):
	sdf

def DevourNextSpawnFar(state_list, launchX, launchY, targetX, targetY, effectInfo, latestGenerateId):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	legality = geo_rule_dict["DevourNextSpawnFar"](state_list, launchX, launchY, targetX, targetY, effectInfo)
	if legality == True:
		launchStateStr = state_list[launchY][launchX]
		launchStateStrs = launchStateStr.split("/")
		# devour target card
		state_list[targetY][targetX] = "--"
		devourGridInfo = {
			"grid": [targetX, targetY],
			"modifyType": "devour",
			"modifyValues": []
		}
		returnDict["modifyGrids"].append(devourGridInfo)
		
		spawnDistance = effectInfo["effectValues"]["distance"]
		spawnX = launchX
		if launchStateStrs[0] == 0:
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
		


def FormationVShoot(stateList, launchX, launchY, targetX, targetY, effectInfo, latestGenerateId):
	returnDict = {
		"success": False,
		"modifyGrids": []
	}
	legality = geo_rule_dict["FormationVShoot"](stateList, launchX, launchY, targetX, targetY, effectInfo)
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
	legality = geo_rule_dict["LineObstacleSwap"](stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId)
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
		legality = geo_rule_dict["checkConnect"](stateList, launchX, launchY, launchCamp, True, searchedList)
	else:
		legality = geo_rule_dict["checkConnect"](stateList, launchX, launchY, launchCamp, False, searchedList)
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
			TLLegality = geo_rule_dict["HornHurtDiagonal"](state_list, x, y, x - i, y + i, effectInfo)
			# bottom left direciton
			BLLegality = geo_rule_dict["HornHurtDiagonal"](state_list, x, y, x - i, y - i, effectInfo)
			# top right direciton
			TRLegality = geo_rule_dict["HornHurtDiagonal"](state_list, x, y, x + i, y + i, effectInfo)
			# bottom right direciton
			BRLegality = geo_rule_dict["HornHurtDiagonal"](state_list, x, y, x + i, y - i, effectInfo)

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


		

action_collection_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

