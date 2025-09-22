// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSGuideActor.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API APSGuideActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSGuideActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void Init(const FVector& startPt, const FVector& targetPts);

	UFUNCTION(BlueprintCallable)
	void CalculateInitParams(FVector startPt, FVector endPt);

	float curLiveTime = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> guidePoints;

	UPROPERTY(EditDefaultsOnly)
	float guideLengthStandard;

	UPROPERTY(EditDefaultsOnly)
	float guideHeightStandard;

	UPROPERTY(EditDefaultsOnly)
	float guideTimeStandard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float guideLength;

	UPROPERTY(BlueprintReadWrite)
	float guideHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float resetTime;

	float lifeTime;
};
