# -*- coding: utf-8 -*-

allCards={"Vitellius": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"IncreaseDefenceByHurtNbTLine": {"launchType": "manual", "coolDown": 3, "availableTimes": 1, "launchGeoType": "point", "targetGeoType":"line", "aoeType":"point", "targetCamp":"oppo", "effectType":"increaseDefence", "effectAffix":"hurtNb", "effectAffixCamp":"self", "prereqTagCondition":"none", "prereqTag":"none", "prereqCampType":"none", "prereqType":"none", "passivePrereqType": "none", "values":"0"}
		}
	}, 
	"Tabora": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"IncreaseDefenceTSeperateHandCardMore2": {"launchType": "manual", "coolDown": 3, "availableTimes": 1, "launchGeoType": "point", "targetGeoType":"line", "aoeType":"point", "targetCamp":"oppo", "effectType":"increaseDefence", "effectAffix":"hurtNb", "effectAffixCamp":"self", "prereqTagCondition":"none", "prereqTag":"none", "prereqCampType":"none", "prereqType":"none", "values":"0"}
		}
	}, 
	"ThirteenSuicideSquad": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"Hurt": {"launchType": "manual", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"values": [1], "stipulation": 0}},
            "TagByHurt": {"launchType": "targetPassive", "countDown": 0, "availableTimes": 1, "selfTarget": False, "prereqs":{"triggerEffectType": "hurt", "triggerValue": 1}, "effectValues": {"tag": "forbear", "isRoundEnd": False}}
		}
	}, 
	"BlackGuard": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"FormationVShoot": {"launchType": "manual", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"value": 2, "distance": 0}}
		}
	}, 
	"PhoenixChariot": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"FormationLine3": {"launchType": "assignImmediate", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{"gap": 0}, "effectValues": {"values":[0]}}
		}
	}, 
	"MasonOnCliff": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"AssignTag": {"launchType": "manual", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"values": [], "state": "spy", "last": 3}}
		}
	}}
