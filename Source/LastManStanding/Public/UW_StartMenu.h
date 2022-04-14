// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StartMenu.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_StartMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* ShowUp;
	UPROPERTY(Meta = (BindWidget))
		class UButton* CreateServerBtn;
	UPROPERTY(Meta = (BindWidget))
		class UButton* JoinServerBtn;

public:
	void ShowUpAnim();
	
	
	UFUNCTION()
		void CreateServerHandler();
	UFUNCTION()
		void JoinServerHandler();
	
	virtual void NativeConstruct() override;
};
