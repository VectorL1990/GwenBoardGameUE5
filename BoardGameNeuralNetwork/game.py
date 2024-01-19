import numpy as np
import copy
import time
from config import CONFIG
from collections import deque
import random

state_list_init = [['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--'],
					['--', '--', '--', '--', '--', '--', '--', '--']]

state_deque_init = deque()

def get_move_way(card_name):
	return 0


def get_legal_moves(state_deque, cur_play_camp):
	state_list = state_deque[-1]

	moves = []

	# traverse all skill moves


	# traverse all motion moves
	for y in range(8):
		for x in rage(8):
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
						if split_str[3] == 1:
							# which means this skill requires triangle center
							if state_list[y][x - 1] != '--':
								if state_list[y-1][x] != '--':
									# which means this card is located at triangle center
									

					move_way = get_move_way(split_str[12])
					if move_way == 0:
						pass
					elif move_way == 1:
						# jump across a obstable card


