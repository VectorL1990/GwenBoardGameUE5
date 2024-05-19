import random
import GlobalConst

def PassiveTriggerHurtRight(stateList, launchX, launchY, triggerX, triggerY, effectInfo, triggeredUids):
	returnDict = {
		"modifyGrids": [],
		"modifyType": [],
		"modifyValues": [],
		"launchX": launchX,
		"launchY": launchY
	}
	return returnDict

def HurtLink(stateList, linkSrcX, linkSrcY, triggerValue, effectInfo, linkStates):
	returnDict = {
		"modifyType": "HurtLink",
		"modifyGrids": []
	}

	linkSrcGridInfos = stateList[linkSrcY][linkSrcX].split('/')
	linkSrcGridUid = linkSrcGridInfos[0]
	if linkSrcGridUid in linkStates:
		for linkTargetKey, linkTargetInfo in linkStates[linkSrcGridUid]["targets"].items():
			linkTargetX, linkTargetY = stateList.FindGridByUid(linkTargetKey)
			hurtLinkValue = triggerValue / effectInfo["values"][0]
			returnDict["modifyGrids"].append([linkTargetX, linkTargetY])
			Hurt(stateList, )
	return returnDict

passiveEffectDict = {
	"PassiveTriggerHurtRight": PassiveTriggerHurtRight,
}
