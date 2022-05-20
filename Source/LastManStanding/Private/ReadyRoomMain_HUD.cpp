// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyRoomMain_HUD.h"
#include "UW_ReadyRoomMain.h"
#include "ABGameInstance.h"

AReadyRoomMain_HUD::AReadyRoomMain_HUD()
{
	static ConstructorHelpers::FClassFinder<UUW_ReadyRoomMain> WB_ReadyRoomMain(TEXT("WidgetBlueprint'/Game/Widget/WB_ReadyRoomMain'"));
	if (WB_ReadyRoomMain.Succeeded())
	{
		ReadyRoomMainUIClass = WB_ReadyRoomMain.Class;
	}
}

void AReadyRoomMain_HUD::BeginPlay()
{
	Super::BeginPlay();

	CheckUIObject(); // 시작하면 UI를 생성한다.

	// 여기서 플레이어가 서버 이면 Play 버튼 활성화
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();
	if (MyGI->GetIsServer("Player") == 1)
	{
		HiddenIPBox();
		VisiblePlayButton();
	}

	else if (MyGI->GetIsServer("Player") == 0)
	{
		VisibleJoinButton();
	}
}

bool AReadyRoomMain_HUD::CheckUIObject()
{
	if (ReadyRoomMainUIObject == nullptr) // UI가 없다면 생성.
	{
		return CreateUIObject();
	}
	return true; // 있다면 True.
}

bool AReadyRoomMain_HUD::CreateUIObject()
{
	if (ReadyRoomMainUIClass)
	{
		ReadyRoomMainUIObject = CreateWidget<UUW_ReadyRoomMain>(GetOwningPlayerController(), ReadyRoomMainUIClass);
		if (ReadyRoomMainUIObject)
		{
			ReadyRoomMainUIObject->AddToViewport();
			return true; // 만들었다면 true.
		}
	}
	return false; // 못 만들었다면 false.
}

void AReadyRoomMain_HUD::VisiblePlayButton()
{
	ReadyRoomMainUIObject->VisiblePlayButton();
}

void AReadyRoomMain_HUD::VisibleJoinButton()
{
	ReadyRoomMainUIObject->VisibleJoinButton();
}

void AReadyRoomMain_HUD::HiddenIPBox()
{
	ReadyRoomMainUIObject->HiddenIPBox();
}

FString AReadyRoomMain_HUD::GetTextBox()
{
	return ReadyRoomMainUIObject->GetTextBox();
}

FString AReadyRoomMain_HUD::GetIp()
{
	return ReadyRoomMainUIObject->GetIp();
}
/*
void AReadyRoomMain_HUD::VisibleJoinButton()
{
	ReadyRoomMainUIObject->VisibleJoinButton();
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
*/