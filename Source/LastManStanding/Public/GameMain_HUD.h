// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameMain_HUD.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AGameMain_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameMain_HUD();
	virtual void BeginPlay() override;

public:
	TSharedPtr<class SWidget> GetChatInputTextObject();
	void AddChatMessage(const FString& Message);
	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);

private:
	TSubclassOf<class UUW_GameMain> MainUIClass;
	class UUW_GameMain* MainUIObject;

private:
	bool CheckUIObject();
	bool CreateUIObject();
};
