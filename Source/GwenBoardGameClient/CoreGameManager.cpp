// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameManager.h"

UCoreGameManager* UCoreGameManager::instance = nullptr;

UCoreGameManager* UCoreGameManager::GetInstance()
{
				if (instance == nullptr)
				{
								instance = NewObject<UCoreGameManager>();
				}
				return instance;
}

