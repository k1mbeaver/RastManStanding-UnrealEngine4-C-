// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "GameMain_HUD.h"
#include "ABAnimInstance.h"
#include "ABCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AABPlayerController::AABPlayerController()
{
	//fSprintSpeedMultiPlayer = 3.0f; // 처음은 3.0, 미션수행시 2.5 2.0 1.5 단계로 줄어듬 
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

	// 키 바인딩

	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

	// 움직임

	//InputComponent->BindAxis(TEXT("UpDown"), this, &AABPlayerController::UpDown);
	//InputComponent->BindAxis(TEXT("LeftRight"), this, &AABPlayerController::LeftRight);
	//InputComponent->BindAxis(TEXT("LookUp"), this, &AABPlayerController::LookUp);
	//InputComponent->BindAxis(TEXT("Turn"), this, &AABPlayerController::Turn);

	// 액션

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABPlayerController::Jump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABPlayerController::Attack);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABPlayerController::Run);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABPlayerController::StopRun);
}

// 움직임 함수
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
// 액션 함수


void AABPlayerController::Jump()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//ABCharacter = Cast <AABCharacter>(ABPawn);
	myCharacter->bPressedJump = true;
	myCharacter->JumpKeyHoldTime = 0.0f;
}

// 달리기
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
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
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
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.

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
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
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
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.
	if (ClientCharacter == nullptr) return;

	ClientCharacter->GetCharacterMovement()->MaxWalkSpeed /= ClientCharacter->fSprintSpeedMultiPlayer;
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

void AABPlayerController::Attack()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	UAnimMontage* playPunch;

	// 이 부분에서 공격 몽타주를 실행한다.
	playPunch = myCharacter->ABAnim->GetAttackMontage();
	myCharacter->AttackPower = 100.0f;
	myCharacter->ABAnim->PlayAttackMontage(playPunch);
	myCharacter->IsAttacking = true;

	CtoS_Attack(myCharacter, playPunch);
	//MyStopRun.Broadcast();
}

void AABPlayerController::CtoS_Attack_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
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
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.
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

// 리플리케이트

void AABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABPlayerController, myCharacter);
	DOREPLIFETIME(AABPlayerController, myPawn);
}

