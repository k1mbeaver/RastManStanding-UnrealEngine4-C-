// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "GameMain_HUD.h"
#include "ABAnimInstance.h"
#include "ABCharacter.h"
#include "Kismet/GameplayStatics.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	ABPawn = aPawn;
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

	InputComponent->BindAxis(TEXT("UpDown"), this, &AABPlayerController::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &AABPlayerController::LeftRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AABPlayerController::LookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &AABPlayerController::Turn);

	// �׼�

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABPlayerController::Jump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABPlayerController::Attack);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABPlayerController::Run);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABPlayerController::StopRun);
}

// ������ �Լ�

void AABPlayerController::UpDown(float NewAxisValue)
{
	//APawn* const myPawn = GetPawn();
	ABPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AABPlayerController::LeftRight(float NewAxisValue)
{
	//APawn* const myPawn = GetPawn();
	ABPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}


void AABPlayerController::LookUp(float NewAxisValue)
{
	//APawn* const myPawn = GetPawn();
	ABPawn->AddControllerPitchInput(NewAxisValue);
}

void AABPlayerController::Turn(float NewAxisValue)
{
	//APawn* const myPawn = GetPawn();
	ABPawn->AddControllerYawInput(NewAxisValue);
}

// �׼� �Լ�

void AABPlayerController::Jump()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	ABCharacter = Cast <AABCharacter>(ABPawn);
	ABCharacter->bPressedJump = true;
	ABCharacter->JumpKeyHoldTime = 0.0f;
}

void AABPlayerController::Attack()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	ABCharacter = Cast <AABCharacter>(ABPawn);
	if (ABCharacter->IsAttacking) return;

	ABCharacter->ABAnim->PlayAttackMontage();

	ABCharacter->IsAttacking = true;

	//MyAttack.Broadcast();
}


void AABPlayerController::Run()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	ABCharacter = Cast <AABCharacter>(ABPawn);
	ABCharacter->GetCharacterMovement()->MaxWalkSpeed *= ABCharacter->fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerRun!"));
	//MyRun.Broadcast();
}

void AABPlayerController::StopRun()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	ABCharacter = Cast <AABCharacter>(ABPawn);
	ABCharacter->GetCharacterMovement()->MaxWalkSpeed /= ABCharacter->fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerStopRun!"));
	//MyStopRun.Broadcast();
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
/*
void AABPlayerController::CtoS_Attack_Implementation()
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_Attack();
		}
	}
}

void AABPlayerController::StoC_Attack_Implementation()
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	APawn* MyPawn = GetPawn();
	AABCharacter* MyCharacter = Cast<AABCharacter>(MyPawn);
	if (MyCharacter == nullptr) return;

	MyCharacter->AttackCheck();
}
*/

// ���ø�����Ʈ

void AABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABPlayerController, ABCharacter);
	DOREPLIFETIME(AABPlayerController, ABPawn);
}