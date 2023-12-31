// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreCardGameManager.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGameManager : public AActor
{
    GENERATED_BODY()
private:
    static ACoreCardGameManager* instance;

public:
    // Sets default values for this actor's properties
    ACoreCardGameManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    bool Init(AActor* actor);

    static ACoreCardGameManager* GetInstance();


};
