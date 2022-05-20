// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Gameover.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_Gameover : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* PlayerNickName;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetWinnerName(const FString& WinnerName);
};
