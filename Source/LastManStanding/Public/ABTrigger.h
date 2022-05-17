// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ABTrigger.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AABTrigger : public ATriggerBox
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
		void TriggerWithCharacter(AActor* TriggerCharacter);
};
