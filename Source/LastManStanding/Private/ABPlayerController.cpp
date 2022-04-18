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

	// �׼� Ű ���ε�
	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

}

void AABPlayerController::SendMessage(const FText& Text)
{
	// GameInstance�� �����صξ��� �� �г���.
	// �Խñ۷δ� �� ������. �̰ű��� �������� �ʰڴ�.
	//UMyGameInstance* MyGI = GetGameInstance<UMyGameInstance>(); // GameInstance�� ���� ���� ���
	//if (MyGI) ���߿� GameInstance ���� json ������ ���� �������
//   {
		//FString UserName = MyGI->GetUserName(); �̰͵�
		//FString Message = FString::Printf(TEXT("%s : %s"), *UserName, *Text.ToString());

		//CtoS_SendMessage(Message); // �������� ����� �� �ֵ��� ������.
//   }
	FString Message = FString::Printf(TEXT("%s"), *Text.ToString());

	CtoS_SendMessage(Message); // �������� ����� �� �ֵ��� ������.
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
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
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
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;

	HUD->AddChatMessage(Message);
}
