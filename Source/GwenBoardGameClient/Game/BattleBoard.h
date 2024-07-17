// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "BattleBoard.generated.h"




UCLASS()
class GWENBOARDGAMECLIENT_API ABattleBoard : public AActor
{
	GENERATED_BODY()
	
public:	
				// Sets default values for this actor's properties
				ABattleBoard();

protected:
				// Called when the game starts or when spawned
				virtual void BeginPlay() override;

public:	
				// Called every frame
				virtual void Tick(float DeltaTime) override;

				void LaunchSkill(int32 launchX, int32 launchY, int32 targetX, int32 targetY, FEffectInfo& effectInfo);

				void SimulationMove();

				void StateCoding();

    UPROPERTY()
    TMap<int32, FBoardRow> boardRows;

				UPROPERTY()
				TMap<int32, FString> boardCardUids;
};
