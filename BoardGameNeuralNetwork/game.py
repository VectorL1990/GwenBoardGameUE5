import numpy as np
import copy
import time
from config import CONFIG
from d_all_cards import allCards
from d_effects import effect_dict
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

class Board(object):
	def __init__(self):
		self.boardState = [['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--']]
		
	def get_action_type(self, actionNb):
		sdf


	def do_move(self, actionNb):
		actionType = self.get_action_type(actionNb)
		if actionType == 0:
			# which means it's card playing action
			playerReqPlayCardAction()
		elif actionType == 1:
			# which means it's skill launching action
			playerReqLaunchCardSkillAction()






def playerReqPlayCardAction(boardState, uniqueCardDict, cardId, targetX, targetY):
	# legality check has been done at this stage, only update board state
	cardState = uniqueCardDict[cardId]
	boardState[targetY][targetX] = cardState
	cardStateStrs = cardState.split('/')
	for k,v in allCards[cardStateStrs[2]]["effects"].items():
		if v["launchType"] == "auto":
			launchEffect(avatarEntityCall.id, -1, gridNb, k, v)


def playerReqLaunchCardSkillAction(boardState, uniqueCardDict, launchX, launchY, targetX, targetY):
	launchCardState = boardState[launchY][launchX]
	launchCardStateStrs = launchCardState.split('/')
	for k, v in allCards[launchCardStateStrs[2]]["effects"].items():
		if v["launchType"] == "assign":
			launchEffect()
			break


def launchEffect(stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId):
	if effectName in effect_dict:
			resultDict = effect_dict[effectName](self.uniqueCardDict, self.gridInfoDict, self.inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo)
			# if launch effect succesfully, settlement has been done
			# broadcast latest operation result to all clients
			if resultDict["success"] == True:
				modifyGridIds = resultDict["modifyGrids"]
				# traverse all target cards to trigger their passive effects
				for targetGridXY in resultDict["modifyGrids"]:
					targetGridState = self.state_list[targetGridXY[0], targetGridXY[1]]
					targetGridStateStrs = targetGridState.split('/')
					for targetPassiveEffectKey,targetPassiveEffectValue in self.uniqueCardDict[targetGridStateStrs[0]]["effects"]:
						if targetPassiveEffectValue["launchType"] == "targetPassive":
							if targetPassiveEffectValue["prereqs"]["triggerEffectType"] == resultDict["triggerEffectType"]:
								if targetPassiveEffectKey in passive_effect_dict:
									passiveEffectResultDict = passive_effect_dict[targetPassiveEffectKey]
									(
										self.state_list,
										targetGridXY[0],
										targetGridXY[1],
										launchX,
										launchY,
										targetPassiveEffectValue
									)
									if passiveEffectResultDict["success"] == True:
										targetPassiveEffectValue["effectValues"]["isRoundEnd"] = True
										for passiveModifyGrid in passiveEffectResultDict["modifyGrids"]:
											if passiveModifyGrid not in modifyGridIds:
												modifyGridIds.append(passiveModifyGrid)
				# assemble all modification and notify all clients
				syncModifyGridInfos = []
				for modifyGrid in modifyGridIds:
					syncModifyGridInfos.append(self.gridInfoDict[modifyGrid])
				
				# only necessary when it's pvp battle
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


