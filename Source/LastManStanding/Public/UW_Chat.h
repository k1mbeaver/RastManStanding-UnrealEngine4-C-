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
	TSharedPtr<class SWidget> GetChatInputTextObject(); // 나중에 enter 누르면 채팅에 포커싱하기

private:
	UPROPERTY(Meta = (BindWidget))
		class UScrollBox* ChatHistoryArea;

	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* ChatInputText;

	// 이 함수를 쓰기위해서는 Build파일에 UMG 모듈 추가 Slat, SlateCore 주석해제

	UFUNCTION()
		void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
