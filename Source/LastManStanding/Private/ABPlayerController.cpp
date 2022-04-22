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

	// 키 바인딩

	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

	// 움직임

	InputComponent->BindAxis(TEXT("UpDown"), this, &AABPlayerController::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &AABPlayerController::LeftRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AABPlayerController::LookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &AABPlayerController::Turn);

	// 액션

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABPlayerController::Jump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABPlayerController::Attack);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABPlayerController::Run);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABPlayerController::StopRun);
}

// 움직임 함수

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

// 액션 함수

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

// 채팅

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

// 채팅(서버)

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

// 공격(서버)
/*
void AABPlayerController::CtoS_Attack_Implementation()
{
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
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
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.
	APawn* MyPawn = GetPawn();
	AABCharacter* MyCharacter = Cast<AABCharacter>(MyPawn);
	if (MyCharacter == nullptr) return;

	MyCharacter->AttackCheck();
}
*/

// 리플리케이트

void AABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABPlayerController, ABCharacter);
	DOREPLIFETIME(AABPlayerController, ABPawn);
}