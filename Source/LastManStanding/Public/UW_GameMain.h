// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_GameMain.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_GameMain : public UUserWidget
{
	GENERATED_BODY()

public:
	TSharedPtr<class SWidget> GetChatInputTextObject();
	void AddChatMessage(const FString& Message);
	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);
	void SetWinnerName(const FString& WinnerName);
	void VisibleGameover();

private:
	UPROPERTY(Meta = (BindWidget))
		class UUW_Gameover* WB_GameOver;

	UPROPERTY(Meta = (BindWidget))
		class UUW_Chat* WB_Chat;

	UPROPERTY(Meta = (BindWidget))
		class UUW_Mission* WB_UserMission;
};
