// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Render/PSGuideActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APSGuideActor::APSGuideActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APSGuideActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APSGuideActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (curLiveTime >= resetTime)
	{
		ConditionalBeginDestroy();
	}
	else
	{
		curLiveTime += DeltaTime;
	}
}

void APSGuideActor::Init_Implementation(const FVector& startPt, const FVector& targetPts)
{
}

void APSGuideActor::CalculateInitParams(FVector startPt, FVector endPt)
{
	FVector offset = endPt - startPt;
	float length = offset.Length();
	guideLength = length;
	guideHeight = (length / guideLengthStandard) * guideHeightStandard;
	resetTime = (length / guideLengthStandard) * guideTimeStandard;
	lifeTime = resetTime * 0.9;

	FVector offsetSegment = offset / 3.0;
	offsetSegment.Z = 0.0;
	FVector p1 = endPt;
	FVector p2 = startPt + offsetSegment * 2.0;
	p2.Z = guideHeight;

	FVector p3 = startPt + offsetSegment;
	p3.Z = guideHeight;

	FVector p4 = startPt;

	guidePoints.Empty();
	guidePoints.Add(p1);
	guidePoints.Add(p2);
	guidePoints.Add(p3);
	guidePoints.Add(p4);
}

