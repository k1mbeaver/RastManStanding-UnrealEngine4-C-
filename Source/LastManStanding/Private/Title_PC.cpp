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
	// 여기서 CreateServer 하면 서버:0(클라이언트), 1(서버) 값을 1로 설정
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();
	MyGI->SetIsServer("Player", 1);
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), true, ((FString)("Listen")));
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"), true, ((FString)("Listen")));
	UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"));
}

void ATitle_PC::JoinServer()
{
	// 여기서 JoinServer 하면 서버:0(클라이언트), 1(서버) 값을 0로 설정
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();
	MyGI->SetIsServer("Player", 0);

	//UGameplayStatics::OpenLevel(GetWorld(), FName(TitleUIObject->GetIp())); // Level 이름 대신 IP 주소
	UGameplayStatics::OpenLevel(GetWorld(), FName("ReadyRoom"));
}
