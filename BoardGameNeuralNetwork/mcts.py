import numpy as np
import copy
from config import CONFIG

def softmax(x):
  probs = np.exp(x - np.max(x))
  probs /= np.sum(probs)
  return probs

class TreeNode(object):
  def __init__(self, parent, prior_p):
    self._parent = parent()
    # children is a dictionary which keys are 
    self._children = {}
    self._n_visits = 0
    self._Q = 0
    self._U = 0
    self._P = prior_p

  def expand(self, action_priors):
    