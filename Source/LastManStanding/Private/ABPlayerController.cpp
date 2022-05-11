// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "GameMain_HUD.h"
#include "ABAnimInstance.h"
#include "ABCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AABPlayerController::AABPlayerController()
{
	//fSprintSpeedMultiPlayer = 3.0f; // ó���� 3.0, �̼Ǽ���� 2.5 2.0 1.5 �ܰ�� �پ�� 
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	myPawn = aPawn;
	myCharacter = Cast<AABCharacter>(myPawn);

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

	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	// Ű ���ε�

	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

	// ������

	//InputComponent->BindAxis(TEXT("UpDown"), this, &AABPlayerController::UpDown);
	//InputComponent->BindAxis(TEXT("LeftRight"), this, &AABPlayerController::LeftRight);
	//InputComponent->BindAxis(TEXT("LookUp"), this, &AABPlayerController::LookUp);
	//InputComponent->BindAxis(TEXT("Turn"), this, &AABPlayerController::Turn);

	// �׼�

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABPlayerController::Jump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABPlayerController::Attack);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABPlayerController::Run);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABPlayerController::StopRun);
}

// ������ �Լ�
/*
void AABPlayerController::UpDown(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AABPlayerController::LeftRight(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}


void AABPlayerController::LookUp(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddControllerPitchInput(NewAxisValue);
}

void AABPlayerController::Turn(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddControllerYawInput(NewAxisValue);
}
*/
// �׼� �Լ�


void AABPlayerController::Jump()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//ABCharacter = Cast <AABCharacter>(ABPawn);
	myCharacter->bPressedJump = true;
	myCharacter->JumpKeyHoldTime = 0.0f;
}

// �޸���
void AABPlayerController::Run()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	//myCharacter->GetCharacterMovement()->MaxWalkSpeed *= myCharacter->fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerRun!"));

	CtoS_Run(myCharacter);
	//MyRun.Broadcast();
}

void AABPlayerController::CtoS_Run_Implementation(AABCharacter* ClientCharacter)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_Run(ClientCharacter);
		}
	}
}

void AABPlayerController::StoC_Run_Implementation(AABCharacter* ClientCharacter)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.

	if (ClientCharacter == nullptr) return;

	ClientCharacter->GetCharacterMovement()->MaxWalkSpeed *= ClientCharacter->fSprintSpeedMultiPlayer;
	
}

void AABPlayerController::StopRun()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	//myCharacter->GetCharacterMovement()->MaxWalkSpeed /= myCharacter->fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerStopRun!"));

	CtoS_StopRun(myCharacter);
	//MyStopRun.Broadcast();
}

void AABPlayerController::CtoS_StopRun_Implementation(AABCharacter* ClientCharacter)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_StopRun(ClientCharacter);
		}
	}
}

void AABPlayerController::StoC_StopRun_Implementation(AABCharacter* ClientCharacter)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	if (ClientCharacter == nullptr) return;

	ClientCharacter->GetCharacterMovement()->MaxWalkSpeed /= ClientCharacter->fSprintSpeedMultiPlayer;
}
// ä��

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

// ä��(����)

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

// ����(����)

void AABPlayerController::Attack()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	UAnimMontage* playPunch;

	// �� �κп��� ���� ��Ÿ�ָ� �����Ѵ�.
	playPunch = myCharacter->ABAnim->GetAttackMontage();
	myCharacter->AttackPower = 100.0f;
	myCharacter->ABAnim->PlayAttackMontage(playPunch);
	myCharacter->IsAttacking = true;

	CtoS_Attack(myCharacter, playPunch);
	//MyStopRun.Broadcast();
}

void AABPlayerController::CtoS_Attack_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_Attack(ClientCharacter, playPunch);
		}
	}
}

void AABPlayerController::StoC_Attack_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	if (ClientCharacter->IsAttacking) return;

	ClientCharacter->AttackPower = 100.0f;
	ClientCharacter->ABAnim->PlayAttackMontage(playPunch);
	ClientCharacter->IsAttacking = true;

	/*
	if (ClientCharacter->DeathCharacter != NULL)
	{
		ClientCharacter->DeathCharacter->ABAnim->SetDeadAnim();
		//ClientCharacter->DeathCharacter->SetCharacterState(ECharacterState::DEAD);
		ClientCharacter->DeathCharacter->SetActorEnableCollision(false);
	    ClientCharacter->DeathCharacter = NULL;
	}
	*/
}

// ���ø�����Ʈ

void AABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABPlayerController, myCharacter);
	DOREPLIFETIME(AABPlayerController, myPawn);
}

