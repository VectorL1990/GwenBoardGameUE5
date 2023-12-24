# -*- coding: utf-8 -*-

datas={"OldManInMountain": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"Burst": {"launchType": "auto", "countDown": 3, "availableTimes": 1, "selfTarget": False, "prereqs":{"beingHurt": [1]}, "effectValues": {"values":[1], "type":"cross"}},
			"FenceDevour": {"launchType": "assign", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"values":[0]}}
		}
	}, 
	"DustyRanger": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"Hurt": {"launchType": "assign", "countDown": 1, "availableTimes": 3, "selfTarget": False, "prereqs":{}, "effectValues": {"values": [1], "stipulation": 3}}
		}
	}, 
	"ThirteenSuicideSquad": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"Hurt": {"launchType": "assign", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"values": [1], "stipulation": 0}},
            "TagByHurt": {"launchType": "targetPassive", "countDown": 0, "availableTimes": 1, "selfTarget": False, "prereqs":{"triggerEffectType": "hurt", "triggerValue": 1}, "effectValues": {"tag": "forbear", "isRoundEnd": False}}
		}
	}, 
	"BlackGuard": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"FormationVShoot": {"launchType": "assign", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"value": 2, "distance": 0}}
		}
	}, 
	"PhoenixChariot": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"FormationLine3": {"launchType": "assignImmediate", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{"gap": 0}, "effectValues": {"values":[0]}}
		}
	}, 
	"MasonOnCliff": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
		"effects":{
			"AssignTag": {"launchType": "assign", "countDown": 1, "availableTimes": 1, "selfTarget": False, "prereqs":{}, "effectValues": {"values": [], "state": "spy", "last": 3}}
		}
	}}

allDatas = {
	'allCards':datas,
}