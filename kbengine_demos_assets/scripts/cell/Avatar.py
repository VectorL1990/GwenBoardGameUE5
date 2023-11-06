# -*- coding: utf-8 -*-
import KBEngine
import GlobalDefine
from KBEDebug import *
from interfaces.GameObject import GameObject

class Avatar(KBEngine.Entity,
			GameObject):
	def __init__(self):
		KBEngine.Entity.__init__(self)
		GameObject.__init__(self) 
		
		# 设置每秒允许的最快速度, 超速会被拉回去
		self.topSpeed = self.moveSpeed + 5.0
		# self.topSpeedY = 10.0
		
		# 如果为7说明在UE4地图中，那么为了配合demo的移动速度，我们将限制设置得更大一些
		if self.spaceUType == 7:
			self.topSpeed = 0
