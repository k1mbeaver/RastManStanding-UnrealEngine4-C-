// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Chat.generated.h"

/**
 * 
 */
UCLASS()
class LASTMANSTANDING_API UUW_Chat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void AddChatMessage(const FString& Message);
	void SetChatInputTextMessage(const FText& Text);
	TSharedPtr<class SWidget> GetChatInputTextObject(); // ���߿� enter ������ ä�ÿ� ��Ŀ���ϱ�

private:
	UPROPERTY(Meta = (BindWidget))
		class UScrollBox* ChatHistoryArea;

	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* ChatInputText;

	// �� �Լ��� �������ؼ��� Build���Ͽ� UMG ��� �߰� Slat, SlateCore �ּ�����

	UFUNCTION()
		void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
