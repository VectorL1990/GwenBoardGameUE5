import numpy as np
import copy
from config import CONFIG

def softmax(x):
	probs = np.exp(x - np.max(x))
	probs /= np.sum(probs)
	return probs

class TreeNode(object):
	def __init__(self, parent, prior_p):
		self.parent = parent()
		# children is a dictionary which keys are 
		self.children = {}
		self.n_visits = 0
		self.Q = 0
		self.U = 0
		self.P = prior_p

	def expand(self, action_priors):
		for action, prob in action_priors:
			if action not in self.children:
				self.children[action] = TreeNode(self, prob)

	def select(self, c_puct):
		return max(self.children.items(), key = lambda action_node:action_node[1].get_value(c_puct))
	
	def get_value(self, c_puct):
		self.U = (c_puct*self.P*np.sqrt(self.parent.n_visits)/(1 + self.n_visits))
		return self.Q + self.U
	
	def update(self, leaf_value):
		self.n_visits += 1
		self.Q += 1.0*(leaf_value - self.Q) / self.n_visits

	def update_recursive(self, leaf_value):
		if self.parent:
			self.parent.update_recursive(-leaf_value)
		self.update(leaf_value)

	def is_leaf(self):
		return self.children == {}
	
	def is_root(self):
		return self.parent is None
	


class MCTS(object):
	def __init__(self, policy_value_eval_func, c_puct = 5, n_playout = 2000):
		self.root = TreeNode(None, 1.0)
		self.policy_eval_func = policy_value_eval_func
		self.c_puct = c_puct
		self.n_playout = n_playout

	def playout(self, game):
		node = self.root
		while True:
			if node.is_leaf():
				break
			action, node = node.select(self.c_puct)
			sdf

class MCTSPlayer(object):
	def __init__(self):
		self.mcts = MCTS()

	def GetAction(self, boardState):
		sdf

