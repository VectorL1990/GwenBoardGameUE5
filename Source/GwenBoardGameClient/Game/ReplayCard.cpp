// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ReplayCard.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "../Base/GwenBoardGameInstance.h"

// Sets default values
AReplayCard::AReplayCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReplayCard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AReplayCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AReplayCard::Init(FString inCardName, uint8 sectionNb, int32 hp, int32 defence, int32 cd, int32 available)
{
	//NotifyInit(sectionNb);
	cardName = inCardName;
	InitReplayCard(sectionNb, inCardName);
	cardWidgetComponent = GetComponentByClass<UWidgetComponent>();
	UCardWidget3D* widget = Cast<UCardWidget3D>(cardWidgetComponent->GetWidget());
	widget->NotifyInit();
	widget->SetupHpDefence(hp, defence, cd, available);
}

void AReplayCard::NotifyInit_Implementation(uint8 camp)
{

}

void AReplayCard::InitReplayCard(uint8 sectionNb, FString cardName)
{
	UMaterialInstanceDynamic* mi = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, matParent);
	if (sectionNb == 0)
	{
		FVector4 baseColor(0.5, 0.0, 0.0, 1.0);
		mi->SetVectorParameterValue(FName("BaseColor"), baseColor);
	}
	else
	{
		FVector4 baseColor(0.0, 0.2, 0.5, 1.0);
		mi->SetVectorParameterValue(FName("BaseColor"), baseColor);
	}
	UGameInstance* gi = UGameplayStatics::GetGameInstance(this);
	UGwenBoardGameInstance* gameGI = Cast<UGwenBoardGameInstance>(gi);
	mi->SetTextureParameterValue("BaseTexture", gameGI->allCardInfos[cardName].texture);
	UStaticMeshComponent* staticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	staticMeshComponent->SetMaterial(0, mi);
}

