// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

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

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Attack();
	void Run();
	void StopRun();
	int RandomTransform(int min, int max); // ·£´ý ÁÂÇ¥ ±¸ÇÏ±â
	FVector GiveFVector(); // ·£´ý ÁÂÇ¥ ¼³Á¤

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY()
		float fSprintSpeedMultiPlayer; // ´Þ¸®±â
	UPROPERTY()
		class UABAnimInstance* ABAnim;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;
	UPROPERTY()
		class AABAIController* ABAIController;
	UPROPERTY()
		class AABPlayerController* ABPlayerController;

	void AttackCheck();
};
