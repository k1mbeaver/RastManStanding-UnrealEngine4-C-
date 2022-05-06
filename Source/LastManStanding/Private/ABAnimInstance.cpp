// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Mannequin/Animations/ThirdPerson_Punch_Montage.ThirdPerson_Punch_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner(); // 폰에 접근해서 폰의 속력 값을 얻어온다.
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage(UAnimMontage* playPunch)
{
	ABCHECK(!IsDead);
	Montage_Play(playPunch, 1.0f);
	PunchAnimation_Punch.Broadcast();
}

UAnimMontage* UABAnimInstance::GetAttackMontage()
{
	return AttackMontage;
}

void UABAnimInstance::AnimNotify_OnCollisonStart_Punch()
{
	ABLOG_S(Warning);
	OnOnCollisonStart_Punch.Broadcast();
}

void UABAnimInstance::AnimNotify_OnCollisonEnd_Punch()
{
	ABLOG_S(Warning);
	OnOnCollisonEnd_Punch.Broadcast();
}

void UABAnimInstance::SetDeadAnim()
{
	IsDead = true;
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}