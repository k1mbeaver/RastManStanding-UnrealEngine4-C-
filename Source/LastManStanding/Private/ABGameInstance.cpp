// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"
#include "ACustoDataTables.h"

UABGameInstance::UABGameInstance()
{
	FString PlayerDataPath = TEXT("DataTable'/Game/ThirdPersonCPP/Data/PlayerData.PlayerData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABPLAYER(*PlayerDataPath);
	if (DT_ABPLAYER.Succeeded())
	{
		FPlayerTable = DT_ABPLAYER.Object;
	}
}

FString UABGameInstance::GetUserName(FString Player)
{
	FPlayerData* PlayerData = FPlayerTable->FindRow<FPlayerData>(*Player, TEXT(""));
	FString PlayerNickName = PlayerData->NickName;
	return PlayerNickName;
}

void UABGameInstance::SetUserName(FString Player, FString UserName)
{
	FPlayerData* PlayerData = FPlayerTable->FindRow<FPlayerData>(*Player, TEXT(""));
	PlayerData->NickName = UserName;

	// �ϴ� ������� �ϰ� ���� ������ �ٽ� ��������
}

int UABGameInstance::GetIsServer(FString Player)
{
	FPlayerData* PlayerData = FPlayerTable->FindRow<FPlayerData>(*Player, TEXT(""));
	int PlayerIsServer = PlayerData->IsServer;
	return PlayerIsServer;
}

void UABGameInstance::SetIsServer(FString Player, int ServerAndClient)
{
	FPlayerData* PlayerData = FPlayerTable->FindRow<FPlayerData>(*Player, TEXT(""));
	PlayerData->IsServer = ServerAndClient;

	// �ϴ� ������� �ϰ� ���� ������ �ٽ� ��������
}
