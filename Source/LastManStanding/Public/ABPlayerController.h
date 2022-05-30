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

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted); // 펀치가 끝났을 때 사용할 거

	UPROPERTY(VisibleInstanceOnly, Replicated)
		class AABCharacter* myCharacter;

	UPROPERTY(VisibleInstanceOnly, Replicated)
		class APawn* myPawn;

	UPROPERTY(VisibleInstanceOnly, Replicated)
		bool bMissionClear = false;

	UPROPERTY(VisibleInstanceOnly, Replicated)
		bool bCanRun = true;

	UPROPERTY()
		bool bCanDeligate = false;

	void SetPlayerMissionClear(int nMissionClear);
	void PlayerKillingMissionAppear();
	void SetPlayerKillingPoint(int nKillingPoint);

	// 게임 끝

	UFUNCTION(Server, Unreliable)
		void CtoS_GameEnd(const FString& WinnerName);

	UFUNCTION(Client, Unreliable)
		void StoC_GameEnd(const FString& WinnerName);

	void GameEnd(const FString& WinnerName);

	void SetWinnerName(const FString& WinnerName);
	void VisibleGameover();

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
	//void UpDown(float NewAxisValue);
	//void LeftRight(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	//void Turn(float NewAxisValue);

	// 점프 함수

	void Jump();

	void CheckMission();

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

	// 서버 플레이어 확인함수 OnPossess에서 사용하자, 그리고 공격할 때 체크하는걸로?
	
	UFUNCTION(Server, Reliable)
		void CtoS_PlayerEnter();

	UFUNCTION(Client, Reliable)
		void StoC_PlayerEnter(int PlayerCount);

	void PlayerEnter();

	// 캐릭터가 죽었는 지 확인하는 함수

	UFUNCTION(Server, Reliable)
		void CtoS_PlayerKill();

	UFUNCTION(Client, Reliable)
		void StoC_PlayerKill(int nPlayerKill);

	void PlayerKill();
	
};
