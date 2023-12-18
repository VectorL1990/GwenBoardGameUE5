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


def TagByHurt(uniqueCardDict, gridInfoDict, inBattleAvatarList, launchAvatarId, triggerGrid, launchGrid, triggerEffectInfo, effectInfo):
	passiveReturnDict = {
		"success": False,
		"assitCardUidList": [],
		"assistType": "",
		"triggerEffectType": "",
		"triggerEffectValues": []
    }
	if triggerEffectInfo["triggerEffectValues"][0] >= effectInfo["prereqs"]["triggerValue"]:
		passiveReturnDict["success"] = True
		# which means passive effect is activated
		if effectInfo["effectValues"]["tag"] not in uniqueCardDict[gridInfoDict[launchGrid]["cardUid"]]:
			# which means this card doesn't contain tag being added
			uniqueCardDict[gridInfoDict[launchGrid]["cardUid"]]["stateTags"].append(effectInfo["effectValues"]["tag"])
	return passiveReturnDict
		

passive_effect_dict = {
	'FormationVShoot': FormationVShoot,
	'LineObstacleSwap': LineObstacleSwap,
	'HurtRandomOppoFirstRow': HurtRandomOppoFirstRow
}

#result = test_dict['test']('Alice',1,1)

