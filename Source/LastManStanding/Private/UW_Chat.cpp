// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Chat.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "ABPlayerController.h"

void UUW_Chat::NativeConstruct()
{
	Super::NativeConstruct();

	ChatInputText->OnTextCommitted.AddDynamic(this, &UUW_Chat::OnChatTextCommitted);
}

void UUW_Chat::AddChatMessage(const FString& Message)
{
	// Text ������Ʈ�� �����ϰ�, ScrollBox�� �߰��Ѵ�.
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatHistoryArea);
	NewTextBlock->SetText(FText::FromString(Message));

	ChatHistoryArea->AddChild(NewTextBlock);
	ChatHistoryArea->ScrollToEnd(); // ���� �ֱ� ä���� ���� ����, ��ũ���� ���� �Ʒ��� ������.
}

void UUW_Chat::SetChatInputTextMessage(const FText& Text)
{
	ChatInputText->SetText(Text);
}

TSharedPtr<SWidget> UUW_Chat::GetChatInputTextObject()
{
	return ChatInputText->GetCachedWidget();
}

void UUW_Chat::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	AABPlayerController* MyPC = Cast<AABPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPC == nullptr) return;

	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
		if (Text.IsEmpty() == false)
		{
			MyPC->SendMessage(Text); // �޽��� ����.
			SetChatInputTextMessage(FText::GetEmpty()); // �޼��� ����������, �����.
		}
		MyPC->FocusGame(); // �ٽ� �������� ��Ŀ��.
		break;
	case ETextCommit::OnCleared:
		MyPC->FocusGame(); // �ٽ� �������� ��Ŀ��.
		break;
	}
}
