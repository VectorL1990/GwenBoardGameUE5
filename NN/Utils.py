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
	
def GetSkillLaunchStrByCoding(code):
	if code == 0:
		return "auto"
	elif code == 1:
		return "manual"
	elif code == 2:
		return "manualImmediate"
	elif code == 3:
		return "passive"
	else:
		return "null"
	
def GetSkillGeoStrByCoding(code):
	if code == 0:
		return "pointOnLine"
	elif code == 1:
		return "lineSweep"
	elif code == 2:
		return "aoe"
	elif code == 3:
		return "seperated"
	elif code == 4:
		return "pointOnDiagonal"
	elif code == 5:
		return "diagonalSweep"
	elif code == 6:
		return "diagonalSeperated"
	elif code == 7:
		return "hornDiagonal"
	elif code == 8:
		return "hornDiagonalSweep"
	elif code == 9:
		return "obliqueCrossLineUp"
	elif code == 10:
		return "obliqueCrossLineSweep"
	elif code == 11:
		return "triangleSeperated"
	elif code == 12:
		return "triangleSeperatedLineSweep"
	elif code == 13:
		return "triangleSeperatedAOE"
	elif code == 14:
		return "triangleLine"
	elif code == 15:
		return "threeGridsPerpendicular"
	elif code == 16:
		return "threeGridsPerpendicularLineSweep"
	elif code == 17:
		return "threeGridsPerpendicularLineAOE"
	elif code == 18:
		return "connects"
	elif code == 19:
		return "teleport"
	elif code == 20:
		return "arbitrary"
	
def GetSkillEffectTypeByCoding(code):
	if code == 0:
		return "hurt"
	

def GetSkillTagConditionTypeByCoding(code):
	if code == 0:
		return "tagOnBoard"
	
def GetCardTypeByCoding(code):
	if code == 0:
		return "soldier"

def GetSkillPrereqTypeByCoding(code):
	if code == 0:
		return "beHeal"
	
def GetSkillLinkTypeByCoding(code):
	if code == 0:
		return "healTogether"
	

    
UtilFuncDict = {
	'GetCosSimilarity': GetCosSimilarity,
	"GetSkillLaunchStrByCoding": GetSkillLaunchStrByCoding,
	"GetSkillGeoStrByCoding": GetSkillGeoStrByCoding,
	"GetSkillEffectTypeByCoding": GetSkillEffectTypeByCoding,
	"GetSkillTagConditionTypeByCoding": GetSkillTagConditionTypeByCoding,
	"GetCardTypeByCoding": GetCardTypeByCoding,
	"GetSkillPrereqTypeByCoding": GetSkillPrereqTypeByCoding,
	"GetSkillLinkTypeByCoding": GetSkillLinkTypeByCoding
}
