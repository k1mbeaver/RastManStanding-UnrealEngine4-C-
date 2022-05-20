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

	CheckUIObject(); // �����ϸ� UI�� �����Ѵ�.

	// ���⼭ �÷��̾ ���� �̸� Play ��ư Ȱ��ȭ
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
	if (ReadyRoomMainUIObject == nullptr) // UI�� ���ٸ� ����.
	{
		return CreateUIObject();
	}
	return true; // �ִٸ� True.
}

bool AReadyRoomMain_HUD::CreateUIObject()
{
	if (ReadyRoomMainUIClass)
	{
		ReadyRoomMainUIObject = CreateWidget<UUW_ReadyRoomMain>(GetOwningPlayerController(), ReadyRoomMainUIClass);
		if (ReadyRoomMainUIObject)
		{
			ReadyRoomMainUIObject->AddToViewport();
			return true; // ������ٸ� true.
		}
	}
	return false; // �� ������ٸ� false.
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
	// BeginPlay()�� ����Ǳ� ���� �� �Լ��� ���� ���� �� ���� �ִ�.
	// UI�� ����� ���� UI�� �����ϸ� ������ ���� ������ �˻��Ѵ�.
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