// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Mission.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_Mission : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* Mission;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* DefaultMission;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* PlayerMission;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* KillingMission;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* PlayerKillingMission;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* DefaultKillingMission;

protected:
	virtual void NativeOnInitialized() override;
	//virtual void NativeConstruct() override;
public:

	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);


	
};
