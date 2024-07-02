import random
import GlobalConst
from CheckGeoRule import geoRuleDict
from GetAffix import getAffixDict



def Hurt(stateList, targetX, targetY, hurtVal):
	targetStateStrs = stateList[targetY][targetX].split('/')
	resultHp = int(targetStateStrs[17]) - hurtVal
	if resultHp < 0:
		resultHp = 0
	return resultHp

def LaunchLinkState(linkStates, launchLinkUid, targetLinkUid, linkRound, linkType):
	'''
	link = 
		{
			"linkType": "hurtLink",
			"targets":
			{
				"card_1":2,
				"card_2":3
			}
		}
	'''
	updateNewLink = False
	for linkKey, linkInfo in linkStates.items():
		if targetLinkUid in linkInfo["targets"]:
			if linkKey == launchLinkUid:
				linkInfo["targets"][targetLinkUid] = linkRound
				updateNewLink = True
				break
			else:
				# remove targetUid from original link pair first
				# then add it to new link pair
				linkInfo["targets"].pop(targetLinkUid)
				if launchLinkUid in linkStates:
					linkStates[launchLinkUid]["targets"][targetLinkUid] = linkRound
				else:
					newLinkInfo = {"linkType": linkType, "targets": {targetLinkUid: linkRound}}
					linkStates[launchLinkUid] = newLinkInfo
				updateNewLink = True
				break
	
	if updateNewLink == False:
		if launchLinkUid in linkStates:
			linkStates[launchLinkUid]["targets"][targetLinkUid] = linkRound
		else:
			newLinkInfo = {"linkType": linkType, "targets": {targetLinkUid: linkRound}}
			linkStates[launchLinkUid] = newLinkInfo



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

def GetAoeTargetGrids(stateList, x, y, targetX, targetY, aoeType, targetCamp):
	modifyGrids = []
	launchGridStateStrs = stateList[y][x].split('/')
	launchGridCamp = launchGridStateStrs[2]
	if aoeType == "H3":
		if targetX > 0 and stateList[targetY][targetX - 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX - 1, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX - 1, targetY])
			else:
				modifyGrids.append([targetX - 1, targetY])
		if targetX < GlobalConst.maxCol - 1 and stateList[targetY][targetX + 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX + 1, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX + 1, targetY])
			else:
				modifyGrids.append([targetX + 1, targetY])
		if stateList[targetY][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY])
			else:
				modifyGrids.append([targetX, targetY])
	elif aoeType == "V3":
		if targetY > 0 and stateList[targetY - 1][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY - 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY - 1])
			else:
				modifyGrids.append([targetX, targetY - 1])
		if targetY < GlobalConst.maxRow - 1 and stateList[targetY + 1][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY + 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY + 1])
			else:
				modifyGrids.append([targetX, targetY + 1])
		if stateList[targetY][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY])
			else:
				modifyGrids.append([targetX, targetY])
	elif aoeType == "sweep":
		xOffset = targetX - x
		yOffset = targetY - y
		if xOffset > 0:
			if targetX + 1 < GlobalConst.maxCol and stateList[targetY][targetX + 1] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX + 1, targetY])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX + 1, targetY])
				else:
					modifyGrids.append([targetX + 1, targetY])
			if targetX + 2 < GlobalConst.maxCol and stateList[targetY][targetX + 2] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY][targetX + 2].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX + 2, targetY])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY][targetX + 2].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX + 2, targetY])
				else:
					modifyGrids.append([targetX + 2, targetY])
		elif xOffset < 0:
			if targetX - 1 >= 0 and stateList[targetY][targetX - 1] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX - 1, targetY])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX - 1, targetY])
				else:
					modifyGrids.append([targetX - 1, targetY])
			if targetX - 2 >= 0 and stateList[targetY][targetX - 2] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY][targetX - 2].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX - 2, targetY])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY][targetX - 2].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX - 2, targetY])
				else:
					modifyGrids.append([targetX - 2, targetY])
		elif yOffset < 0:
			if targetY - 1 >= 0 and stateList[targetY - 1][targetX] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX, targetY - 1])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX, targetY - 1])
				else:
					modifyGrids.append([targetX, targetY - 1])
			if targetY - 2 >= 0 and stateList[targetY - 2][targetX] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY - 2][targetX].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX, targetY - 2])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY - 2][targetX].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX, targetY - 2])
				else:
					modifyGrids.append([targetX, targetY - 2])
		elif yOffset > 0:
			if targetY + 1 < GlobalConst.maxRow and stateList[targetY + 1][targetX] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX, targetY + 1])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX, targetY + 1])
				else:
					modifyGrids.append([targetX, targetY + 1])
			if targetY + 2 < GlobalConst.maxRow and stateList[targetY + 2][targetX] != "--":
				if targetCamp == "self":
					targetGridStateStrs = stateList[targetY + 2][targetX].split('/')
					if targetGridStateStrs[2] == launchGridCamp:
						modifyGrids.append([targetX, targetY + 2])
				elif targetCamp == "oppo":
					targetGridStateStrs = stateList[targetY + 2][targetX].split('/')
					if targetGridStateStrs[2] != launchGridCamp:
						modifyGrids.append([targetX, targetY + 2])
				else:
					modifyGrids.append([targetX, targetY + 2])
	elif aoeType == "normalCross":
		# left
		if targetX > 0 and stateList[targetY][targetX - 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX - 1, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX - 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX - 1, targetY])
			else:
				modifyGrids.append([targetX - 1, targetY])
		# right
		if targetX < GlobalConst.maxCol - 1 and stateList[targetY][targetX + 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX + 1, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX + 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX + 1, targetY])
			else:
				modifyGrids.append([targetX + 1, targetY])
		# down
		if targetY > 0 and stateList[targetY - 1][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY - 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY - 1][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY - 1])
			else:
				modifyGrids.append([targetX, targetY - 1])
		# up
		if targetY < GlobalConst.maxRow - 1 and stateList[targetY + 1][targetX] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY + 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY + 1][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY + 1])
			else:
				modifyGrids.append([targetX, targetY + 1])
	elif aoeType == "obliqueCross":
		if targetX - 1 >= 0 and targetY - 1 >= 0 and stateList[targetY - 1][targetX - 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY - 1][targetX - 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX - 1, targetY - 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY - 1][targetX - 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX - 1, targetY - 1])
			else:
				modifyGrids.append([targetX - 1, targetY - 1])
		if targetX + 1 < GlobalConst.maxCol and targetY - 1 >= 0 and stateList[targetY - 1][targetX + 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY - 1][targetX + 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX - 1, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY - 1][targetX + 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX + 1, targetY - 1])
			else:
				modifyGrids.append([targetX + 1, targetY - 1])
		if targetX + 1 < GlobalConst.maxCol and targetY + 1 < GlobalConst.maxRow and stateList[targetY + 1][targetX + 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY + 1][targetX + 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX + 1, targetY + 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY + 1][targetX + 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX + 1, targetY + 1])
			else:
				modifyGrids.append([targetX + 1, targetY + 1])
		if targetX - 1 >= 0 and targetY + 1 < GlobalConst.maxRow and stateList[targetY + 1][targetX - 1] != "--":
			if targetCamp == "self":
				targetGridStateStrs = stateList[targetY + 1][targetX - 1].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX - 1, targetY + 1])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY + 1][targetX - 1].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX - 1, targetY + 1])
			else:
				modifyGrids.append([targetX - 1, targetY + 1])
	else:
		if targetCamp == "self":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] == launchGridCamp:
					modifyGrids.append([targetX, targetY])
			elif targetCamp == "oppo":
				targetGridStateStrs = stateList[targetY][targetX].split('/')
				if targetGridStateStrs[2] != launchGridCamp:
					modifyGrids.append([targetX, targetY])
			else:
				modifyGrids.append([targetX, targetY])
	return modifyGrids


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
	
	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		resultValue = int(targetStateStrs[19]) + effectValue
		targetStateStrs[19] = str(resultValue)
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

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		targetStateStrs[19] = str(effectValue)
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
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

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = int(stateList[grid[1]][grid[0]].split('/'))
		resultValue = int(targetStateStrs[19]) + effectValue
		targetStateStrs[19] = str(resultValue)
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict


def GiveTempArmor(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "giveTempArmor",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		if "armor" in targetStateStrs[24]:
			tags = targetStateStrs[24].split('&')
			for tag in tags:
				if "armor" in tag:
					strs = tag.split('=')
					strs[1] = str(effectValue)
					tag = '='.join(strs)
					combineTags = '&'.join(tags)
					targetStateStrs[24] = combineTags
					stateList[grid[1]][grid[0]] = '/'.join(targetStateStrs)
					break
		else:
			targetStateStrs.append("armor=" + str(effectValue))
			stateList[grid[1]][grid[0]] = '/'.join(targetStateStrs)
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict

def TempArmor(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "tempArmor",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		if "armor" in targetStateStrs[24]:
			tags = targetStateStrs[24].split('&')
			for tag in tags:
				if "armor" in tag:
					strs = tag.split('=')
					strs[1] = str(effectValue)
					tag = '='.join(strs)
					combineTags = '&'.join(tags)
					targetStateStrs[24] = combineTags
					stateList[grid[1]][grid[0]] = '/'.join(targetStateStrs)
					break
		else:
			targetStateStrs.append("armor=" + str(effectValue))
			stateList[grid[1]][grid[0]] = '/'.join(targetStateStrs)
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict

def DefenceHurt(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "defenceHurt",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])
	launchStateStrs = stateList[y][x].split('/')
	launchDefence = int(launchStateStrs[19])
	hurtValue = launchDefence / effectValue

	for grid in modifyGrids:
		resultHp = Hurt(stateList, grid[0], grid[1], hurtValue)
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		targetStateStrs[17] = str(resultHp)
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict

def SwitchCamp(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "switchCamp",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])
	
	targetCamp = ""
	launchStateStrs = stateList[y][x].split('/')
	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		targetCamp = targetStateStrs[2]
		targetStateStrs[2] = launchStateStrs[2]
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	
	launchStateStrs[2] = targetCamp
	launchCampCombineStr = '/'.join(launchStateStrs)
	returnDict["modifyGrids"].append([x, y])
	return returnDict

def Capture(stateList, x, y, targetX, targetY, effectInfo):
	returnDict = {
		"success": False,
		"modifyType": "campture",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])
	
	launchStateStrs = stateList[y][x].split('/')
	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		targetStateStrs[2] = launchStateStrs[2]
		combineStr = '/'.join(targetStateStrs)
		stateList[grid[1]][grid[0]] = combineStr
		returnDict["modifyGrids"].append([grid[0], grid[1]])
	return returnDict

def TempHurtLink(stateList, x, y, targetX, targetY, effectInfo, linkState):
	returnDict = {
		"success": False,
		"modifyType": "campture",
		"modifyGrids": []
	}

	effectValue = 0
	if effectInfo["affixType"] != "none":
		affix = effectInfo["affixType"]
		effectValue = getAffixDict[affix](stateList, x, y, targetX, targetY)
	else:
		effectValue = effectInfo["values"]

	modifyGrids = GetAoeTargetGrids(stateList, x, y, targetX, targetY, effectInfo["aoeType"], effectInfo["targetCamp"])

	for grid in modifyGrids:
		targetStateStrs = stateList[grid[1]][grid[0]].split('/')
		if targetStateStrs[16]


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

