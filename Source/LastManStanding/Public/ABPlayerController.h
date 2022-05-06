// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 *
 */
UCLASS()
class LASTMANSTANDING_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

		AABPlayerController();
public:
	virtual void PostInitializeComponents() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	void SendMessage(const FText& Text);

	UFUNCTION()
		void FocusChatInputText();

	UFUNCTION()
		void FocusGame();

	//float testkey = 0;

	//UPROPERTY(VisibleInstanceOnly, Replicated, Category = Speed)
		//float fSprintSpeedMultiPlayer; // 달리기
protected:

private:
	UFUNCTION(Server, Unreliable)
		void CtoS_SendMessage(const FString& Message);
	UFUNCTION(Client, Unreliable)
		void StoC_SendMessage(const FString& Message);
	//UFUNCTION(Server, Reliable)
		//void CtoS_Attack();
	//UFUNCTION(Client, Reliable)
		//void StoC_Attack();
	UPROPERTY()
		class UABAnimInstance* ABAnimInstance;
	UPROPERTY(VisibleInstanceOnly, Replicated)
		class AABCharacter* ABCharacter;
	UPROPERTY(VisibleInstanceOnly, Replicated)
		class APawn* ABPawn;
	//void UpDown(float NewAxisValue);
	//void LeftRight(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	//void Turn(float NewAxisValue);
	void Jump();

	// 달리기 함수

	UFUNCTION(Server, Reliable)
		void CtoS_Run(AABCharacter* ClientCharacter);

	UFUNCTION(Client, Reliable)
		void StoC_Run(AABCharacter* ClientCharacter);

	void Run();

	UFUNCTION(Server, Reliable)
		void CtoS_StopRun(AABCharacter* ClientCharacter);

	UFUNCTION(Client, Reliable)
		void StoC_StopRun(AABCharacter* ClientCharacter);

	void StopRun();

	//UFUNCTION(NetMulticast, Reliable)
	//void AttackCheck();
	//UFUNCTION(NetMulticast, Reliable)
	
	// 공격 함수

	UFUNCTION(Server, Reliable)
		void CtoS_Attack(AABCharacter* ClientCharacter, UAnimMontage* playPunch);

	UFUNCTION(Client, Reliable)
		void StoC_Attack(AABCharacter* ClientCharacter, UAnimMontage* playPunch);

	void Attack();
};
