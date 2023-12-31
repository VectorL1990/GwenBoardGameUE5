# -*- coding: utf-8 -*-

from enum import Enum

g_handCardAmount = 10
g_maxChangeHandCardNb = 3
g_maxHeartBeatCount = 60

class g_battleState(Enum):
	DEFAULT = 1
	WAIT_AVATAR_ENTER_ROOM = 2
	SELECT_CARD = 3
	SELECT_CARD_INTERLUDE = 4
	BATTLE = 5
	BATTLE_INTERLUDE = 6

GC_OK								= 0x000

# 技能相关
GC_SKILL_MP_NOT_ENGOUH				= 0x001		# 法力值不足
GC_SKILL_ENTITY_DEAD				= 0x002		# Entity已经死亡

# 不同demo所对应的地图
g_demoMaps = {
	b'kbengine_ue4_demo' : 7,
	b'kbengine_cocos2d_js_demo' : 6,
	b'kbengine_unity3d_demo' : 3,
	b'kbengine_ogre_demo' : 2,
	b'' : 1,
}