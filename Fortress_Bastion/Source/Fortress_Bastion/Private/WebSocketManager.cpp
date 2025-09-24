// WebSocketManager.cpp

#include "WebSocketManager.h"
#include "WebSocketsModule.h" // Не забудьте добавить "WebSockets" в Build.cs
#include "IWebSocket.h"
#include "Json.h"
#include "JsonUtilities.h"

void UWebSocketManager::Connect(const FString& Url)
{
    // Проверяем, загружен ли модуль WebSockets, чтобы избежать ошибок
    if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
    {
        FModuleManager::Get().LoadModule("WebSockets");
    }

    // Создаем объект сокета
    WebSocket = FWebSocketsModule::Get().CreateWebSocket(Url);

    // Привязываем наши C++ функции к событиям сокета
    WebSocket->OnConnected().AddUObject(this, &UWebSocketManager::HandleOnConnected);
    WebSocket->OnConnectionError().AddUObject(this, &UWebSocketManager::HandleOnConnectionError);
    WebSocket->OnClosed().AddUObject(this, &UWebSocketManager::OnClosed);
    WebSocket->OnMessage().AddUObject(this, &UWebSocketManager::OnMessageReceived);

    UE_LOG(LogTemp, Warning, TEXT("Connecting to WebSocket server: %s"), *Url);
    WebSocket->Connect();
}

void UWebSocketManager::SendMessage(const FString& Message)
{
    // Отправляем сообщение, только если соединение установлено
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        WebSocket->Send(Message);
    }
}

// ДОБАВЛЕНО: Реализация функции создания аккаунта
void UWebSocketManager::CreateAccount(const FString& PlayerName, const FString& Email, const FString& Password)
{
    // Формируем JSON-строку для запроса
    FString Request = FString::Printf(
        TEXT("{\"action\":\"create_account\", \"player_name\":\"%s\", \"email\":\"%s\", \"password\":\"%s\"}"),
        *PlayerName, *Email, *Password
    );
    SendMessage(Request);
}

// ДОБАВЛЕНО: Реализация функции входа по почте
void UWebSocketManager::LoginWithEmail(const FString& Email, const FString& Password)
{
    // Формируем JSON-строку для запроса
    FString Request = FString::Printf(
        TEXT("{\"action\":\"login_with_email\", \"email\":\"%s\", \"password\":\"%s\"}"),
        *Email, *Password
    );
    SendMessage(Request);
}

void UWebSocketManager::LoginWithToken(const FString& PlayerID, const FString& Token)
{
    // Формируем JSON-строку для запроса
    FString Request = FString::Printf(
        TEXT("{\"action\":\"login_with_token\", \"player_id\":\"%s\", \"token\":\"%s\"}"),
        *PlayerID, *Token
    );
    SendMessage(Request);
}

// --- Обработчики событий ---

void UWebSocketManager::HandleOnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket Connected!"));
    // Транслируем событие в Blueprint
    OnConnected.Broadcast();
}

void UWebSocketManager::HandleOnConnectionError(const FString& Error)
{
    UE_LOG(LogTemp, Error, TEXT("WebSocket Connection Error: %s"), *Error);
    // Транслируем событие с текстом ошибки в Blueprint
    OnConnectionError.Broadcast(Error);
}

void UWebSocketManager::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket Closed. Code: %d, Reason: %s"), StatusCode, *Reason);
    // Сюда можно добавить трансляцию события OnDisconnected, если нужно
}

void UWebSocketManager::OnMessageReceived(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("WebSocket Received: %s"), *Message);

    // Пытаемся распарсить входящее сообщение как JSON
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString Action;
        // Ищем поле 'action', чтобы понять тип сообщения
        if (JsonObject->TryGetStringField(TEXT("action"), Action))
        {
            if (Action == TEXT("create_account_success"))
            {
                FString PlayerID, PlayerName, Token;
                // Извлекаем все необходимые данные
                if (JsonObject->TryGetStringField(TEXT("player_id"), PlayerID) &&
                    JsonObject->TryGetStringField(TEXT("player_name"), PlayerName) &&
                    JsonObject->TryGetStringField(TEXT("token"), Token))
                {
                    // Транслируем событие в Blueprint, передавая все данные
                    OnAccountCreated.Broadcast(PlayerID, PlayerName, Token);
                }
            }
            else if (Action == TEXT("login_success"))
            {
                OnLoginSuccess.Broadcast();
            }
            else if (Action == TEXT("login_failed"))
            {
                // ИСПРАВЛЕНО: Пытаемся получить причину с сервера, если ее нет - используем стандартный текст
                FString Reason = TEXT("Unknown error. Please try again.");
                JsonObject->TryGetStringField(TEXT("reason"), Reason);
                OnLoginFailed.Broadcast(Reason);
            }
            // Сюда можно добавлять обработку других "action" от сервера
        }
    }
}