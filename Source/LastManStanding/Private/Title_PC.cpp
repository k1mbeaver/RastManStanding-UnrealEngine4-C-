// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_PC.h"
#include "UW_Title.h"
#include "Kismet/GameplayStatics.h"

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
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), true, ((FString)("Listen")));
}

void ATitle_PC::JoinServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("203.232.193.171")); // Level 이름 대신 IP 주소
}
