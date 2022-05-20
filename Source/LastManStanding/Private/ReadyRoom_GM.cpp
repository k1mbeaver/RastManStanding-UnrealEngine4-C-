// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyRoom_GM.h"
#include "ReadyRoom_PC.h"
#include "ReadyRoomMain_HUD.h"
#include "Kismet/GameplayStatics.h"

AReadyRoom_GM::AReadyRoom_GM()
{
	PlayerControllerClass = AReadyRoom_PC::StaticClass();
	HUDClass = AReadyRoomMain_HUD::StaticClass();
}
/*
void AReadyRoom_GM::BeginPlay()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	AActor* OutActor = OutActors[0];
	AReadyRoom_PC* PC = Cast<AReadyRoom_PC>(OutActor);
	PC->VisiblePlay();
}
*/
void AReadyRoom_GM::PostLogin(APlayerController* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("PostLogin End"));

}