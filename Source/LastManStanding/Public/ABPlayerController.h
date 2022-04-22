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
protected:

private:
	UFUNCTION(Server, Unreliable)
		void CtoS_SendMessage(const FString& Message);
	UFUNCTION(Client, Unreliable)
		void StoC_SendMessage(const FString& Message);
	//UFUNCTION(Server, Unreliable)
	//	void CtoS_Attack();
	//UFUNCTION(Client, Unreliable)
	//	void StoC_Attack();
	UPROPERTY()
		class UABAnimInstance* ABAnimInstance;
	UPROPERTY(VisibleInstanceOnly, Replicated)
		class AABCharacter* ABCharacter;
	UPROPERTY(VisibleInstanceOnly, Replicated)
		class APawn* ABPawn;
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Jump();
	void Run();
	//UFUNCTION(NetMulticast, Reliable)
	void StopRun();
	//UFUNCTION(NetMulticast, Reliable)
	void AttackCheck();
	//UFUNCTION(NetMulticast, Reliable)
	void Attack();
};
