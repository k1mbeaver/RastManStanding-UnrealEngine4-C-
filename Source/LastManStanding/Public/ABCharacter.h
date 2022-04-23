// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FMyAttack_Delegate); // 공격 알림
DECLARE_MULTICAST_DELEGATE(FMyTakeDamage_Delegate); // 데미지 알림
DECLARE_MULTICAST_DELEGATE(FMyRun_Delegate); // 달리기 알림
DECLARE_MULTICAST_DELEGATE(FMyStopRun_Delegate); // 달리기 멈춤 알림
DECLARE_MULTICAST_DELEGATE(FMyAttackCheck_Delegate); // 공격 체크 알림

UCLASS()
class LASTMANSTANDING_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	enum class EControlMode
	{
		Player,
		NPC
	};
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::Player;
	FVector DirectionToMove = FVector::ZeroVector;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamasgeCauser) override;
	//virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	//UPROPERTY(VisibleInstanceOnly, Replicated, Category = Speed)
		//float fSprintSpeedMultiPlayer; // 달리기

	UPROPERTY()
		class UABAnimInstance* ABAnim;

	FMyAttack_Delegate MyAttack;
	FMyTakeDamage_Delegate MyTakeDamage;
	FMyRun_Delegate MyRun;
	FMyStopRun_Delegate MyStopRun;
	FMyAttackCheck_Delegate MyAttackCheck;

private:
	int RandomTransform(int min, int max); // 랜덤 좌표 구하기
	FVector GiveFVector(); // 랜덤 좌표 설정

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;
	UPROPERTY()
		class AABAIController* ABAIController;
	UPROPERTY()
		class AABPlayerController* ABPlayerController;
	//void UpDown(float NewAxisValue);
	//void LeftRight(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	//void Turn(float NewAxisValue);
	//UFUNCTION(NetMulticast, Reliable)
	//void Run();
	//UFUNCTION(NetMulticast, Reliable)
	//void StopRun();
	//UFUNCTION(NetMulticast, Reliable)
	void AttackCheck();
	//UFUNCTION(NetMulticast, Reliable)
	//void Attack();


};
