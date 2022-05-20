// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMain_HUD.h"
#include "UW_GameMain.h"

AGameMain_HUD::AGameMain_HUD()
{
	static ConstructorHelpers::FClassFinder<UUW_GameMain> WB_Main(TEXT("WidgetBlueprint'/Game/Widget/WB_GameMain'"));
	if (WB_Main.Succeeded())
	{
		MainUIClass = WB_Main.Class;
	}
}

void AGameMain_HUD::BeginPlay()
{
	Super::BeginPlay();

	CheckUIObject(); // 시작하면 UI를 생성한다.
}

TSharedPtr<SWidget> AGameMain_HUD::GetChatInputTextObject()
{
	return MainUIObject->GetChatInputTextObject();
}

void AGameMain_HUD::AddChatMessage(const FString& Message)
{
	// BeginPlay()가 실행되기 전에 이 함수가 먼저 실행 될 수도 있다.
	// UI가 생기기 전에 UI에 접근하면 오류가 나기 때문에 검사한다.
	if (!CheckUIObject()) return;

	MainUIObject->AddChatMessage(Message);
}

bool AGameMain_HUD::CheckUIObject()
{
	if (MainUIObject == nullptr) // UI가 없다면 생성.
	{
		return CreateUIObject();
	}
	return true; // 있다면 True.
}

bool AGameMain_HUD::CreateUIObject()
{
	if (MainUIClass)
	{
		MainUIObject = CreateWidget<UUW_GameMain>(GetOwningPlayerController(), MainUIClass);
		if (MainUIObject)
		{
			MainUIObject->AddToViewport();
			return true; // 만들었다면 true.
		}
	}
	return false; // 못 만들었다면 false.
}

void AGameMain_HUD::SetPlayerMissionClear(int nMissionClear)
{
	MainUIObject->SetPlayerMissionClear(nMissionClear);
}

void AGameMain_HUD::PlayerKillingMissionAppear()
{
	MainUIObject->PlayerKillingMissionAppear();
}

void AGameMain_HUD::SetPlayerKillingPoint(int nKillingPoint)
{
	MainUIObject->SetPlayerKillingPoint(nKillingPoint);
}

void AGameMain_HUD::SetWinnerName(const FString& WinnerName)
{
	MainUIObject->SetWinnerName(WinnerName);
}

void AGameMain_HUD::VisibleGameover()
{
	MainUIObject->VisibleGameover();
}