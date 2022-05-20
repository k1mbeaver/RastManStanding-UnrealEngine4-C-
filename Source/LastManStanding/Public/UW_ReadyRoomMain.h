// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ReadyRoomMain.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_ReadyRoomMain : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta = (BindWidget))
		class UUW_ReadyRoom* WB_ReadyRoom; // 이게 널값이었다?

public:
	void VisiblePlayButton();
	void VisibleJoinButton();
	void HiddenIPBox();
	FString GetTextBox();
	FString GetIp();
	//void VisibleJoinButton();
};
