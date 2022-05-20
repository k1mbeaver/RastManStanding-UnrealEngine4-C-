// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ACustoDataTables.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPlayerData() : NickName("Player2"), IsServer(1) {}

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString NickName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int IsServer;
};
UCLASS()
class LASTMANSTANDING_API AACustoDataTables : public AActor
{
	GENERATED_BODY()

};
