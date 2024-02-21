import numpy as np
import copy
import time
from config import CONFIG
from collections import deque   # 这个队列用来判断长将或长捉
import random


#(state, mcts_prob, winner) ((9,10,9),2086,1) => ((9,90),(2,1043),1)
def zip_state_mcts_prob(tuple):
    state, mcts_prob, winner = tuple
    state = state.reshape((9,-1))
    mcts_prob = mcts_prob.reshape((2,-1))
    state = zip_array(state)
    mcts_prob = zip_array(mcts_prob)
    return state,mcts_prob,winner

def recovery_state_mcts_prob(tuple):
    state, mcts_prob, winner = tuple
    state = recovery_array(state)
    mcts_prob = recovery_array(mcts_prob)
    state = state.reshape((9,10,9))
    mcts_prob = mcts_prob.reshape(2086)
    return state,mcts_prob,winner

def zip_array(array, data=0.):  # 压缩成稀疏数组
    zip_res = []
    zip_res.append([len(array), len(array[0])])
    for i in range(len(array)):
        for j in range(len(array[0])):
            if array[i][j] != data:
                zip_res.append([i, j, array[i][j]])
    return np.array(zip_res)


def recovery_array(array, data=0.):  # 恢复数组
    recovery_res = []
    for i in range(array[0][0]):
        recovery_res.append([data for i in range(array[0][1])])
    for i in range(1, len(array)):
        # print(len(recovery_res[0]))
        recovery_res[array[i][0]][array[i][1]] = array[i][2]
    return np.array(recovery_res)