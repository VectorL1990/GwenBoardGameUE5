# -*- coding: utf-8 -*-
#
"""
"""
from KBEDebug import *
import d_entities

def onInit():
	"""
	init Dialog
	"""
	return
				
def onGossip(key, avatar, talker):
	INFO_MSG("onGossip: avatar:%i, talker=%i, key:%s." % (avatar.id, talker.id, key))
	
	if d_entities.datas.get(talker.uid, None) is None:
		return
		
	return

