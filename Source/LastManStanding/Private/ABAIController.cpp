// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABCharacter.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));

AABAIController::AABAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BTAsset_Component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BBAsset = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void AABAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTAsset);
	BTAsset_Component->StartTree(*BTAsset);
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

UBlackboardComponent* AABAIController::get_blackboard() const
{
	return BBAsset;
}

void AABAIController::RunAI()
{
	if (BBAsset)
	{
		BBAsset->InitializeBlackboard(*BTAsset->BlackboardAsset);
	}
}

void AABAIController::StopAI()
{
	BTAsset_Component->StopTree(EBTStopMode::Safe);
}