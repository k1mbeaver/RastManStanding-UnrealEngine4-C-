// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_PC.h"
#include "UW_Title.h"
#include "ABGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ABGameInstance.h"

ATitle_PC::ATitle_PC()
{
	static ConstructorHelpers::FClassFinder<UUW_Title> WB_Title(TEXT("WidgetBlueprint'/Game/Widget/WB_Title'"));
	if (WB_Title.Succeeded())
	{
		TitleUIClass = WB_Title.Class;
	}
}

void ATitle_PC::BeginPlay()
{
	Super::BeginPlay();

	TitleUIObject = CreateWidget<UUW_Title>(this, TitleUIClass);
	TitleUIObject->AddToViewport();
	TitleUIObject->SetVisibility(ESlateVisibility::Hidden);

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	FTimerHandle StartAnimDelayTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartAnimDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			TitleUIObject->SetVisibility(ESlateVisibility::Visible);
			TitleUIObject->StartAnim();
		}), 1.0f, false);
}

void ATitle_PC::CreateServer()
{
	// ���⼭ CreateServer �ϸ� ����:0(Ŭ���̾�Ʈ), 1(����) ���� 1�� ����
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();
	MyGI->SetIsServer("Player", 1);
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), true, ((FString)("Listen")));
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"), true, ((FString)("Listen")));
	UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"));
}

void ATitle_PC::JoinServer()
{
	// ���⼭ JoinServer �ϸ� ����:0(Ŭ���̾�Ʈ), 1(����) ���� 0�� ����
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();
	MyGI->SetIsServer("Player", 0);

	//UGameplayStatics::OpenLevel(GetWorld(), FName(TitleUIObject->GetIp())); // Level �̸� ��� IP �ּ�
	UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"));
}
