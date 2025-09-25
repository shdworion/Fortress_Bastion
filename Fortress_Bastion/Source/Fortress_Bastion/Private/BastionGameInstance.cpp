// BastionGameInstance.cpp
#include "BastionGameInstance.h"
#include "WebSocketManager.h" // Подключаем наш менеджер

void UBastionGameInstance::Init()
{
	Super::Init();

	// Создаем экземпляр нашего WebSocketManager при старте игры
	// 'this' указывает, что владельцем объекта является GameInstance
	WebSocketManager = NewObject<UWebSocketManager>(this);
}

void UBastionGameInstance::ChangeLanguage(const FString& CultureName)
{
	// Устанавливаем новую культуру (язык и региональные стандарты)
	FInternationalization::Get().SetCurrentCulture(CultureName);
}
