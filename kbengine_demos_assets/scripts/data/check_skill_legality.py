import random
import GlobalConst

def FormationVShoot(state_list, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"triggerEffectType": "hurt",
		"triggerEffectValues": [0],
		"modifyGrids": []
	}
	if state_list[y][x] == '--':
		return returnDict
	else:
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

