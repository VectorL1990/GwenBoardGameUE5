import numpy as np
import copy
import time
import GlobalConst
from config import CONFIG
from AllCards import allCards
from d_effects import effect_dict
from collections import deque
from Mcts import MCTSPlayer
from GetActions import 
import random

# [0]uid
# [1]cardName
# [2]camp
# [3]skillLaunchType
# [4]skillType(hurt, heal, link, lock)
# [5]skillGeoType
# [6]skillTagConditionType
# [7]tagType
# [8]linkStateType
# [9]linkStateLeftRound
# [10]linkPairNb
# [11]addTagType
# [12]addTagLeftRound
# [13]hp
# [14]defence
# [15]agility
# [16]attackRange
# [17]cardHirachy

# range 3
defaultSkillLaunchCode = np.array([0,0,0])
skillLaunchCoding = dict(
	auto = np.array([1,0,0]),
	manual = np.array([0,1,0]),
	passive = np.array([0,0,1])
)

# range 21
defaultSkillGeoCode = np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
skillGeoCoding = dict(
	pointOnLine = 						np.array([1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	lineSweep = 						np.array([0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	aoe = 								np.array([0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	seperated = 						np.array([0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	pointOnDiagonal = 					np.array([0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	diagonalSweep = 					np.array([0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	diagonalSeperated = 				np.array([0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	hornDiagonal = 						np.array([0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	hornDiagonalSweep = 				np.array([0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0]),
	obliqueCrossLineUp = 				np.array([0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0]),
	obliqueCrossLineSweep = 			np.array([0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0]),
	triangleSeperated = 				np.array([0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0]),
	triangleSeperatedLineSweep = 		np.array([0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0]),
	triangleSeperatedAOE = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0]),
	triangleLine = 						np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0]),
	threeGridsPerpendicular = 			np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0]),
	threeGridsPerpendicularLineSweep = 	np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0]),
	threeGridsPerpendicularLineAOE = 	np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0]),
	connect = 							np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0]),
	teleport = 							np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0]),
	arbitrary = 						np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1]),
)

# range 17
defaultSkillEffectCode = np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
skillEffectCoding = dict(
	hurt = 			np.array([1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	heal = 			np.array([0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	devour = 		np.array([0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	spawn = 		np.array([0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	lock = 			np.array([0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0]),
	absort = 		np.array([0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0]),
	infect = 		np.array([0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0]),
	convert = 		np.array([0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0]),
	exchange = 		np.array([0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0]),
	suborn = 		np.array([0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0]),
	explode = 		np.array([0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0]),
	link = 			np.array([0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0]),
	assimilate = 	np.array([0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0]),
	addTag = 		np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0]),
	# which includes add defence or reinforce agility
	reinforce = 	np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0]),
	# which includes elminate cards progressively(like poison tag) or immediately
	eliminate = 	np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0]),
	# which includes eliminate tags and unlock movement restriction
	purify = 		np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1])
)

# range 3
defaultSkillTagConditionCode = np.array([0,0,0])
skillTagConditionCoding = dict(
	tagOnBoard = np.array([1,0,0]),
	tagOnTarget = np.array([0,1,0]),
	tagOnSelf = np.array([0,0,1])
)

# range 10
defaultCardTypeCode = np.array([0,0,0,0,0,0,0,0,0,0])
cardTypeCoding = dict(
	tag1 = np.array([1,0,0,0,0,0,0,0,0,0]),
	tag2 = np.array([0,1,0,0,0,0,0,0,0,0]),
	tag3 = np.array([0,0,1,0,0,0,0,0,0,0]),
	tag4 = np.array([0,0,0,1,0,0,0,0,0,0]),
	tag5 = np.array([0,0,0,0,1,0,0,0,0,0]),
	tag6 = np.array([0,0,0,0,0,1,0,0,0,0]),
	tag7 = np.array([0,0,0,0,0,0,1,0,0,0]),
	tag8 = np.array([0,0,0,0,0,0,0,1,0,0]),
	tag9 = np.array([0,0,0,0,0,0,0,0,1,0]),
	tag10 = np.array([0,0,0,0,0,0,0,0,0,1])
)

# range 3
defaultCardHirachyCode = np.array([0,0,0])
cardHirachyCoding = dict(
	low = np.array([1,0,0]),
	mid = np.array([0,1,0]),
	high = np.array([0,0,1])
)

# range 10
defaultLinkCode = np.array([0,0,0,0,0,0,0,0,0,0])
linkCoding = dict(
	link1 = np.array([1,0,0,0,0,0,0,0,0,0])
	link2 = np.array([0,1,0,0,0,0,0,0,0,0])
	link3 = np.array([0,0,1,0,0,0,0,0,0,0])
	link4 = np.array([0,0,0,1,0,0,0,0,0,0])
	link5 = np.array([0,0,0,0,1,0,0,0,0,0])
	link6 = np.array([0,0,0,0,0,1,0,0,0,0])
	link7 = np.array([0,0,0,0,0,0,1,0,0,0])
	link8 = np.array([0,0,0,0,0,0,0,1,0,0])
	link9 = np.array([0,0,0,0,0,0,0,0,1,0])
	link10 = np.array([0,0,0,0,0,0,0,0,0,1])
)

class Board(object):
	def __init__(self):
		self.uniqueCardDict = {}
		self.boardState = [['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--'],
							['--', '--', '--', '--', '--', '--', '--', '--']]

		self.downSectionHandCards = ['--', '--', '--', '--', '--', '--', '--', '--', '--', '--']
		self.upSectionHandCards = ['--', '--', '--', '--', '--', '--', '--', '--', '--', '--']
		self.curPlayerId = 0

	def CardCoding(self, cardStateStr):
		# 3 + 15 + 15 + 3 + 10 + 3 + 10 + hp + defence + agil + attackRange = 63
		# total 63 channels
		if cardStateStr == "--":
			return np.zeros(63)
		else:
			cardStateStrs = cardStateStr.split("/")
			skillLaunchTypeStr = cardStateStrs[3]
			skillEffectTypeStr = cardStateStrs[4]
			skillGeoTypeStr = cardStateStrs[5]
			skillTagConditionTypeStr = cardStateStrs[6]
			cardTypeStr = cardStateStrs[7]
			linkNbStr = cardStateStrs[10]
			hpStr = cardStateStrs[13]
			defenceStr = cardStateStrs[14]
			agilityStr = cardStateStrs[15]
			attackRangeStr = cardStateStrs[16]
			cardHirachyStr = cardStateStrs[17]


			skillLaunchTypeCode = defaultSkillLaunchCode
			if skillLaunchTypeStr in skillLaunchCoding:
				skillLaunchTypeCode = skillLaunchCoding[skillLaunchTypeStr]

			skillEffectCode = defaultSkillEffectCode
			if skillEffectTypeStr in skillEffectCoding:
				skillEffectCode = skillEffectCoding[skillEffectTypeStr]

			skillGeoCode = defaultSkillGeoCode
			if skillGeoTypeStr in skillGeoCoding:
				skillGeoCode = skillGeoCoding[skillGeoTypeStr]

			skillTagConditionTypeCode = defaultSkillTagConditionCode
			if skillTagConditionTypeStr in skillTagConditionCoding:
				skillTagConditionTypeCode = skillTagConditionCoding[skillTagConditionTypeStr]

			cardTypeCode = defaultCardTypeCode
			if cardTypeStr in cardTypeCoding:
				cardTypeCode = cardTypeCoding[cardTypeStr]

			cardHirachyCode = defaultCardHirachyCode
			if cardHirachyStr in cardHirachyCoding:
				cardHirachyCode = cardHirachyCoding[cardHirachyStr]

			linkCode = defaultLinkCode
			if linkNbStr in linkCoding:
				linkCode = linkCoding[linkNbStr]

			cardCode = skillLaunchTypeCode.extend(skillEffectCode)
			cardCode = cardCode.extend(skillGeoCode)
			cardCode = cardCode.extend(skillTagConditionTypeCode)
			cardCode = cardCode.extend(cardTypeCode)
			cardCode = cardCode.extend(linkCode)
			cardCode = cardCode.extend(int(hpStr))
			cardCode = cardCode.extend(int(defenceStr))
			cardCode = cardCode.extend(int(agilityStr))
			cardCode = cardCode.extend(int(attackRangeStr))
			cardCode = cardCode.extend(cardHirachyCode)

			return cardCode

	def CardDecoding(self, cardCoding):
		sdf
		

	def GetCurrentPlayer(self):
		return self.curPlayerId

	def GetLegalMoves(self, curPlayer):
		moves = []
		# check move card actions
		# check left dir legal moves
		for y in range(0, GlobalConst.maxRow):
			for x in range(0, GlobalConst.maxCol):
				# check legal moves for every grid
				if self.boardState[y][x] == "--":
					# check play card actions
					if curPlayer == 0:
						# which means it's down section player turn, player can not put cards in up section
						if y < GlobalConst.maxRow / 2:
							# traverse all hand cards of down section player
							for cardNb in range(0, len(self.downSectionHandCards)):
								if self.downSectionHandCards[cardNb] != '--':
									action = "playCard_" + str(cardNb) + "_" + str(y) + "_" + str(x)
									moves.append(action)
					else:
						# which means it's up section player turn
						if y >= GlobalConst.maxRow / 2:
							for cardNb in range(0, len(self.upSectionHandCards)):
								if self.upSectionHandCards[cardNb] != '--':
									action = "playCard_" + str(cardNb + GlobalConst.handCardNb) + "_" + str(y) + "_" + str(x)
									moves.append(action)
				else:
					# it could be launch skill move or card movement
					stateStrs = self.boardState[y][x].split('/')
					cardName = stateStrs[1]
					for effectK, effectV in allCards[cardName]["effects"]:
						if effectV["launchType"] == "assign":
							# which means this effect is triggered manually

		
	def GetActionInfoById(self, actionId):
		# actions should be arranged in following orders:
		# play cards in corresponding grids 0 ~ 32, which is 64 actions in total
		# launch skills of cards in corresponding grids 0 ~ 64, target could be 0 ~ 64, so 64x64 = 4096 in total
		# move a card to specific location which is 64x14 = 896
		actionInfo = {}
		totalGridNb = GlobalConst.maxCol * GlobalConst.maxRow
		halfBoardGridNb = GlobalConst.maxCol * GlobalConst.maxRow / 2

		if actionId < GlobalConst.totalPlayCardActionId:
			# which means this action belongs to play card type
			if actionId < GlobalConst.totalPlayCardActionId / 2:
				# which means card is put in down section of board
				handCardNb = actionId // halfBoardGridNb
				targetGridNb = actionId % halfBoardGridNb
				targetGridY = targetGridNb // GlobalConst.maxCol
				targetGridX = targetGridNb % GlobalConst.maxCol
				actionInfo = {
					"actionType": "playCard",
					"launchSection": "down",
					"handCardNb": handCardNb,
					"targetGridY": targetGridY,
					"targetGridX": targetGridX
				}
			else:
				# which means card is put in up section
				halfActionId = actionId - GlobalConst.totalPlayCardActionId/2
				handCardNb = halfActionId // halfBoardGridNb
				targetGridNb = halfActionId % halfBoardGridNb
				targetGridY = targetGridNb // GlobalConst.maxCol + 4
				targetGridX = targetGridNb % GlobalConst.maxCol
				actionInfo = {
					"actionType": "playCard",
					"launchSection": "up",
					"handCardNb": handCardNb,
					"targetGridY": targetGridY,
					"targetGridX": targetGridX
				}
		elif actionId < GlobalConst.totalPlayCardActionId + GlobalConst.totalLaunchSkillActionId:
			# which means this action belongs to launching skill type
			launchSkillActionId = actionId - GlobalConst.totalPlayCardActionId
			launchGridNb = launchSkillActionId // totalGridNb
			targetGridNb = launchSkillActionId % totalGridNb
			launchGridY = launchGridNb // GlobalConst.maxCol
			launchGridX = launchGridNb % GlobalConst.maxCol
			targetGridY = targetGridNb // GlobalConst.maxCol
			targetGridX = targetGridNb % GlobalConst.maxCol
			actionInfo = {
				"actionType": "launchSkill",
				"launchGridY": launchGridY,
				"launchGridX": launchGridX,
				"targetGridY": targetGridY,
				"targetGridX": targetGridX
			}
		else:
			# which means this action belongs to move card type
			moveCardActionId = actionId - GlobalConst.totalPlayCardActionId - GlobalConst.totalLaunchSkillActionId
			launchGridNb = moveCardActionId // 


	def DoMove(self, actionId):
		actionInfo = self.GetActionInfoById(actionId)
		if actionInfo["actionType"] == 0:
			# which means it's card playing action
			playerReqPlayCardAction(self.boardState, self.uniqueCardDict, actionInfo["playCardUid"], actionInfo["targetX"], actionInfo["targetY"])
		elif actionInfo["actionType"] == 1:
			# which means it's skill launching action
			playerReqLaunchCardSkillAction(self.boardState, self.uniqueCardDict, actionInfo["launchX"])
		elif actionInfo["actionType"] == 2:
			# which means it's card movement action
			playerReqMove()

	def GameEnd(self):
		sdf

	def AISelfPlay(self, mctsPlayer):
		mctProbs = []
		actionCount = 0
		while True:
			actionCount += 1
			moveId, moveProb = mctsPlayer.GetAction(self)
			mctProbs.append(moveProb)
			self.DoMove(moveId)
			isEnd, winner = self.GameEnd()
			if isEnd == True:
				return winner, moveProb





########## Global functions for both pvp and ai battle
			
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

def playerReqMove(boardState, launchX, launchY, targetX, targetY):
	legality = geo_rule_dict["Teleport"](launchX, launchY, targetX, targetY)
	if legality == True:
		launchMove(boardState, launchX, launchY, targetX, targetY)

def launchMove(stateList, launchX, launchY, targetX, targetY):
	targetGridStr = stateList[targetY][targetX]
	targetGridStrs = targetGridStr.split('/')
	tags = targetGridStrs[6]
	resultDict = {}
	if "Teleport" in tags:
		# which means target grid is a teleport
		resultDict = effect_dict["Teleport"](stateList, launchX, launchY, targetX, targetY)
	else:
		resultDict = effect_dict["Move"](stateList, launchX, launchY, targetX, targetY)

def TriggerPassiveEffect(stateList, launchResultDict, modifyGrids):
	for modifyGrid in launchResultDict["modifyGrids"]:
		# traverse all passive effects attached to compare
		modifyGridX = modifyGrid["grid"][0]
		modifyGridY = modifyGrid["grid"][1]
		modifyGridState = stateList[modifyGridY, modifyGridX]
		modifyGridStateStrs = modifyGridState.split('/')
		modifyGridUid = modifyGridStateStrs[0]
		for modifyGridEffectKey, modifyGridEffectVal in uniqueCardDict[modifyGridUid]["effects"]:
			if modifyGridEffectVal["launchType"] == "targetPassive":
				if modifyGridEffectVal["prereqs"]["triggerEffectType"] == modifyGrid["modifyType"]:
					if modifyGridEffectKey in passive_effect_dict:
						passiveEffectResult = passive_effect_dict[modifyGridEffectKey](stateList, modifyGridX, modifyGridY, launchX, launchY, modifyGridEffectVal)
						if passiveEffectResult["success"] == True:
							for passiveModifyGrid in passiveEffectResult["modifyGrids"]:
								passiveModifyGridX = passiveModifyGrid["grid"][0]
								passiveModifyGridY = passiveModifyGrid["grid"][1]
								passiveModifyGridStr = stateList[passiveModifyGridY][passiveModifyGridX]
								passiveModifyGridStrs = passiveModifyGridStr.split('/')
								if passiveModifyGridStrs[0] not in modifyGrids:
									modifyGrids.append(passiveModifyGridStrs[0])
							TriggerPassiveEffect(stateList, passiveEffectResult, modifyGrids)

def launchEffect(stateList, launchX, launchY, targetX, targetY, effectInfo, lastGenerateId):
	if effectName in effect_dict:
			resultDict = effect_dict[effectName](self.uniqueCardDict, self.gridInfoDict, self.inBattleAvatarList, launchAvatarId, targetGrid, launchGrid, effectInfo)
			# if launch effect succesfully, settlement has been done
			# broadcast latest operation result to all clients
			modifyGrids = []
			if resultDict["success"] == True:
				TriggerPassiveEffect(stateList, resultDict, modifyGrids)

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
				# [0]uid/ [1]cardName/ [2]camp/ [3]moveType/ [4]skillType(hurt or heal)/
				# [5]skillGeoType/ [6]tagType(may be combination)/ [7]linkStateType/
				# [8]linkStateLeftRound/ [9]linkPairNb/ [10]addTagType/ [11]addTagLeftRound/
				# [12]hp/ [13]defence/ [14]agility
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


