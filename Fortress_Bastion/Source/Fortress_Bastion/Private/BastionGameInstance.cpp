// BastionGameInstance.cpp
#include "BastionGameInstance.h"
#include "WebSocketManager.h" // ���������� ��� ��������

void UBastionGameInstance::Init()
{
	Super::Init();

	// ������� ��������� ������ WebSocketManager ��� ������ ����
	// 'this' ���������, ��� ���������� ������� �������� GameInstance
	WebSocketManager = NewObject<UWebSocketManager>(this);
}

void UBastionGameInstance::ChangeLanguage(const FString& CultureName)
{
	// ������������� ����� �������� (���� � ������������ ���������)
	FInternationalization::Get().SetCurrentCulture(CultureName);
}
