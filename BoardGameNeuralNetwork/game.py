import numpy as np
import copy
import time
from config import CONFIG
from collections import deque
import random


class Game(object):
	def __init__(self):
		self.state_list = [['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--']]

	def selfPlay(self):
		actionCount = 0
		while True:
			actionCount += 1






def machineReqPlayCardAction(self, avatarEntityCall, avatarClientActionSequence, cardUid, gridNb):
	actionSequenceLatency = self.curActionSequence - avatarClientActionSequence
	if actionSequenceLatency == 1:
		# which means there's no information lost on client side
		# find corresponding card info from card dictionary, including effects attached to this card
		for k,v in self.uniqueCardDict[cardUid]["effects"].items():
			if v["launchType"] == "auto":
				self.launchEffect(avatarEntityCall.id, avatarClientActionSequence, -1, gridNb, k, v)


def machineReqLaunchCardSkillAction(self, avatarEntityCall, clientActionSequence, cardUid, skillName, launchGridNb, targetGridNb):
	actionSequenceLatency = self.curActionSequence - clientActionSequence
	if actionSequenceLatency == 1:
		# which means there's no information lost on client side
		if skillName in self.uniqueCardDict[cardUid]["effects"]:
			if self.uniqueCardDict[cardUid]["effects"][skillName]["availableTimes"] >= 1:
				# which means this skill is still available to launch
				self.launchEffect(avatarEntityCall.id, clientActionSequence, targetGridNb, launchGridNb, skillName, self.uniqueCardDict[cardUid]["effects"][skillName])


def launchEffect(self, clientActionSequence, launchAvatarId, targetGrid, launchGrid, effectName, effectInfo):
	if effectName in effect_dict:
		resultDict = effect_dict[effectName](self.uniqueCardDict, self.gridInfoDict, self.inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo)
		# if launch effect succesfully, settlement has been done
		# broadcast latest operation result to all clients
		if resultDict["success"] == True:
			modifyGridIds = resultDict["modifyGrids"]
			modifyCardUids = resultDict["modifyCardUids"]
			# traverse all target cards to trigger their passive effects
			for targetUid in resultDict["targetUids"]:
				for targetPassiveEffectKey,targetPassiveEffectValue in self.uniqueCardDict[targetUid]["effects"]:
					if targetPassiveEffectValue["launchType"] == "targetPassive":
						if targetPassiveEffectValue["prereqs"]["triggerEffectType"] == resultDict["triggerEffectType"]:
							if targetPassiveEffectKey in passive_effect_dict:
								passiveEffectResultDict = passive_effect_dict[targetPassiveEffectKey]
								(
									self.uniqueCardDict, 
									self.gridInfoDict, 
									self.inBattleAvatarList, 
									self.gridInfoDict[targetGrid]["avatarId"], 
									launchGrid,
									effectInfo,
									targetPassiveEffectValue
								)
								if passiveEffectResultDict["success"] == True:
									targetPassiveEffectValue["effectValues"]["isRoundEnd"] = True
									for passiveModifyGrid in passiveEffectResultDict["modifyGrids"]:
										if passiveModifyGrid not in modifyGridIds:
											modifyGridIds.append(passiveModifyGrid)
									for passiveModifyCardUid in passiveEffectResultDict["modifyCardUids"]:
										if passiveModifyCardUid not in modifyCardUids:
											modifyCardUids.append(passiveModifyCardUid)
			# traverse all assistant cards to trigger their passive effects
			for assistCardUid in resultDict["assitCardUidList"]:
				# traverse all effects attached to assist card
				for assistPassiveEffectKey,assistPassiveEffectValue in self.uniqueCardDict[assistCardUid]["effects"]:
					if assistPassiveEffectValue["launchType"] == "assitPassive":
						# check whether this effect can be triggered
						if assistPassiveEffectValue["prereqs"]["triggerEffectType"] == resultDict["triggerEffectType"]:
							if assistPassiveEffectKey in passive_effect_dict:
								if assistPassiveEffectKey in passive_effect_dict:
									assistEffectResultDict = passive_effect_dict[assistPassiveEffectKey]
									(
										self.uniqueCardDict,
										self.gridInfoDict,
										self.inBattleAvatarList,
										self.gridInfoDict[targetGrid]["avatarId"],
										launchGrid,
										effectInfo,
										assistPassiveEffectValue
									)
									if assistEffectResultDict["success"] == True:
										assistEffectResultDict["effectValues"]["isRoundEnd"] = True
									for assistModifyGrid in assistEffectResultDict["modifyGrids"]:
										if assistModifyGrid not in modifyGridIds:
											modifyGridIds.append(assistModifyGrid)
									for assistModifyCardUid in assistEffectResultDict["modifyCardUids"]:
										if assistModifyCardUid not in modifyCardUids:
											modifyCardUids.append(assistModifyCardUid)
			# assemble all modification and notify all clients
			syncModifyGridInfos = []
			syncModifyCardInfos = []
			for modifyGrid in modifyGridIds:
				syncModifyGridInfos.append(self.gridInfoDict[modifyGrid])
			for modifyCardUid in self.uniqueCardDict:
				syncEffectInfos = []
				for k,v in self.uniqueCardDict[modifyCardUid]["effects"]:
					syncEffectInfo = {
						"effectName": k,
						"countDown": v["countDonw"],
						"availableTimes": v["availableTimes"]
					}
					syncEffectInfos.append(syncEffectInfo)
				syncModifyCardInfo = {
					"cardKey": modifyCardUid,
					"cardName": self.uniqueCardDict[modifyCardUid]["cardName"],
					"hp": self.uniqueCardDict[modifyCardUid]["hp"],
					"defence": self.uniqueCardDict[modifyCardUid]["defence"],
					"agility": self.uniqueCardDict[modifyCardUid]["agility"],
					"tags": self.uniqueCardDict[modifyCardUid]["tags"],
					"stateTags": self.uniqueCardDict[modifyCardUid]["stateTags"],
					"effectInfos": syncEffectInfos
				}
				syncModifyCardInfos.append(syncModifyCardInfo)
			# notify all clients about latest modification
			syncModificationInfo = {
				"actionSequence": self.curActionSequence,
				"updateGridList": syncModifyGridInfos,
				"updateCardList": syncModifyCardInfos,
			}
			for avatar in self.avatars:
				self.avatars[avatar].roomReqUpdateActionModification(syncModificationInfo)
		else:
			# if effect lauching fails, notify corresponding client
			self.avatars[launchAvatarId].roomReqNotifyLaunchSkillFailed(self.curActionSequence, clientActionSequence)

# skillInfo should look like
# skillTypeName/auto=0/countdown=1/availableTimes=3/prereqTriggerEffect=hurt/prereqTriggerVal=1/skillRecount=3/skillValues=1,2,3/skillDis=3/skillTag=spy
def getSkillLegality(state_list, y, x):
	if state_list[y][x] == '--':
		return False
	else:
		split_str = state_list[y][x].split('_')
		skillInfoStr = split_str[4]
		skillInfos = skillInfoStr.split('/')
		if skillInfos[0] == "HornHurtDiagonal":
			# which means this skill requires launching card located at horn center and target is located at diagonal pose
			if y == 0:
				if x == 0:
					if state_list[y + 1][x] == '--' or state_list[y][x + 1] == '--' or state_list[y + 1][x + 1] == '--':
						return False, 0
					else:
						# check whether diagonal card belongs to adversary
						return True, actionId
				elif x == 7:
					if state_list[y + 1][x] == '--' or state_list[y][x - 1] == '--' or state_list[y + 1][x - 1] == '--':
						return False, 0
					else:
						return True, actionId
				else:
					# top left target
					if state_list[y + 1][x] != '--' and state_list[y][x - 1] != '--' and state_list[y + 1][x - 1] != '--':
						return True, actionId
					# top right target
					elif state_list[y + 1][x] != '--' and state_list[y][x + 1] != '--' and state_list[y + 1][x + 1] != '--':
						return True, actionId
					else:
						return False, 0
			elif y == 7:
				if x == 0:
					if state_list[y + 1][x] == '--' or state_list[y][x + 1] == '--' or state_list[y + 1][x + 1] == '--':
						return False, 0
					else:
						# check whether diagonal card belongs to adversary
						return True, actionId
				elif x == 7:
					if state_list[y + 1][x] == '--' or state_list[y][x - 1] == '--' or state_list[y + 1][x - 1] == '--':
						return False, 0
					else:
						return True, actionId
				else:
					# top left target
					if state_list[y + 1][x] != '--' and state_list[y][x - 1] != '--' and state_list[y + 1][x - 1] != '--':
						return True, actionId
					# top right target
					elif state_list[y + 1][x] != '--' and state_list[y][x + 1] != '--' and state_list[y + 1][x + 1] != '--':
						return True, actionId
					else:
						return False, 0
			else:
				if x == 0:
					sdf
				elif x == 7:
					sdf
				else:
					

def get_legal_moves(state_deque, cur_play_camp):
	state_list = state_deque[-1]

	moves = []

	# traverse all skill moves


	# traverse all motion moves
	for y in range(8):
		for x in range(8):
			if state_list[y][x] == '--':
				# which means this grid is empty, we can put a hand card in this grid
				pass
			else:
				# split string by _ which seperates camp and card type
				split_str = state_list[y][x].split('_')
				# [0]camp_[1]skillState_[2]moveType_
				# [3]skillPrereqGeoType_[4]skillApplyGeoType_[5]skillType_
				# [6]hp_[7]defence_[8]agility_[9]tag_[10]stateTag_[11]stateTagRound
				# [12]cardName
				if split_str[0] == cur_play_camp:
					# which means this card belongs to player in turn
					if split_str[1] == 1 or split_str[1] == 2:
						# which means card skill is available or repeatable
						if split_str[4] == 1:
							# which means this skill requires launch card is located at triangle wind
							# c1 -- --
							# c2 c3 --
							if state_list[y][x - 1] != '--':
								if state_list[y-1][x] != '--':
									# which means this card is located at triangle center
									

					move_way = get_move_way(split_str[12])
					if move_way == 0:
						pass
					elif move_way == 1:
						# jump across a obstable card


