import random
import GlobalConst

def FormationVShoot(state_list, x, y, targetX, targetY, effectInfo):
	actionIds = []
	if state_list[y][x] == '--':
		return False
	else:
		for i in range(2, effectInfo["effectValues"]["distance"] + 1, 1):
			# top left direction
			leftLegality = rule_dict["FormationVShoot"](state_list, x, y, x - i, y, effectInfo)
			# bottom left direciton
			rightLegality = rule_dict["FormationVShoot"](state_list, x, y, x + i, y, effectInfo)
			# top right direciton
			downLegality = rule_dict["FormationVShoot"](state_list, x, y, x, y - i, effectInfo)
			# bottom right direciton
			upLegality = rule_dict["FormationVShoot"](state_list, x, y, x, y + i, effectInfo)

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

	returnDict = {
		"success": False,
		"triggerEffectType": "hurt",
		"triggerEffectValues": [0],
		"modifyGrids": []
	}
	# if card occupied target grid is not enermy, action fails
	launchState = state_list[y][x]
	launchStateStrs = launchState.split('/')
	targetState = state_list[targetY][targetX]
	targetStateStrs = targetState.split('/')
	if targetStateStrs[1] != launchStateStrs[1]:
		legality = rule_dict["FormationVShoot"](state_list, x, y, targetX, targetY, effectInfo)
		if legality == True:
			hurtDict = calculateCardHp(gridInfoDict[targetGrid]["cardUid"], effectInfo["effectValues"]["values"])
			returnDict["success"] = True
			returnDict["triggerEffectValues"][0] = hurtDict["hpLoss"]
			returnDict["modifyGrids"].append([targetX, targetY])
	return returnDict

