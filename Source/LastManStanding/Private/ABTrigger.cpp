// Fill out your copyright notice in the Description page of Project Settings.


#include "ABTrigger.h"
#include "ABCharacter.h"

void AABTrigger::TriggerWithCharacter(AActor* TriggerCharacter)
{
	AABCharacter* RealCharacter = Cast<AABCharacter>(TriggerCharacter);

	RealCharacter->fSprintSpeedMultiPlayer = 1.0f;
	RealCharacter->nMissionClear = 1;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Mission Clear!")); // 플레이어가 펀치하는지 확인용
}
