// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ReadyRoomMain.h"
#include "UW_ReadyRoom.h"

void UUW_ReadyRoomMain::VisiblePlayButton()
{
	WB_ReadyRoom->VisiblePlayButton();
}

void UUW_ReadyRoomMain::VisibleJoinButton()
{
	WB_ReadyRoom->VisibleJoinButton();
}

void UUW_ReadyRoomMain::HiddenIPBox()
{
	WB_ReadyRoom->HiddenIPBox();
}

FString UUW_ReadyRoomMain::GetTextBox()
{
	return WB_ReadyRoom->GetTextBox();
}

FString UUW_ReadyRoomMain::GetIp()
{
	return WB_ReadyRoom->GetIp();
}
/*
void UUW_ReadyRoomMain::VisibleJoinButton()
{
	WB_ReadyRoom_One->VisibleJoinButton();
}
*/