// WebSocketManager.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WebSocketManager.generated.h"

// --- ���������� ���� "��������" (���������) ��� Blueprint ---

// ������ �� �������� ����������� � �������
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnected);
// ������ �� ������ �����������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionError, const FString&, ErrorMessage);
// ������ �� �������� �������� ������ �������� (����� ������ ��� ����������)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAccountCreated, const FString&, PlayerID, const FString&, PlayerName, const FString&, Token);
// ������ �� �������� ����� ������������� ������
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginSuccess);
// ������ � ��������� ������� ����� (������ ����� � ���� ������� ������)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginFailed, const FString&, Reason); // ����������: �������� �������� FString


UCLASS()
class FORTRESS_BASTION_API UWebSocketManager : public UObject
{
    GENERATED_BODY()

public:
    // --- ��������, ��������� � Blueprint ��� �������� ---
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

    // --- �������, ���������� �� Blueprint ---

    // ������������ � ������� �� ���������� ������
    UFUNCTION(BlueprintCallable, Category = "WebSocket")
    void Connect(const FString& Url);

    // ���������� �� ������ ������ �� �������� ������ ��������
    UFUNCTION(BlueprintCallable, Category = "WebSocket")
    void CreateAccount(const FString& PlayerName, const FString& Email, const FString& Password); // ���������

    // ���������� �� ������ ������ �� ���� �� ����� � ������
    UFUNCTION(BlueprintCallable, Category = "WebSocket")
    void LoginWithEmail(const FString& Email, const FString& Password); // ���������

    // ���������� �� ������ ������ �� ���� �� ������������ ������
    UFUNCTION(BlueprintCallable, Category = "WebSocket")
    void LoginWithToken(const FString& PlayerID, const FString& Token);

private:
    TSharedPtr<class IWebSocket> WebSocket;

    // ���������� ������������� ������� ��� �������� ���������
    void SendMessage(const FString& Message);

    // --- ���������� ����������� ������� ������ ---
    void HandleOnConnected();
    void HandleOnConnectionError(const FString& Error);
    void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
    void OnMessageReceived(const FString& Message);
};