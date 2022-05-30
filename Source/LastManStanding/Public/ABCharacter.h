// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FMyCharacterDead_Delegate); // 공격 알림
//DECLARE_MULTICAST_DELEGATE(FMyTakeDamage_Delegate); // 데미지 알림
//DECLARE_MULTICAST_DELEGATE(FMyRun_Delegate); // 달리기 알림
//DECLARE_MULTICAST_DELEGATE(FMyStopRun_Delegate); // 달리기 멈춤 알림
//DECLARE_MULTICAST_DELEGATE(FMyAttackCheck_Delegate); // 공격 체크 알림

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
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamasgeCauser) override;
	//virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION(NetMulticast, Reliable)
    void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	
	//UFUNCTION(Server, Reliable)
		//void CtoS_AttackCheck();

	//UFUNCTION(Client, Reliable)
		//void StoC_AttackCheck();


	UFUNCTION(NetMulticast, Reliable)
		//void MultiAttackCheck(FHitResult myHitResult);
		void MultiAttackCheck(AABCharacter* DeathCharacter, AController* DeathPlayer); // 캐릭터와 플레이어인경우 플레이어 컨트롤러도 받아온다.

	//UFUNCTION(Server, Reliable)
		//void CtoS_PlayerDeadCheck(); // 캐릭터와 플레이어인경우 플레이어 컨트롤러도 받아온다.

	//UFUNCTION(Client, Reliable)
	//	void StoC_PlayerDeadCheck(); // 캐릭터와 플레이어인경우 플레이어 컨트롤러도 받아온다.

	//UFUNCTION(Server, Reliable)
	//	void CtoS_MultiIsPlayer();

	//UFUNCTION(Client, Reliable)
	//	void StoC_MultiIsPlayer(); 
/*
	UFUNCTION(Server, Reliable)
		//void MultiAttackCheck(FHitResult myHitResult);
		void CtoS_AttackCheck(AABCharacter* DeadCharacter);

	UFUNCTION(Client, Reliable)
		//void MultiAttackCheck(FHitResult myHitResult);
		void StoC_AttackCheck(AABCharacter* DeadCharacter);
	*/
	void AttackCheck();

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackPower;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = Speed)
		float fSprintSpeedMultiPlayer; // 달리기

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = Mission)
		int nMissionClear; // Mission

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = Mission)
		int nKillingCharacter; // Mission

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = HiddenMision)
		int nPlayerKill; // 플레이어를 죽인 수

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = HiddenMision)
		bool PunchTrue; // 현재 플레이어 수 

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = ControledByPlayer)
		bool IsControlledPlayer; // 현재 플레이어 수 

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = HiddenMision)
		int nNowPlayer = 0; // 현재 플레이어 수 (사용할 때는 nNowPlayer - 1)

	//UPROPERTY(VisibleInstanceOnly, Replicated, Category = Animation)
	UPROPERTY(VisibleInstanceOnly, Category = Animation)
		class UABAnimInstance* ABAnim;

	//UPROPERTY(VisibleInstanceOnly, Replicated, Category = Death)
		//AABCharacter* DeathCharacter;

	UPROPERTY(Transient, Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	FMyCharacterDead_Delegate MyCharacterDead;
	//FMyTakeDamage_Delegate MyTakeDamage;
	//FMyRun_Delegate MyRun;
	//FMyStopRun_Delegate MyStopRun;
	//FMyAttackCheck_Delegate MyAttackCheck;

private:
	int RandomTransform(int min, int max); // 랜덤 좌표 구하기
	FVector GiveFVector(); // 랜덤 좌표 설정

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//UFUNCTION(NetMulticast, Reliable)
	UFUNCTION()
		void CharacterDead(AABCharacter* DeadCharacter);
	UPROPERTY(Transient, Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;
	UPROPERTY()
		class AABAIController* ABAIController;
	UPROPERTY()
		class AABPlayerController* ABPlayerController;
	//UFUNCTION(NetMulticast, Reliable)
	void UpDown(float NewAxisValue);
	//UFUNCTION(NetMulticast, Reliable)
	void LeftRight(float NewAxisValue);
	//UFUNCTION(NetMulticast, Reliable)
	void LookUp(float NewAxisValue);
	//UFUNCTION(NetMulticast, Reliable)
	void Turn(float NewAxisValue);


};
