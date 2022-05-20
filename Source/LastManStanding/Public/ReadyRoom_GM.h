// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReadyRoom_GM.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AReadyRoom_GM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AReadyRoom_GM();
	//virtual void BeginPlay();
	virtual void PostLogin(APlayerController* NewPlayer) override; // �� �������� �÷��̾� ������ ����ȴ�.
};
