import numpy as np
import copy
import time
from config import CONFIG
from collections import deque   # 这个队列用来判断长将或长捉
import random
import GlobalConst


#(state, mcts_prob, winner) ((9,10,9),2086,1) => ((9,90),(2,1043),1)
def ZipStateMctsProb(tuple):
    state, mctsProb, winner = tuple
    state = state.reshape((9,-1))
    mctsProb = mctsProb.reshape((2,-1))
    state = ZipArray(state)
    mctsProb = ZipArray(mctsProb)
    return state,mctsProb,winner

def RecoveryStateMctsProb(tuple):
    state, mctsProb, winner = tuple
    state = RecoveryArray(state)
    mctsProb = RecoveryArray(mctsProb)
    state = state.reshape((GlobalConst.maxRow, GlobalConst.maxCol, GlobalConst.cardCodeLen))
    mctsProb = mctsProb.reshape(GlobalConst.totalMoveNb)
    return state,mctsProb,winner

def ZipArray(array, data=0.):  # 压缩成稀疏数组
    zipRes = []
    zipRes.append([len(array), len(array[0])])
    for i in range(len(array)):
        for j in range(len(array[0])):
            if array[i][j] != data:
                zipRes.append([i, j, array[i][j]])
    return np.array(zipRes)


def RecoveryArray(array, data=0.):  # 恢复数组
	recoveryRes = []
	for i in range(array[0][0]):
		recoveryRes.append([data for i in range(array[0][1])])
	for i in range(1, len(array)):
		# print(len(recovery_res[0]))
		recoveryRes[array[i][0]][array[i][1]] = array[i][2]
	return np.array(recoveryRes)

def GetCosSimilarity(a1, a2):
	num = float(np.dot(a1, a2))
	denom = np.linalg.norm(a1) * np.linalg.norm(a2)
	if denom <= 0.0001:
		return 0
	else:
		return num/denom

def DecodeSkillLaunchType(code):
	skillLaunchTypeDefaultCode = np.zeros(GlobalConst.skillLaunchTypdeCodeLen)
	maxSkillLaunchTypeSimilarity = 0
	maxSkillLaunchType = -1
	for i in range(0, GlobalConst.skillLaunchTypdeCodeLen):
		skillLaunchTypeCoding = copy.deepcopy(skillLaunchTypeDefaultCode)
		skillLaunchTypeCoding[i] = 1
		curSkillLaunchTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillLaunchTypeCoding, code)
		if curSkillLaunchTypeSimilarity > maxSkillLaunchTypeSimilarity:
			maxSkillLaunchTypeSimilarity = curSkillLaunchTypeSimilarity
			maxSkillLaunchType = i
	if maxSkillLaunchType != -1:
		if maxSkillLaunchType == 0:
			return "auto"
		elif maxSkillLaunchType == 1:
			return "manual"
		else:
			return "passive"
	else:
		return "null"
	
def DecodeSkillGeoType(code):
	skillGeoTypeDefaultCode = np.zeros(GlobalConst.skillGeoCodeLen)
	maxSkillGeoTypeSimilarity = 0
	maxSkillGeoType = -1
	for i in range(0, GlobalConst.skillGeoCodeLen):
		skillGeoTypeCoding = copy.deepcopy(skillGeoTypeDefaultCode)
		skillGeoTypeCoding[i] = 1
		curSkillGeoTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillGeoTypeCoding, code)
		if curSkillGeoTypeSimilarity > maxSkillGeoTypeSimilarity:
			maxSkillGeoTypeSimilarity = curSkillGeoTypeSimilarity
			maxSkillGeoType = i
	if maxSkillGeoType != -1:
		if maxSkillGeoType == 0:
			return "line"
		elif maxSkillGeoType == 1:
			return "seperated"
		elif maxSkillGeoType == 2:
			return "diagonal"
		elif maxSkillGeoType == 3:
			return "diagonalSeperated"
		elif maxSkillGeoType == 4:
			return "adjacent"
		elif maxSkillGeoType == 5:
			return "hornDiagonal"
		elif maxSkillGeoType == 6:
			return "hornSeperated"
		elif maxSkillGeoType == 7:
			return "normalCrossSeperated"
		elif maxSkillGeoType == 8:
			return "obliqueCrossLineUp"
		elif maxSkillGeoType == 9:
			return "triangleSeperated"
		elif maxSkillGeoType == 10:
			return "triangleLine"
		elif maxSkillGeoType == 11:
			return "threeGridsPerpendicular"
		elif maxSkillGeoType == 12:
			return "connect"
		elif maxSkillGeoType == 13:
			return "teleport"
		else:
			return "arbitrary"
	else:
		return "null"
	
def DecodeSkillEffectType(code):
	skillEffectTypeDefaultCode = np.zeros(GlobalConst.skillEffectCodeLen)
	maxSkillEffectTypeSimilarity = 0
	maxSkillEffectType = -1
	for i in range(0, GlobalConst.skillEffectCodeLen):
		skillEffectTypeCoding = copy.deepcopy(skillEffectTypeDefaultCode)
		skillEffectTypeCoding[i] = 1
		curSkillEffectTypeSimilarity = UtilFuncDict["GetCosSimilarity"](skillEffectTypeCoding, code)
		if curSkillEffectTypeSimilarity > maxSkillEffectTypeSimilarity:
			maxSkillEffectTypeSimilarity = curSkillEffectTypeSimilarity
			maxSkillEffectType = i
	if maxSkillEffectType != -1:
		if maxSkillEffectType == 0:
			return "hurt"
		elif maxSkillEffectType == 1:
			return "heal"
    
UtilFuncDict = {
	'GetCosSimilarity': GetCosSimilarity
}
