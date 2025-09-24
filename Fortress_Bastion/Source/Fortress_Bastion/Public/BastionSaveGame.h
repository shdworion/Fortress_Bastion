//BastionSaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BastionSaveGame.generated.h"

// Структура для хранения данных одного аккаунта
USTRUCT(BlueprintType)
struct FAccountData
{
	GENERATED_BODY()

	// Уникальный ID игрока, используется как логин
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Account Data")
	FString Login;

	// Токен для автоматической авторизации
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Account Data")
	FString AuthToken;
};

/**
 * Класс, который будет сохраняться на диск.
 * Он содержит массив всех аккаунтов, которые когда-либо входили в игру на этом устройстве.
 */
UCLASS(Blueprintable)
class FORTRESS_BASTION_API UBastionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Массив сохраненных аккаунтов
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save Game Data")
	TArray<FAccountData> SavedAccounts;
};
