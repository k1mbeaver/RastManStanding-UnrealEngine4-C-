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

	CheckUIObject(); // �����ϸ� UI�� �����Ѵ�.
}

TSharedPtr<SWidget> AGameMain_HUD::GetChatInputTextObject()
{
	return MainUIObject->GetChatInputTextObject();
}

void AGameMain_HUD::AddChatMessage(const FString& Message)
{
	// BeginPlay()�� ����Ǳ� ���� �� �Լ��� ���� ���� �� ���� �ִ�.
	// UI�� ����� ���� UI�� �����ϸ� ������ ���� ������ �˻��Ѵ�.
	if (!CheckUIObject()) return;

	MainUIObject->AddChatMessage(Message);
}

bool AGameMain_HUD::CheckUIObject()
{
	if (MainUIObject == nullptr) // UI�� ���ٸ� ����.
	{
		return CreateUIObject();
	}
	return true; // �ִٸ� True.
}

bool AGameMain_HUD::CreateUIObject()
{
	if (MainUIClass)
	{
		MainUIObject = CreateWidget<UUW_GameMain>(GetOwningPlayerController(), MainUIClass);
		if (MainUIObject)
		{
			MainUIObject->AddToViewport();
			return true; // ������ٸ� true.
		}
	}
	return false; // �� ������ٸ� false.
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