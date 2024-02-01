import random
import GlobalConst
from check_geo_rule import geo_rule_dict


def GetLaunchSkillActionId(state_list, x, y, targetX, targetY):
	gridId = 8 * y + x
	actionId = 64 + 64*gridId + 8*targetY + targetX
	return actionId

def SpawnBulletForward(state_list, x, y, effectInfo):
	sdf

def DevourNextSpawnFar(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == "--":
		return False
	else:
		upLegality = geo_rule_dict["DevourNextSpawnFar"](state_list, x, y, x, y + 1, effectInfo)
		downLegality = geo_rule_dict["DevourNextSpawnFar"](state_list, x, y, x, y - 1, effectInfo)
		rightLegality = geo_rule_dict["DevourNextSpawnFar"](state_list, x, y, x + 1, y, effectInfo)
		leftLegality = geo_rule_dict["DevourNextSpawnFar"](state_list, x, y, x - 1, y, effectInfo)
		if upLegality == True:
			actionId = GetLaunchSkillActionId(state_list, x, y, x, y + 1)
			actionIds.append(actionId)
		if downLegality == True:
			actionId = GetLaunchSkillActionId(state_list, x, y, x, y - 1)
			actionIds.append(actionId)
		if rightLegality == True:
			actionId = GetLaunchSkillActionId(state_list, x, y, x + 1, y)
			actionIds.append(actionId)
		if leftLegality == True:
			actionId = GetLaunchSkillActionId(state_list, x, y, x - 1, y)
			actionIds.append(actionId)
		return True, actionIds

def FormationVShoot(state_list, x, y, effectInfo):
	actionIds = []
	if state_list[y][x] == '--':
		return False
	else:
		for i in range(2, effectInfo["effectValues"]["distance"] + 1, 1):
			# top left direction
			leftLegality = geo_rule_dict["FormationVShoot"](state_list, x, y, x - i, y, effectInfo)
			# bottom left direciton
			rightLegality = geo_rule_dict["FormationVShoot"](state_list, x, y, x + i, y, effectInfo)
			# top right direciton
			downLegality = geo_rule_dict["FormationVShoot"](state_list, x, y, x, y - i, effectInfo)
			# bottom right direciton
			upLegality = geo_rule_dict["FormationVShoot"](state_list, x, y, x, y + i, effectInfo)

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
			geo_rule_dict["SearchLink"](state_list, x, y, launchCamp, True, searched_list)
		else:
			geo_rule_dict["SearchLink"](state_list, x, y, launchCamp, False, searched_list)
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

