// BastionGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Internationalization/Internationalization.h" // ��� ����� �����
#include "BastionGameInstance.generated.h"

class UWebSocketManager;

UCLASS()
class FORTRESS_BASTION_API UBastionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// ��� ������� ���������� ���� ��� ��� ����� ������ ����
	virtual void Init() override;

	// ���������� ��� �������� ������ ��������� �������. 
	// UPROPERTY() �������� ������ �� �������� ������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
	UWebSocketManager* WebSocketManager;

	// ������� ��� ����� �����, ��������� � Blueprints
	UFUNCTION(BlueprintCallable, Category = "Localization")
	void ChangeLanguage(const FString& CultureName);
};