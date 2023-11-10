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
								basicGameMode->ReqCreateAccount(playerName, password);
				}
				else if (buttonName == "Login")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ABasicGameMode* basicGameMode = Cast<ABasicGameMode>(gameMode);
								basicGameMode->ReqLogin(playerName, password);
				}
				else if (buttonName == "ReqTest")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ALoginGameModeBase* loginGameMode = Cast<ALoginGameModeBase>(gameMode);
								loginGameMode->ReqTest();
				}
				else if (buttonName == "EnterRoom")
				{
								AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
								ALoginGameModeBase* loginGameMode = Cast<ALoginGameModeBase>(gameMode);
								loginGameMode->ReqEnterRoom();
				}
}

void ULoginWidget::ChangePlayerName(FString name)
{
				playerName = name;
}

void ULoginWidget::ChangePassword(FString pwd)
{
				password = pwd;
}
