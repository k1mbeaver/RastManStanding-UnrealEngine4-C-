// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "ABAIController.h"
#include "ABPlayerController.h"
#include "Kismet/GameplayStatics.h"
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

	IsAttacking = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	AttackRange = 50.0f;
	AttackRadius = 25.0f;
	fSprintSpeedMultiPlayer = 3.0f; // ó���� 3.0, �̼Ǽ���� 2.5 2.0 1.5 �ܰ�� �پ�� 

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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABCharacter::StopRun);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AABCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AABCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AABCharacter::Attack_Implementation()
{
	if (IsAttacking) return;

	ABAnim->PlayAttackMontage();

	IsAttacking = true;

	MyAttack.Broadcast();
}


void AABCharacter::Run_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed *= fSprintSpeedMultiPlayer;
	MyRun.Broadcast();
}

void AABCharacter::StopRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed /= fSprintSpeedMultiPlayer;
	MyStopRun.Broadcast();
}

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

void AABCharacter::AttackCheck_Implementation()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // Attack ä��
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerPunch!")); // �÷��̾ ��ġ�ϴ��� Ȯ�ο�

#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit!"));

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(100.0f, DamageEvent, GetController(), this);
		}
	}

	MyAttackCheck.Broadcast();
}

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

		// ȸ���ӵ��� �Բ� ������ �̵� �������� ĳ���Ͱ� �ε巴�� ȸ���ϵ��� ����� �߰��Ѵ�.
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

// possess�� ���� ���� �ȵǸ� �� �Լ� all �ּ� ó��
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
		SetActorLocation(GiveFVector()); // ������ġ
		EnableInput(ABPlayerController);
	}

	else
	{
		SetControlMode(EControlMode::NPC);
		CurrentState = ECharacterState::READY;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		SetActorLocation(GiveFVector()); // ������ġ
		ABAIController->RunAI();
	}
}
*/

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
}

float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f) // �ϴ� ������ ����
	{
		ABAnim->SetDeadAnim();
		//SetActorEnableCollision(false);
		SetCharacterState(ECharacterState::DEAD); // ���ó��
	}

	MyTakeDamage.Broadcast();
	return FinalDamage;
}

void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		if (bIsPlayer) // �÷��̾��� ���
		{
			SetControlMode(EControlMode::Player);
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			SetActorLocation(GiveFVector()); // ������ġ
			EnableInput(ABPlayerController);
		}

		else // AI�� ���
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			SetActorLocation(GiveFVector()); // ������ġ
			ABAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController); // ���߿� �÷��̾� ����� ��������ī�޶� ��ȯ 
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
}
