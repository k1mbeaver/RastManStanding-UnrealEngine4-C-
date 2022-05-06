// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// ���� üũ�� ��������Ʈ
DECLARE_MULTICAST_DELEGATE(FOnOnCollisonStart_PunchDelegate); // ���� ����
DECLARE_MULTICAST_DELEGATE(FOnOnCollisonEnd_PunchDelegate); // ���� ��
DECLARE_MULTICAST_DELEGATE(FPunchAnimation_PunchAnimationDelegate); // ���� �ִϸ��̼� ���

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage(UAnimMontage* playPunch);
	UAnimMontage* GetAttackMontage();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	FOnOnCollisonStart_PunchDelegate OnOnCollisonStart_Punch;
	FOnOnCollisonEnd_PunchDelegate OnOnCollisonEnd_Punch;
	FPunchAnimation_PunchAnimationDelegate PunchAnimation_Punch;
	FName GetAttackMontageSectionName(int32 Section);
	void SetDeadAnim();

private:
	UFUNCTION()
		void AnimNotify_OnCollisonStart_Punch();
	UFUNCTION()
		void AnimNotify_OnCollisonEnd_Punch();
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed; // AnimInstance�� ����ؼ� C++ ��ũ���� �� ���� �������Ʈ���� ����� �����ϴ�.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
public:
};
