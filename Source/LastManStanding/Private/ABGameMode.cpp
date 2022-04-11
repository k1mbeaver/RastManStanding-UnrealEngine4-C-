// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

/*
1. 플레이어 컨트롤러의 생성
2. 플레이어 폰의 생성
3. 플레이어 컨트롤러가 플레이어 폰을 빙의
4. 게임의 시작
*/
AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin End"));

}