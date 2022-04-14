// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_GameMain.h"
#include "UW_Chat.h"

TSharedPtr<SWidget> UUW_GameMain::GetChatInputTextObject()
{
	return WB_Chat->GetChatInputTextObject();
}

void UUW_GameMain::AddChatMessage(const FString& Message)
{
	WB_Chat->AddChatMessage(Message);
}