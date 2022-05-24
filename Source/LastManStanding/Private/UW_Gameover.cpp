// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Gameover.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ABPlayerController.h"

void UUW_Gameover::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerNickName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNickName")));
}

void UUW_Gameover::SetWinnerName(const FString& WinnerName)
{
	PlayerNickName->SetText(FText::FromString(*WinnerName));
}