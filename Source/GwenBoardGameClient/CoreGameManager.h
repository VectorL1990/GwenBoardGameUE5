// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreGameManager.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCoreGameManager : public UObject
{
GENERATED_BODY()
private:
    static UCoreGameManager* instance;

public:
    static UCoreGameManager* GetInstance();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AActor> testActorClass;
	
};
