// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "UI/CardWidget3D.h"
#include "ReplayCard.generated.h"

UCLASS()
class GWENBOARDGAMECLIENT_API AReplayCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplayCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* cardWidgetComponent;


	void Init(FString cardName, uint8 sectionNb, int32 hp, int32 defence);

	UFUNCTION(BlueprintNativeEvent)
	void NotifyInit(uint8 camp);
};
