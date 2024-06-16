import random
import GlobalConst
from CheckGeoRule import rule_dict

def GetPlayCardActionId(stateList, targetX, targetY):
	sdf

def GetMoveActionId(stateList, x, y, targetX, targetY):
	sdf

def GetLaunchSkillActionId(state_list, x, y, targetX, targetY):
	gridId = 8 * y + x
	actionId = 64 + 64*gridId + 8*targetY + targetX
	return actionId

def GetPossibleMove(stateList, x, y):
	actionIds = []
	stateStrs = stateList[y][x].split('/')
	agility = stateStrs[14]
	yPlus = True
	yNeg = True
	xPlus = True
	xNeg = True
	for i in range(1, agility+1, 1):
		if xPlus == False and xNeg == False and yPlus == False and yNeg == False:
			break
		if (y + i) < 7 and yPlus == True:
			moveGridState = stateList[y + i][x]
			if moveGridState == "--":
				actionId = GetMoveActionId(stateList, x, y, x, y + i)
				actionIds.append(actionId)
			else:
				moveGridStateStrs = moveGridState.split('/')
				if moveGridStateStrs[3] == "Teleport":
					if rule_dict["Teleport"](x, y, x, y + i) == True:
						actionId = GetMoveActionId(stateList, x, y, x, y + i)
						actionIds.append(actionId)
				yPlus = False
		if (y - i) > 0 and yNeg == True:
			moveGridState = stateList[y - i][x]
			if moveGridState == "--":
				actionId = GetMoveActionId(stateList, x, y, x, y - i)
				actionIds.append(actionId)
			else:
				moveGridStateStrs = moveGridState.split('/')
				if moveGridStateStrs[3] == "Teleport":
					if rule_dict["Teleport"](x, y, x, y - i) == True:
						actionId = GetMoveActionId(stateList, x, y, x, y - i)
						actionIds.append(actionId)
				yNeg = False
		if (x + i) > 7 and xPlus == True:
			moveGridState = stateList[y][x + i]
			if moveGridState == "--":
				actionId = GetMoveActionId(stateList, x, y, x + i, y)
				actionIds.append(actionId)
			else:
				moveGridStateStrs = moveGridState.split('/')
				if moveGridStateStrs[3] == "Teleport":
					if rule_dict["Teleport"](x, y, x + i, y) == True:
						actionId = GetMoveActionId(stateList, x, y, x + i, y)
						actionIds.append(actionId)
				xPlus = False
		if (x - i) < 0 and xNeg == True:
			moveGridState = stateList[y][x - i]
			if moveGridState == "--":
				actionId = GetMoveActionId(stateList, x, y, x - i, y)
				actionIds.append(actionId)
			else:
				moveGridStateStrs = moveGridState.split('/')
				if moveGridStateStrs[3] == "Teleport":
					if rule_dict["Teleport"](x, y, x - i, y) == True:
						actionId = GetMoveActionId(stateList, x, y, x - i, y)
						actionIds.append(actionId)
				xNeg = False
	return actionIds


def GetLegalSkillAction(stateList, x, y, targetGeoRule, selfOppoCampFlag, distance):
	return False



def SpawnBulletForward(state_list, x, y, effectInfo):
	return False



def TriangleSeperated(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == '--':
		return False
	else:
		for i in range(2, effectInfo["effectValues"]["distance"] + 1, 1):
			# top left direction
			leftLegality = rule_dict["TriangleSeperated"](state_list, x, y, x - i, y, effectInfo)
			# bottom left direciton
			rightLegality = rule_dict["TriangleSeperated"](state_list, x, y, x + i, y, effectInfo)
			# top right direciton
			downLegality = rule_dict["TriangleSeperated"](state_list, x, y, x, y - i, effectInfo)
			# bottom right direciton
			upLegality = rule_dict["TriangleSeperated"](state_list, x, y, x, y + i, effectInfo)

			if leftLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x - i, y)
				actionIds.append(actionId)
			if rightLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x + i, y)
				actionIds.append(actionId)
			if downLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x, y - i)
				actionIds.append(actionId)
			if upLegality == True:
				actionId = GetLaunchSkillActionId(state_list, x, y, x, y + i)
				actionIds.append(actionId)
		return True, actionIds

def LineObstacleSwap(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == "--":
		return False
	else:
		obstacle = "--"
		for i in range(x + 1, 8, 1):
			if obstacle != "--":
				# which means obstacle already exists
				# search target right now
				if state_list[y][i] != "--":
					actionId = GetLaunchSkillActionId(state_list, x, y, i, y)
					actionIds.append(actionId)
					break
			else:
				if state_list[y][i] != "--":
					obstacle = state_list[y][i]
		
		for i in range(x - 1, -1, -1):
			if obstacle != "--":
				# which means obstacle already exists
				# search target right now
				if state_list[y][i] != "--":
					actionId = GetLaunchSkillActionId(state_list, x, y, i, y)
					actionIds.append(actionId)
					break
			else:
				if state_list[y][i] != "--":
					obstacle = state_list[y][i]

		for i in range(y + 1, 8, 1):
			if obstacle != "--":
				# which means obstacle already exists
				# search target right now
				if state_list[i][x] != "--":
					actionId = GetLaunchSkillActionId(state_list, x, y, x, i)
					actionIds.append(actionId)
					break
			else:
				if state_list[i][i] != "--":
					obstacle = state_list[i][x]

		for i in range(y - 1, -1, -1):
			if obstacle != "--":
				# which means obstacle already exists
				# search target right now
				if state_list[i][x] != "--":
					actionId = GetLaunchSkillActionId(state_list, x, y, x, i)
					actionIds.append(actionId)
					break
			else:
				if state_list[i][i] != "--":
					obstacle = state_list[i][x]
		return True, actionIds


def LinkGrid(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == "--":
		return False, actionIds
	else:
		launchStateStrs = state_list[y][x].split('_')
		launchCamp = launchStateStrs[0]
		search_oppo_flag = effectInfo["effectValues"]["values"][1]
		searched_list = []
		if search_oppo_flag == 1:
			rule_dict["SearchLink"](state_list, x, y, launchCamp, True, searched_list)
		else:
			rule_dict["SearchLink"](state_list, x, y, launchCamp, False, searched_list)
		for grid in searched_list:
			splitStrs = grid.split('_')
			targetY = int(splitStrs[0])
			targetX = int(splitStrs[1])
			actionId = GetLaunchSkillActionId(state_list, x, y, targetX, targetY)
			actionIds.append(actionId)
		return True, actionIds


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
			TLLegality = rule_dict["HornHurtDiagonal"](state_list, x, y, x - i, y + i, effectInfo)
			# bottom left direciton
			BLLegality = rule_dict["HornHurtDiagonal"](state_list, x, y, x - i, y - i, effectInfo)
			# top right direciton
			TRLegality = rule_dict["HornHurtDiagonal"](state_list, x, y, x + i, y + i, effectInfo)
			# bottom right direciton
			BRLegality = rule_dict["HornHurtDiagonal"](state_list, x, y, x + i, y - i, effectInfo)

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


		

actionIdConverterDict = {
}

#result = test_dict['test']('Alice',1,1)

