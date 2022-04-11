// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastManStanding.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController(FObjectInitializer const& object_initializer);
	virtual void OnPossess(APawn* InPawn) override;
	void BeginPlay() override;

	class UBlackboardComponent* get_blackboard() const;

	static const FName HomePosKey;
	static const FName PatrolPosKey;

	void RunAI();
	void StopAI();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTAsset;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BTAsset_Component;

	UPROPERTY()
		class UBlackboardComponent* BBAsset;
};
