import random
import GlobalConst

def BeingHurt(stateList, targetX, targetY, launchX, launchY, modifyValue, effectInfo):
    if modifyValue >= effectInfo["values"][0]:
        return True
    else:
        return False
    
checkPassivePrereqRule = {
	'beingHurt': BeingHurt,
}
