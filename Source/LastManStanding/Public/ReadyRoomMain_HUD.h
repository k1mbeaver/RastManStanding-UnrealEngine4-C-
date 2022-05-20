// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ReadyRoomMain_HUD.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AReadyRoomMain_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AReadyRoomMain_HUD();
	virtual void BeginPlay() override;

public:
	/* // 할거면 비슷하게 해보자
	TSharedPtr<class SWidget> GetChatInputTextObject();
	void AddChatMessage(const FString& Message);
	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);
	*/
	void VisiblePlayButton();
	void VisibleJoinButton();
	void HiddenIPBox();

	FString GetTextBox();
	FString GetIp();
private:
	TSubclassOf<class UUW_ReadyRoomMain> ReadyRoomMainUIClass;
	class UUW_ReadyRoomMain* ReadyRoomMainUIObject;

private:
	bool CheckUIObject();
	bool CreateUIObject();
};
