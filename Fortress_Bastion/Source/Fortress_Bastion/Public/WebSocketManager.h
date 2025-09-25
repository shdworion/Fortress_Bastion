// WebSocketManager.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WebSocketManager.generated.h"

// --- Объявление всех "сигналов" (делегатов) для Blueprint ---

// Сигнал об успешном подключении к серверу
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnected);
// Сигнал об ошибке подключения
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionError, const FString&, ErrorMessage);

// Сигнал об успешном создании нового аккаунта (несет данные для сохранения)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAccountCreated, const FString&, PlayerID, const FString&, PlayerName, const FString&, Token);
// Сигнал об успешном входе существующего игрока
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginSuccess);
/// Сигнал о неудачной попытке входа (несет сообщение об ошибке)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginFailed, const FString&, ErrorMessage);


UCLASS(Blueprintable, BlueprintType)
class FORTRESS_BASTION_API UWebSocketManager : public UObject
{
	GENERATED_BODY()

public:
	// --- Делегаты, доступные в Blueprint для подписки ---
	UPROPERTY(BlueprintAssignable, Category = "WebSocket Events")
	FOnConnected OnConnected;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket Events")
	FOnConnectionError OnConnectionError;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket Events")
	FOnAccountCreated OnAccountCreated;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket Events")
	FOnLoginSuccess OnLoginSuccess;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket Events")
	FOnLoginFailed OnLoginFailed;

	// --- Функции, вызываемые из Blueprint ---
	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Connect(const FString& Url);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void SendMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void LoginWithToken(const FString& PlayerID, const FString& Token);

private:
	TSharedPtr<class IWebSocket> WebSocket;

	// --- Внутренние обработчики событий сокета ---
	void HandleOnConnected();
	void HandleOnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessageReceived(const FString& Message);
};
