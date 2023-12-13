# -*- coding: utf-8 -*-

datas={"OldManInMountain": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
    "effects":{
      "Burst": {"launchType": "auto", "selfTarget": False, "prereqs":{"beingHurt": [1]}, "effectValues": [1]},
      "FenceDevour": {"launchType": "assign", "selfTarget": False, "prereqs":{}, "effectValues": [0]}
    }
  }, 
  "DustyRanger": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
    "effects":{
      "Hurt": {"launchType": "assign", "selfTarget": False, "prereqs":{}, "effectValues": [1]}
	  }
  }, 
  "ThirteenSuicideSquad": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': [],
    "effects":{
      "Hurt": {"launchType": "assign", "selfTarget": False, "prereqs":{}, "effectValues": [1]}
	  }
  }, 
  "BlackGuard": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': []}, 
  "PhoenixChariot": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': []}, 
  "MasonOnCliff": {'hp': 4, 'defence': 60, 'agility': 5, 'tags': []}}

allDatas = {
	'allCards':datas,
}