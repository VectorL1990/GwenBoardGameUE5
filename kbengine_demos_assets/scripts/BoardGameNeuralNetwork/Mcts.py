import numpy as np
import copy
from config import CONFIG
import GlobalConst

def softmax(x):
	probs = np.exp(x - np.max(x))
	probs /= np.sum(probs)
	return probs

class TreeNode(object):
	def __init__(self, parent, prior_p):
		self.parent = parent()
		# children is a dictionary which keys are 
		self.children = {}
		self.nVisits = 0
		self.Q = 0
		self.U = 0
		self.P = prior_p

	def Expand(self, actionPriors):
		for action, prob in actionPriors:
			if action not in self.children:
				self.children[action] = TreeNode(self, prob)

	def Select(self, c_puct):
		maxQU = 0
		maxQUAction = ""
		maxQUNode = self
		for nodeKey, treeNode in self.children:
			QU = treeNode.GetValue(c_puct)
			if (QU >= maxQU):
				maxQU = QU
				maxQUAction = nodeKey
				maxQUNode = treeNode
		return maxQUAction, maxQUNode
		#return max(self.children.items(), key = lambda action_node:action_node[1].GetValue(c_puct))
	
	def GetValue(self, c_puct):
		self.U = (c_puct*self.P*np.sqrt(self.parent.nVisits)/(1 + self.nVisits))
		return self.Q + self.U
	
	def Update(self, QValue):
		self.nVisits = 1
		self.Q = QValue
		if self.parent:
			self.UpdateParentQValueRecursive(QValue)

	def UpdateLeafQValue(self, leafValue):
		self.nVisits += 1
		self.Q = (self.Q - leafValue) / self.nVisits

	def UpdateParentQValueRecursive(self, leafValue):
		if self.parent:
			self.parent.UpdateParentQValueRecursive(leafValue)
		self.UpdateLeafQValue(leafValue)

	def IsLeaf(self):
		return self.children == {}
	
	def IsRoot(self):
		return self.parent is None
	


class MCTS(object):
	def __init__(self, policyValueEvalFunc, c_puct = 5, nPlayout = 2000):
		self.root = TreeNode(None, 1.0)
		self.policyEvalFunc = policyValueEvalFunc
		self.c_puct = c_puct
		self.nPlayout = nPlayout

	def DoSimulationMove(self, copyBoard):
		node = self.root
		while True:
			if node.IsLeaf():
				break
			action, node = node.Select(self.c_puct)
			copyBoard.DoMove(action)

		# policy and value evaluation function returns a tuple which contains both action id and action prob
		actionProbTuples, leafValue = self.policyEvalFunc(copyBoard)

		end, winner = copyBoard.CheckGameEnd()
		if not end:
			node.Expand(actionProbTuples)
		else:
			if winner == -1:
				leafValue = 0.0
			else:
				leafValue = (1.0 if winner == copyBoard.GetCurrentPlayer() else -1.0)

		# leafValue should be positive ???
		node.Update(leafValue)

	def GetMoveProbs(self, boardState, temp = 1e-3):
		for n in range(self.nPlayout):
			stateCopy = copy.deepcopy(boardState)
			self.DoSimulationMove(stateCopy)

		actVisits = [(act, node.nVisits) for act, node in self.root.children.items()]
		acts, visits = zip(*actVisits)
		actProbs = softmax(1.0 / temp * np.log(np.array(visits) + 1e-10))
		return acts, actProbs

	def UpdateWithMove(self, lastMove):
		if lastMove in self.root.children:
			self.root = self.root.children[lastMove]
			self.root.parent = None
		else:
			self.root = TreeNode(None, 1.0)


class MCTSPlayer(object):
	def __init__(self, policyValueEvalFunc, cPuct=5, nPlayout=2000, isSelfPlay=0):
		self.mcts = MCTS(policyValueEvalFunc, cPuct, nPlayout)
		self.isSelfPlay = isSelfPlay

	def ResetPlayer(self):
		self.mcts.UpdateWithMove(-1)

	def GetAction(self, boardState, temp=1e-3, returnProbs=False):
		moveProbs = np.zeros(GlobalConst.totalMoveNb)

		acts, probs = self.mcts.GetMoveProbs(boardState, temp)
		moveProbs[list(acts)] = probs
		if self.isSelfPlay:
			move = np.random.choice(
				acts,
				p = 0.75*probs + 0.25*np.random.dirichlet(0.2 * np.ones(len(probs)))
			)
			self.mcts.UpdateWithMove(move)
		else:
			move = np.random.choice(acts, p=probs)
			self.mcts.UpdateWithMove(-1)
		if returnProbs:
			return move, moveProbs
		else:
			return move

