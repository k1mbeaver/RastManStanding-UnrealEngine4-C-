// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyRoom_PC.h"
#include "UW_ReadyRoomMain.h"
#include "ReadyRoomMain_HUD.h"
#include "ABGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AReadyRoom_PC::AReadyRoom_PC()
{
	/*
	static ConstructorHelpers::FClassFinder<UUW_ReadyRoomMain> WB_ReadyRoomMain(TEXT("WidgetBlueprint'/Game/Widget/WB_ReadyRoomMain'"));
	if (WB_ReadyRoomMain.Succeeded())
	{
		ReadyRoomUIClass = WB_ReadyRoomMain.Class;
	}
	*/
}

void AReadyRoom_PC::BeginPlay()
{
	Super::BeginPlay();

	//ReadyRoomUIObject = CreateWidget<UUW_ReadyRoomMain>(this, ReadyRoomUIClass);
	//ReadyRoomUIObject->AddToViewport();
	//ReadyRoomUIObject->SetVisibility(ESlateVisibility::Visible);

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	/*
	FTimerHandle StartAnimDelayTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartAnimDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			ReadyRoomUIObject->SetVisibility(ESlateVisibility::Visible);
		}), 1.0f, false);
	*/
}

void AReadyRoom_PC::VisiblePlay()
{
	AReadyRoomMain_HUD* HUD = GetHUD<AReadyRoomMain_HUD>();
	if (HUD == nullptr) return;

	HUD->VisiblePlayButton();
}

void AReadyRoom_PC::Play()
{
	//CtoS_Play(); // 여기서 각 클라이언트 들의 닉네임을 저장한다.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), true, ((FString)("Listen")));
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"));
}

/*
void AReadyRoom_PC::CtoS_Play_Implementation()
{
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AReadyRoom_PC* PC = Cast<AReadyRoom_PC>(OutActor);
		if (PC)
		{
			PC->StoC_Play();
		}
	}
}

void AReadyRoom_PC::StoC_Play_Implementation()
{
	// 클라이언트들에서는 Join버튼이 생성된다.
	AReadyRoomMain_HUD* HUD = GetHUD<AReadyRoomMain_HUD>();

	if (HUD == nullptr) return;

}
*/
void AReadyRoom_PC::Decide()
{
	AReadyRoomMain_HUD* HUD = GetHUD<AReadyRoomMain_HUD>();
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();

	if (HUD == nullptr) return;

	FString PlayerNickName = HUD->GetTextBox();

	MyGI->SetUserName("Player", PlayerNickName);
}

void AReadyRoom_PC::Join()
{
	AReadyRoomMain_HUD* HUD = GetHUD<AReadyRoomMain_HUD>();
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>();

	if (HUD == nullptr) return;

	FString GetServerIp = HUD->GetIp();

	UGameplayStatics::OpenLevel(GetWorld(), FName(GetServerIp));
}