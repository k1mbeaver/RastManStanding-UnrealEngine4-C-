// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Title_PC.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API ATitle_PC : public APlayerController
{
	GENERATED_BODY()

public:
	
	ATitle_PC();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUW_Title> TitleUIClass;
	class UUW_Title* TitleUIObject;
	void CreateServer();
	void JoinServer();
};
