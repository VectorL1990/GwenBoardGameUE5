// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/LoginWidget.h"
#include "LoginGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ULoginWidget::ClickButton(FString buttonName)
{
				if (buttonName == "CreateAccount")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ABasicGameMode* basicGameMode = Cast<ABasicGameMode>(gameMode);
								basicGameMode->ReqCreateAccount();
				}
				else if (buttonName == "Login")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ABasicGameMode* basicGameMode = Cast<ABasicGameMode>(gameMode);
								basicGameMode->ReqLogin();
				}
				else if (buttonName == "ReqTest")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ALoginGameModeBase* loginGameMode = Cast<ALoginGameModeBase>(gameMode);
								loginGameMode->ReqTest();
				}
}
