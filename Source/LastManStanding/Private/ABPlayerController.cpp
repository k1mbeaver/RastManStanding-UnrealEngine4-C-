// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "GameMain_HUD.h"
#include "Kismet/GameplayStatics.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 액션 키 바인딩
	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

}

void AABPlayerController::SendMessage(const FText& Text)
{
	// GameInstance에 저장해두었던 내 닉네임.
	// 게시글로는 안 적었다. 이거까지 설명하진 않겠다.
	//UMyGameInstance* MyGI = GetGameInstance<UMyGameInstance>(); // GameInstance를 직접 만들어서 사용
	//if (MyGI) 나중에 GameInstance 만들어서 json 적용한 다음 사용하자
//   {
		//FString UserName = MyGI->GetUserName(); 이것도
		//FString Message = FString::Printf(TEXT("%s : %s"), *UserName, *Text.ToString());

		//CtoS_SendMessage(Message); // 서버에서 실행될 수 있도록 보낸다.
//   }
	FString Message = FString::Printf(TEXT("%s"), *Text.ToString());

	CtoS_SendMessage(Message); // 서버에서 실행될 수 있도록 보낸다.
}

void AABPlayerController::FocusChatInputText()
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(HUD->GetChatInputTextObject());

	SetInputMode(InputMode);
}

void AABPlayerController::FocusGame()
{
	SetInputMode(FInputModeGameOnly());
}


void AABPlayerController::CtoS_SendMessage_Implementation(const FString& Message)
{
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_SendMessage(Message);
		}
	}
}

void AABPlayerController::StoC_SendMessage_Implementation(const FString& Message)
{
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;

	HUD->AddChatMessage(Message);
}
