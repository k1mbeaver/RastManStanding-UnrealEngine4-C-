// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReadyRoom_PC.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AReadyRoom_PC : public APlayerController
{
	GENERATED_BODY()
	
public:

	AReadyRoom_PC();
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		//TSubclassOf<class UUW_ReadyRoomMain> ReadyRoomUIClass;

	//class UUW_ReadyRoomMain* ReadyRoomUIObject;
	//UFUNCTION(Server, Reliable)
		//void CtoS_Play();

	//UFUNCTION(Client, Reliable)
		//void StoC_Play();

	void Play();
	void Decide();
	void Join();
	void VisiblePlay();
	//void Join();
};
