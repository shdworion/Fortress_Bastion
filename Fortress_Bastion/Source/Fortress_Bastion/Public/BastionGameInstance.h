// BastionGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Internationalization/Internationalization.h" // Для смены языка
#include "BastionGameInstance.generated.h"

class UWebSocketManager;

UCLASS()
class FORTRESS_BASTION_API UBastionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Эта функция вызывается один раз при самом старте игры
	virtual void Init() override;

	// Переменная для хранения нашего менеджера сокетов. 
	// UPROPERTY() защищает объект от сборщика мусора.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
	UWebSocketManager* WebSocketManager;

	// Функция для смены языка, доступная в Blueprints
	UFUNCTION(BlueprintCallable, Category = "Localization")
	void ChangeLanguage(const FString& CultureName);
};