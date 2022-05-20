// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_StartMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Title_PC.h"

void UUW_StartMenu::ShowUpAnim()
{
	PlayAnimation(ShowUp);
}

void UUW_StartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	CreateServerBtn->OnClicked.AddDynamic(this, &UUW_StartMenu::CreateServerHandler);
	JoinServerBtn->OnClicked.AddDynamic(this, &UUW_StartMenu::JoinServerHandler);
}

void UUW_StartMenu::CreateServerHandler()
{
	ATitle_PC* MyPC = Cast<ATitle_PC>(GetOwningPlayer());
	if (MyPC)
	{
		MyPC->CreateServer();
	}
}

void UUW_StartMenu::JoinServerHandler()
{
	ATitle_PC* MyPC = Cast<ATitle_PC>(GetOwningPlayer());
	if (MyPC)
	{
		MyPC->JoinServer();
	}
}