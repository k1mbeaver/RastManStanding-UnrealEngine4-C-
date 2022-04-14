// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LastManStanding, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(LastManStanding, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(LastManStanding, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	//PREINIT, // 캐릭터 생성 전의 스테이트, 
	//LOADING, // 선택한 캐릭터 애셋을 로딩
	READY, // 준비가 완료, 평상 시의 상태 (플레이어, AI)
	DEAD // Death, 플레이어는 자유 시점 카메라, AI는 비헤이비어 트리 구동을 멈춤
};