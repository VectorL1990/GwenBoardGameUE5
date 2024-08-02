// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheckPrereqFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GWENBOARDGAMECLIENT_API UCheckPrereqFunctionLibrary : public UBlueprintFunctionLibrary
{
				GENERATED_BODY()
public:
				static bool CheckPrereqRule(FString prereq, int32 launchX, int32 launchY, int32 targetX, int32 targetY, int32 launchCamp, FString prereqCampType);
	
};
