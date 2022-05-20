// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "ABAIController.h"
#include "ABPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMain_HUD.h"
#include <random>

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_EASYMODEL(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_EASYMODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_EASYMODEL.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Mannequin/Animations/PersonAnimBlueprint"));
	if (PLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	SetControlMode(EControlMode::Player);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	IsAttacking = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	AttackRange = 50.0f;
	AttackRadius = 25.0f;
	AttackPower = 100.0f;
	fSprintSpeedMultiPlayer = 3.0f; // 처음은 3.0, 미션수행시 2.5 2.0 1.5 단계로 줄어듬 
	nMissionClear = 0; // 현재 미션 클리어는 0
	//DeathCharacter = NULL; // 일단 죽은 캐릭터는 없다는 

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetActorHiddenInGame(false);
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);
	auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != AnimInstance);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	//ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
	ABAnim->OnOnCollisonStart_Punch.AddUObject(this, &AABCharacter::AttackCheck);
}


// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsPlayer = IsPlayerControlled();

	if (bIsPlayer)
	{
		ABPlayerController = Cast<AABPlayerController>(GetController());
		//SetControlMode(EControlMode::Player);
		ABCHECK(nullptr != ABPlayerController);
	}

	else
	{
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}
	SetCharacterState(ECharacterState::READY);
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
	//PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABCharacter::Run);
	//PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABCharacter::StopRun);
}


void AABCharacter::UpDown(float NewAxisValue)
{
	if (CurrentState == ECharacterState::READY)
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
		Direction.Z = 0.0f;
		Direction.Normalize();
		AddMovementInput(Direction, NewAxisValue);
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	if (CurrentState == ECharacterState::READY)
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);
		Direction.Z = 0.0f;
		Direction.Normalize();
		AddMovementInput(Direction, NewAxisValue);
	}
}

void AABCharacter::LookUp(float NewAxisValue)
{
	if (CurrentState == ECharacterState::READY)
	{
		AddControllerPitchInput(NewAxisValue);
	}

	else
	{
		AddControllerPitchInput(NewAxisValue);
	}
}

void AABCharacter::Turn(float NewAxisValue)
{
	if (CurrentState == ECharacterState::READY)
	{
		AddControllerYawInput(NewAxisValue);
	}
	else
	{
		AddControllerYawInput(NewAxisValue);
	}
}

/*
void AABCharacter::Attack()
{
	if (IsAttacking) return;

	ABAnim->PlayAttackMontage();

	IsAttacking = true;

	//MyAttack.Broadcast();
}


void AABCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed *= fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerRun!"));
	//MyRun.Broadcast();
}

void AABCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed /= fSprintSpeedMultiPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerStopRun!"));
	//MyStopRun.Broadcast();
}
*/
/*
void AABCharacter::BeginCrouch()
{
	this->Crouch();
}

void AABCharacter::EndCrouch()
{
	this->UnCrouch();
}
*/

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // Attack 채널 
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	// 이거는 에디터에서만 사용하는거
	/*
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		//DrawColor,
		false,
		DebugLifeTime);
	*/ 
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerPunch!")); // 플레이어가 펀치하는지 확인용

#endif


	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit!"));
			//ABAnim->SetDeadAnim();
			//SetActorEnableCollision(false);
			//SetCharacterState(ECharacterState::DEAD); // 사망처리
			//AABCharacter* DeadCharacter = Cast<AABCharacter>(HitResult.Actor);
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackPower, DamageEvent, GetController(), this);

			if (nMissionClear > 0)
			{
				nKillingCharacter += 1;
			}
			//CtoS_AttackCheck(DeathCharacter);
			//MultiAttackCheck(HitResult, AttackPower, DamageEvent, GetController(), this);
			//CharacterDead(DeadCharacter);
			
		}
	}
	//MyAttackCheck.Broadcast();
}

float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f) // 일단 맞으면 기절
	{
		//DeathCharacter = this; // 여기서 죽은 캐릭터를 받아온다.
		ABAnim->SetDeadAnim();
		//SetActorEnableCollision(false);
		SetCharacterState(ECharacterState::DEAD); // 사망처리(서버에서는 실행), 클라이언트에서는 따로 해준다.
		MultiAttackCheck(this, this->GetController()); // 클라이언트, 서버에서 모두 실행한다.
	}

	//MyTakeDamage.Broadcast();
	return FinalDamage;
	
}
// TakeDamage 대신에 사용할거?

/*
void AABCharacter::CtoS_AttackCheck_Implementation(AABCharacter* DeadCharacter)
{
	// 서버에서는 모든 PlayerController에게 이벤트를 보낸다.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_AttackCheck();
		}
	}
}

void AABCharacter::StoC_AttackCheck_Implementation(AABCharacter* DeadCharacter)
{
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.

}
*/

// 죽은 캐릭터의 정보를 클라이언트, 서버에 퍼트린다.
// 애니메이션을 출력하되 그 캐릭터는 그 자리에 고정시키고 충돌처리를 없앤다.

void AABCharacter::MultiAttackCheck_Implementation(AABCharacter* DeathCharacter, AController* DeathPlayer)
{
	DeathCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DeathCharacter->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	DeathCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	DeathCharacter->ABAnim->SetDeadAnim();

	if (DeathCharacter->bIsPlayer)
	{
		AABPlayerController* DeathMultiPlayer = Cast<AABPlayerController>(DeathCharacter);
		//DeathCharacter->GetMesh()->DetachFromParent(true); // 여기서 메쉬는 가만히 있고 나머지는 움직이게함
		//DeathCharacter->GetMesh()->SetSkeletalMesh(NULL);
		//DeathCharacter->DisableInput(DeathMultiPlayer);
	}

	//AABCharacter* DeadCharacter = Cast<AABCharacter>(myHitResult.Actor);
	//DeadCharacter->ABAnim->SetDeadAnim();
	//DeadCharacter->SetCharacterState(ECharacterState::DEAD);
}


/*
void AABPlayerController::CtoS_AttackCheck_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
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

void AABPlayerController::StoC_AttackCheck_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// 서버와 클라이언트는 이 이벤트를 받아서 실행한다.
	if (ClientCharacter->IsAttacking) return;

	ClientCharacter->ABAnim->PlayAttackMontage(playPunch);
	ClientCharacter->IsAttacking = true;
}
*/
void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::Player:
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;

		// 회전속도를 함께 지정해 이동 방향으로 캐릭터가 부드럽게 회저하도록 기능을 추가한다.
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

		break;
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// possess된 시점 만약 안되면 이 함수 all 주석 처리
/*
void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ABPlayerController = Cast<AABPlayerController>(NewController);
	
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::Player);
		CurrentState = ECharacterState::READY;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		SetActorLocation(GiveFVector()); // 랜덤배치
		EnableInput(ABPlayerController);
	}

	else
	{
		SetControlMode(EControlMode::NPC);
		CurrentState = ECharacterState::READY;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		SetActorLocation(GiveFVector()); // 랜덤배치
		ABAIController->RunAI();
	}
	
}
*/

// 그리고 몽타주가 끝났을 때
void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 어택 변수를 다시 false로 변환한다.
	ABCHECK(IsAttacking);
	IsAttacking = false;
}

void AABCharacter::CharacterDead(AABCharacter* DeadCharacter)
{
	DeadCharacter->ABAnim->SetDeadAnim();
	DeadCharacter->SetCharacterState(ECharacterState::DEAD);
}

void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		if (bIsPlayer) // 플레이어일 경우
		{
			(EControlMode::Player);
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			SetActorLocation(GiveFVector()); // 랜덤배치
			EnableInput(ABPlayerController);
		}

		else // AI일 경우
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			SetActorLocation(GiveFVector()); // 랜덤배치
			ABAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		if (bIsPlayer)
		{
			//GetMesh()->DetachFromComponent(); // 여기서 메쉬는 가만히 있고 나머지는 움직이게함 true는 메쉬가 보이게 할건지말건지
			//GetMesh()->SetSkeletalMesh(NULL);
			//DisableInput(ABPlayerController); // 나중에 플레이어 사망시 자유시점카메라 변환 
		}

		else
		{
			ABAIController->StopAI();
		}
		break;
	}
	}
}

ECharacterState AABCharacter::GetCharacterState() const
{
	return CurrentState;
}

int AABCharacter::RandomTransform(int min, int max)
{
	std::random_device rd;
	std::mt19937_64 rEngine(rd());
	std::uniform_int_distribution<>dist(min, max);
	return static_cast<int>(dist(rEngine));
}

FVector AABCharacter::GiveFVector()
{
	int nDestinationX, nDestinationY;
	nDestinationX = RandomTransform(-3300, 2500);
	nDestinationY = RandomTransform(-2850, 2850);

	return FVector(nDestinationX, nDestinationY, 218);
}


void AABCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABCharacter, fSprintSpeedMultiPlayer);
	DOREPLIFETIME(AABCharacter, CurrentState);
	DOREPLIFETIME(AABCharacter, IsAttacking);
	DOREPLIFETIME(AABCharacter, AttackPower);
	DOREPLIFETIME(AABCharacter, bIsPlayer);
	DOREPLIFETIME(AABCharacter, nMissionClear);
	//DOREPLIFETIME(AABCharacter, ABAnim);AttackPower

}

