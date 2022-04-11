// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// 공격 체크용 딜리게이트
DECLARE_MULTICAST_DELEGATE(FOnOnCollisonStart_PunchDelegate); // 공격 시작
DECLARE_MULTICAST_DELEGATE(FOnOnCollisonEnd_PunchDelegate); // 공격 끝

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

	void PlayAttackMontage();
	FOnOnCollisonStart_PunchDelegate OnOnCollisonStart_Punch;
	FOnOnCollisonEnd_PunchDelegate OnOnCollisonEnd_Punch;
	FName GetAttackMontageSectionName(int32 Section);
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
		void AnimNotify_OnCollisonStart_Punch();
	UFUNCTION()
		void AnimNotify_OnCollisonEnd_Punch();
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed; // AnimInstance를 사용해서 C++ 스크립팅 한 것을 블루프린트에서 사용이 가능하다.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
public:
};
