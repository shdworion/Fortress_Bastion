//BastionSaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BastionSaveGame.generated.h"

// ��������� ��� �������� ������ ������ ��������
USTRUCT(BlueprintType)
struct FAccountData
{
	GENERATED_BODY()

	// ���������� ID ������, ������������ ��� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Account Data")
	FString Login;

	// ����� ��� �������������� �����������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Account Data")
	FString AuthToken;
};

/**
 * �����, ������� ����� ����������� �� ����.
 * �� �������� ������ ���� ���������, ������� �����-���� ������� � ���� �� ���� ����������.
 */
UCLASS(Blueprintable)
class FORTRESS_BASTION_API UBastionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// ������ ����������� ���������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save Game Data")
	TArray<FAccountData> SavedAccounts;
};
