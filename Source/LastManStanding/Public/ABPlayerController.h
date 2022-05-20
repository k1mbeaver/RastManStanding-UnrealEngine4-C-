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

	UPROPERTY(VisibleInstanceOnly, Replicated)
		class AABCharacter* myCharacter;
	UPROPERTY(VisibleInstanceOnly, Replicated)
		class APawn* myPawn;

	UPROPERTY(VisibleInstanceOnly, Replicated)
		bool bMissionClear = false;

	UPROPERTY(VisibleInstanceOnly, Replicated)
		bool bCanRun = true;

	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);

	// ���� ��

	UFUNCTION(Server, Unreliable)
		void CtoS_GameEnd(const FString& WinnerName);

	UFUNCTION(Client, Unreliable)
		void StoC_GameEnd(const FString& WinnerName);

	void GameEnd(const FString& WinnerName);

	void SetWinnerName(const FString& WinnerName);
	void VisibleGameover();

	//float testkey = 0;

	//UPROPERTY(VisibleInstanceOnly, Replicated, Category = Speed)
		//float fSprintSpeedMultiPlayer; // �޸���
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
	//void UpDown(float NewAxisValue);
	//void LeftRight(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	//void Turn(float NewAxisValue);

	// ���� �Լ�

	void Jump();

	void CheckMission();

	// �޸��� �Լ�

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
	
	// ���� �Լ�

	UFUNCTION(Server, Reliable)
		void CtoS_Attack(AABCharacter* ClientCharacter, UAnimMontage* playPunch);

	UFUNCTION(Client, Reliable)
		void StoC_Attack(AABCharacter* ClientCharacter, UAnimMontage* playPunch);

	void Attack();
};
