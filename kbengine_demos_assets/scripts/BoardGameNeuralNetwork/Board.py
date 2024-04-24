import numpy as np
import copy
import time
import GlobalConst
from config import CONFIG
from AllCards import allCards
from d_effects import effect_dict
from collections import deque
from Mcts import MCTSPlayer
from Utils import UtilFuncDict
from GetPossibleActions import legalSkillActionsDict
import random

# [0]uid
# [1]cardName
# [2]camp
# [3]skillLaunchType
# [4]skillType(hurt, heal, link, lock)
# [5]skillGeoType
# [6]skillTagConditionType
# [7]skillPrereqTagType
# [8]tagType
# [9]skillPrereqType
# [10]skillLinkType
# [11]linkPairNb
# [12]linkStateLeftRound
# [13]skillName:SeperatedSwap & skillCountDown:0 & skillAvailableTime:1 & selfTarget:0 & prereqTriggerValue:1 & value:3 & assignTag:spy
# [14]hp
# [15]defence
# [16]agility
# [17]attackRange

# range 5
defaultSkillLaunchCode = np.array([0,0,0,0,0])
skillLaunchCoding = dict(
	auto = 				np.array([1,0,0,0,0]),
	autoRoundEnd = 		np.array([0,1,0,0,0]),
	manual = 			np.array([0,0,1,0,0]),
	manualImmediate = 	np.array([0,0,0,1,0]),
	passive = 			np.array([0,0,0,0,1])
)

# range 14
defaultSkillLauchGeoCode = np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0])
skillLaunchGeoCoding = dict(
	point = 					np.array([1,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	three = 					np.array([0,1,0,0,0,0,0,0,0,0,0,0,0,0]),
	horn = 						np.array([0,0,1,0,0,0,0,0,0,0,0,0,0,0]),
	triangle = 					np.array([0,0,0,1,0,0,0,0,0,0,0,0,0,0]),
	obliqueCross = 				np.array([0,0,0,0,1,0,0,0,0,0,0,0,0,0]),
	normalCross = 				np.array([0,0,0,0,0,1,0,0,0,0,0,0,0,0]),
	selfSection = 				np.array([0,0,0,0,0,0,1,0,0,0,0,0,0,0]),
	selfFrontCol = 				np.array([0,0,0,0,0,0,0,1,0,0,0,0,0,0]),
	selfBackCol = 				np.array([0,0,0,0,0,0,0,0,1,0,0,0,0,0]),
	selfFrontLine = 			np.array([0,0,0,0,0,0,0,0,0,1,0,0,0,0]),
	oppoSection = 				np.array([0,0,0,0,0,0,0,0,0,0,1,0,0,0]),
	oppoFrontCol = 				np.array([0,0,0,0,0,0,0,0,0,0,0,1,0,0]),
	oppoBackCol = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,1,0]),
	oppoFrontLine = 			np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,1])
)

defaultSkillTargetGeoCode = np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
skillTargetGeoCoding = dict(
	line = 						np.array([1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	seperated = 				np.array([0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	diagonal = 					np.array([0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	diagonalSeperated = 		np.array([0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	three = 					np.array([0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	horn = 						np.array([0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	triangle = 					np.array([0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0]),
	obliqueCross = 				np.array([0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0]),
	normalCross = 				np.array([0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0]),
	selfSection = 				np.array([0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0]),
	selfFrontCol = 				np.array([0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0]),
	selfBackCol = 				np.array([0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0]),
	selfFrontLine = 			np.array([0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0]),
	oppoSection = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0]),
	oppoFrontCol = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0]),
	oppoBackCol = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0]),
	oppoFrontLine = 			np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0]),
	connect = 					np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0]),
	teleport = 					np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0]),
	arbitrary = 				np.array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1])
)

defaultSkillAoeCode = np.array([0,0,0,0,0,0])
skillAoeCoding = dict(
	point = 				np.array([1,0,0,0,0,0]),
	sweep = 				np.array([0,1,0,0,0,0]),
	H3 = 					np.array([0,0,1,0,0,0]),
	V3 = 					np.array([0,0,0,1,0,0]),
	normalCross = 			np.array([0,0,0,0,1,0]),
	obliqueCross = 			np.array([0,0,0,0,0,1])
)

defaultSkillTargetCampCode = np.array([0,0,0])
skillTargetCampCoding = dict(
	selfCamp = 				np.array([1,0,0]),
	oppoCamp = 				np.array([0,1,0]),
	arbitraryCamp = 		np.array([0,0,1])
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
	tagOnSelf = np.array([0,0,1]),
	tagOnNext,
	tagOnPlayCard,
)


defaultCardTypeCode = np.zeros(GlobalConst.cardTypeCodeLen)
cardTypeCoding = dict()
def InitCardTagDict(cardTagDict):
	for i in range(GlobalConst.cardTypeCodeLen):
		key = "tag" + str(i)
		val = defaultCardTypeCode
		val[i] = 1
		cardTagDict[key] = val


defaultSkillPrereqTagCode = np.zeros(GlobalConst.cardTypeCodeLen)
skillPrereqTagCoding = dict()
def InitSkillPrereqTagDict(skillPrereqTagDict):
	for i in range(GlobalConst.cardTypeCodeLen):
		key = "tag" + str(i)
		val = defaultSkillPrereqTagCode
		val[i] = 1
		skillPrereqTagDict[key] = val

defaultSkillPrereqCode = np.array([0,0,0,0,0,0])
skillPrereqCoding = dict(
	beHeal = 			np.array([1,0,0,0,0,0]),
	beHurt = 			np.array([0,1,0,0,0,0]),
	armorPenetrate = 	np.array([0,0,1,0,0,0]),
	hpSum = 			np.array([0,0,0,1,0,0]),
	hpSumMore  =	np.array([0,0,0,0,0,1]),
	hpSumLess  =	np.array([0,0,0,0,0,1]),
	hpLess = 		np.array([0,0,0,0,1,0]),
	hpMore = 	np.array([0,0,0,0,0,1]),
	minHpOnBoard = 	np.array([0,0,0,0,0,1]),
	maxHpOnBoard = 	np.array([0,0,0,0,0,1]),
	sameRowMore = 	np.array([0,0,0,0,0,1]),
	sameRowLess = 	np.array([0,0,0,0,0,1]),
	sameColMore  = 	np.array([0,0,0,0,0,1]),
	sameColLess =	np.array([0,0,0,0,0,1]),
	defenceLess =	np.array([0,0,0,0,0,1]),
	hurtCardMore =	np.array([0,0,0,0,0,1]),
	hurtCardLess =	np.array([0,0,0,0,0,1]),
	healCardMore =	np.array([0,0,0,0,0,1]),
	healCardLess =	np.array([0,0,0,0,0,1]),
	compareHpMore  =	np.array([0,0,0,0,0,1]),
	compareHpLess  =	np.array([0,0,0,0,0,1]),
	handCardMore  =	np.array([0,0,0,0,0,1]),
	handCardLess  =	np.array([0,0,0,0,0,1]),
	graveCardMore  =	np.array([0,0,0,0,0,1]),
	graveCardLess  =	np.array([0,0,0,0,0,1]),
	sameRowHpMore  =	np.array([0,0,0,0,0,1]),
	sameRowHpLess  =	np.array([0,0,0,0,0,1]),
	sameColHpMore  =	np.array([0,0,0,0,0,1]),
	sameColHpLess  =	np.array([0,0,0,0,0,1]),
	nextToHurt  =	np.array([0,0,0,0,0,1]),
	nextToHeal  =	np.array([0,0,0,0,0,1]),

)


# range 5
defaultSkillLinkTypeCode = np.array([0,0,0,0,0])
skillLinkTypeCoding = dict(
	healTogether = 	np.array([1,0,0,0,0]),
	hurtTogether = 	np.array([0,1,0,0,0]),
	healTransfer = 	np.array([0,0,1,0,0]),
	hurtTransfer = 	np.array([0,0,0,1,0]),
	vampiric = 		np.array([0,0,0,0,1])
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
		# skillLaunchType = 3
		# skillGeoType = 22
		# skillEffectType = 17
		# skillTagConditionType = 3
		# skillPrereqTagType = 50
		# cardTagType = 50
		# skillPrereqType = 6
		# skillLinkType = 5
		# 3 + 22 + 17 + 3 + 50 + 50 + 6 + 5 + hp + defence + agil + attackRange = 159
		# total 158 channels
		if cardStateStr == "--":
			return np.zeros(158)
		else:
			cardStateStrs = cardStateStr.split("/")
			skillLaunchTypeStr = cardStateStrs[3]
			skillEffectTypeStr = cardStateStrs[4]
			skillGeoTypeStr = cardStateStrs[5]
			skillTagConditionTypeStr = cardStateStrs[6]
			cardTypeStr = cardStateStrs[7]
			skillPrereqTagTypeStr = cardStateStrs[8]
			skillPrereqTypeStr = cardStateStrs[9]
			skillLinkTypeStr = cardStateStrs[10]
			hpStr = cardStateStrs[14]
			defenceStr = cardStateStrs[15]
			agilityStr = cardStateStrs[16]
			attackRangeStr = cardStateStrs[17]


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

			skillPrereqTagTypeCode = defaultSkillPrereqTagCode
			if skillPrereqTagTypeStr in skillPrereqTagCoding:
				skillPrereqTagTypeCode = skillPrereqTagCoding[skillPrereqTagTypeStr]

			cardTypeCode = defaultCardTypeCode
			if cardTypeStr in cardTypeCoding:
				cardTypeCode = cardTypeCoding[cardTypeStr]

			skillPrereqTypeCode = defaultSkillPrereqCode
			if skillPrereqTypeStr in skillPrereqCoding:
				skillPrereqTypeCode = skillPrereqCoding[skillPrereqTypeStr]

			skillPrereqTagTypeCode = defaultSkillPrereqTagCode
			if skillPrereqTagTypeStr in skillPrereqTagCoding:
				skillPrereqTagTypeCode = skillPrereqTagCoding[skillPrereqTagTypeStr]

			skillPrereqTypeCode = defaultSkillPrereqCode
			if skillPrereqTypeStr in skillPrereqCoding:
				skillPrereqTypeCode = skillPrereqCoding[skillPrereqTypeStr]

			skillLinkTypeCode = defaultSkillLinkTypeCode
			if skillLinkTypeStr in skillLinkTypeCoding:
				skillLinkTypeCode = skillLinkTypeCoding[skillLinkTypeStr]

			cardCode = skillLaunchTypeCode.extend(skillEffectCode)
			cardCode = cardCode.extend(skillGeoCode)
			cardCode = cardCode.extend(skillTagConditionTypeCode)
			cardCode = cardCode.extend(skillPrereqTagTypeCode)
			cardCode = cardCode.extend(cardTypeCode)
			cardCode = cardCode.extend(skillPrereqTagTypeCode)
			cardCode = cardCode.extend(skillPrereqTypeCode)
			cardCode = cardCode.extend(skillLinkTypeCode)
			cardCode = cardCode.extend(int(hpStr))
			cardCode = cardCode.extend(int(defenceStr))
			cardCode = cardCode.extend(int(agilityStr))
			cardCode = cardCode.extend(int(attackRangeStr))

			return cardCode

	# this function is only being called by action interpretation
	# which means nn outputs a specific action, which should be interpreted as a string type action
	# which indicates how to select specific card to put on board
	def CardDecoding(self, cardCoding):
		skillLaunchTypeStr = ""
		curCodeLen = 0
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen
		rSkillLaunchTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillLaunchTypeDefaultCode = np.zeros(GlobalConst.skillLaunchTypdeCodeLen)
		maxSkillLaunchTypeSimilarity = 0
		maxSkillLaunchType = -1
		for i in range(0, GlobalConst.skillLaunchTypdeCodeLen):
			skillLaunchTypeCoding = copy.deepcopy(skillLaunchTypeDefaultCode)
			skillLaunchTypeCoding[i] = 1
			curSkillLaunchTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillLaunchTypeCoding, rSkillLaunchTypeCode)
			if curSkillLaunchTypeSimilarity > maxSkillLaunchTypeSimilarity:
				maxSkillLaunchTypeSimilarity = curSkillLaunchTypeSimilarity
				maxSkillLaunchType = i
		if maxSkillLaunchType != -1:
			skillLaunchTypeStr = UtilFuncDict["GetSkillLaunchStrByCoding"](maxSkillLaunchType)

		skillGeoTypeStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen
		rSkillGeoTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillGeoTypeDefaultCode = np.zeros(GlobalConst.skillGeoCodeLen)
		maxSkillGeoTypeSimilarity = 0
		maxSkillGeoType = -1
		for i in range(0, GlobalConst.skillGeoCodeLen):
			skillGeoTypeCoding = copy.deepcopy(skillGeoTypeDefaultCode)
			skillGeoTypeCoding[i] = 1
			curSkillGeoTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillGeoTypeCoding, rSkillGeoTypeCode)
			if curSkillGeoTypeSimilarity > maxSkillGeoTypeSimilarity:
				maxSkillGeoTypeSimilarity = curSkillGeoTypeSimilarity
				maxSkillGeoType = i
		if maxSkillGeoType != -1:
			skillGeoTypeStr = UtilFuncDict["GetSkillGeoStrByCoding"](maxSkillGeoType)

		skillEffectTypeStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen
		rSkillEffectTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillEffectTypeDefaultCode = np.zeros(GlobalConst.skillEffectCodeLen)
		maxSkillEffectTypeSimilarity = 0
		maxSkillEffectType = -1
		for i in range(0, GlobalConst.skillEffectCodeLen):
			skillEffectTypeCoding = copy.deepcopy(skillEffectTypeDefaultCode)
			skillEffectTypeCoding[i] = 1
			curSkillEffectTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillEffectTypeCoding, rSkillEffectTypeCode)
			if curSkillEffectTypeSimilarity > maxSkillEffectTypeSimilarity:
				maxSkillEffectTypeSimilarity = curSkillEffectTypeSimilarity
				maxSkillEffectType = i
		if maxSkillEffectType != -1:
			skillEffectTypeStr = UtilFuncDict["GetSkillEffectTypeByCoding"](maxSkillEffectType)

		skillTagConditionStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen
		rSkillTagConditionTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillTagConditionDefaultCode = np.zeros(GlobalConst.skillTagConditionCodeLen)
		maxSkillTagConditionSimilarity = 0
		maxSkillTagCondition = -1
		for i in range(0, GlobalConst.skillTagConditionCodeLen):
			skillTagConditionCoding = copy.deepcopy(skillTagConditionDefaultCode)
			skillTagConditionCoding[i] = 1
			curSkillTagConditionSimilarity = UtilFuncDict["GetCosSimilarity"](skillTagConditionCoding, rSkillTagConditionTypeCode)
			if curSkillTagConditionSimilarity > maxSkillTagConditionSimilarity:
				maxSkillTagConditionSimilarity = curSkillTagConditionSimilarity
				maxSkillTagCondition = i
		if maxSkillTagCondition != -1:
			skillTagConditionStr = UtilFuncDict["GetSkillTagConditionTypeByCoding"](maxSkillTagCondition)

		cardTypeStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen
		rCardTypeCode = cardCoding[curCodeLen : nextCodeLen]
		cardTypeDefaultCode = np.zeros(GlobalConst.cardTypeCodeLen)
		maxCardTypeSimilarity = 0
		maxCardType = -1
		for i in range(0, GlobalConst.cardTypeCodeLen):
			cardTypeCoding = copy.deepcopy(cardTypeDefaultCode)
			cardTypeCoding[i] = 1
			curCardTypeSimilarity = UtilFuncDict["GetCosSimilarity"](cardTypeCoding, rCardTypeCode)
			if curCardTypeSimilarity > maxCardTypeSimilarity:
				maxCardTypeSimilarity = curCardTypeSimilarity
				maxCardType = i
		if maxCardType != -1:
			cardTypeStr = UtilFuncDict["GetCardTypeByCoding"](maxCardType)

		skillPrereqTagStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen + GlobalConst.skillPrereqTagTypeCodeLen
		rSkillPrereqTagTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillPrereqTagTypeDefaultCode = np.zeros(GlobalConst.skillPrereqTagTypeCodeLen)
		maxSkillPrereqTagTypeSimilarity = 0
		maxSkillPrereqTagType = -1
		for i in range(0, GlobalConst.skillPrereqTagTypeCodeLen):
			skillPrereqTagTypeCoding = copy.deepcopy(skillPrereqTagTypeDefaultCode)
			skillPrereqTagTypeCoding[i] = 1
			curSkillPrereqTagTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillPrereqTagTypeCoding, rSkillPrereqTagTypeCode)
			if curSkillPrereqTagTypeSimilarity > maxSkillPrereqTagTypeSimilarity:
				maxSkillPrereqTagTypeSimilarity = curSkillPrereqTagTypeSimilarity
				maxSkillPrereqTagType = i
		if maxSkillPrereqTagType != -1:
			skillPrereqTagStr = UtilFuncDict["GetCardTypeByCoding"](maxSkillPrereqTagType)

		skillPrereqTypeStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen + GlobalConst.skillPrereqTagTypeCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen + GlobalConst.skillPrereqTagTypeCodeLen + GlobalConst.skillPrereqTypeCodeLen
		rSkillPrereqTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillPrereqTypeDefaultCode = np.zeros(GlobalConst.skillPrereqTypeCodeLen)
		maxSkillPrereqTypeSimilarity = 0
		maxSkillPrereqType = -1
		for i in range(0, GlobalConst.skillPrereqTypeCodeLen):
			skillPrereqTypeCoding = copy.deepcopy(skillPrereqTypeDefaultCode)
			skillPrereqTypeCoding[i] = 1
			curSkillPrereqTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillPrereqTypeCoding, rSkillPrereqTypeCode)
			if curSkillPrereqTypeSimilarity > maxSkillPrereqTypeSimilarity:
				maxSkillPrereqTypeSimilarity = curSkillPrereqTypeSimilarity
				maxSkillPrereqType = i
		if maxSkillPrereqType != -1:
			skillPrereqTypeStr = UtilFuncDict["GetSkillLinkTypeByCoding"](maxSkillPrereqType)

		skillLinkTypeStr = ""
		curCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen + GlobalConst.skillPrereqTagTypeCodeLen + GlobalConst.skillPrereqTypeCodeLen
		nextCodeLen = GlobalConst.skillLaunchTypeCodeLen + GlobalConst.skillGeoCodeLen + GlobalConst.skillEffectCodeLen + GlobalConst.skillTagConditionCodeLen + GlobalConst.cardTypeCodeLen + GlobalConst.skillPrereqTagTypeCodeLen + GlobalConst.skillPrereqTypeCodeLen + GlobalConst.skillLinkTypeCodeLen
		rSkillLinkTypeCode = cardCoding[curCodeLen : nextCodeLen]
		skillLinkTypeDefaultCode = np.zeros(GlobalConst.skillLinkTypeCodeLen)
		maxSkillLinkTypeSimilarity = 0
		maxSkillLinkType = -1
		for i in range(0, GlobalConst.skillLinkTypeCodeLen):
			skillLinkTypeCoding = copy.deepcopy(skillLinkTypeDefaultCode)
			skillLinkTypeCoding[i] = 1
			curSkillLinkTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillLinkTypeCoding, rSkillLinkTypeCode)
			if curSkillLinkTypeSimilarity > maxSkillLinkTypeSimilarity:
				maxSkillLinkTypeSimilarity = curSkillLinkTypeSimilarity
				maxSkillLinkType = i
		if maxSkillLinkType != -1:
			skillLinkTypeStr = UtilFuncDict["GetSkillLinkTypeByCoding"](maxSkillLinkType)

		resultStr = skillLaunchTypeStr + "/" + skillGeoTypeStr + "/" + skillEffectTypeStr + "/" + skillTagConditionStr + "/" + cardTypeStr + "/" + skillPrereqTagStr + "/" + skillPrereqTypeStr + "/" + skillLinkTypeStr
		return resultStr

		

	def GetCurrentPlayer(self):
		return self.curPlayerId

	def GetLegalMoves(self, curPlayer):
		moves = []
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
					skillOveralInfo = stateStrs[13]
					skillInfoStrs = skillOveralInfo.split('&')
					skillName = ""
					skillCountDown = 0
					skillAvailableTime = 0
					skillSelfTarget = 0
					prereqTriggerValue = 0
					skillValue = 0
					assignTag = ""
					for skillInfo in skillInfoStrs:
						skillInfoKV = skillInfo.split(':')
						if skillInfoKV[0] == "":
							skillName = skillInfoKV[1]
						elif skillInfoKV[0] == "skillCountDown":
							skillCountDown = int(skillInfoKV[1])
						elif skillInfoKV[0] == "skillAvailableTimes":
							skillAvailableTime = int(skillInfoKV[1])
						elif skillInfoKV[0] == "selfTarget":
							skillSelfTarget = int(skillInfoKV[1])
						elif skillInfoKV[0] == "prereqTriggerValue":
							prereqTriggerValue = int(skillInfoKV[1])
						elif skillInfoKV[0] == "skillValue":
							skillValue = int(skillInfoKV[1])
						elif skillInfoKV[0] == "assignTag":
							assignTag = skillInfoKV[1]
					
					skillLaunchType = stateStrs[3]
					if skillLaunchType == "manual" or skillLaunchType == "manualImmediate":
						if (skillCountDown == 0 or skillCountDown == -1) and (skillAvailableTime > 0 or skillAvailableTime == -1):
							legalSkillActionsDict[skillName]()
							for targetY in range()
							action = "playSkill_" 
							moves.append() 

		
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
		if v["launchType"] == "manual" or v["launchType"] == "manualImmediate":
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


