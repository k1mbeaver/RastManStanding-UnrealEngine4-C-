// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABAIController.h"
#include "ABPlayerController.h"
#include "GameMain_HUD.h"

/*
1. �÷��̾� ��Ʈ�ѷ��� ����, AI ��Ʈ�ѷ��� ����
2. �÷��̾� ���� ����, AI ���� ����
3. �÷��̾� ��Ʈ�ѷ��� �÷��̾� ���� ����, AI ��Ʈ�ѷ��� AI ���� ����
4. ������ ����
*/
AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	HUDClass = AGameMain_HUD::StaticClass();

}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin End"));

}