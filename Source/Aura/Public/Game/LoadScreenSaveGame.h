// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus : uint8
{
	ESSS_Vacant,
	ESSS_EnterName,
	ESSS_Taken
};

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* Identifiers */
	UPROPERTY()
	FString SlotName = FString();
	UPROPERTY()
	int32 SlotIndex = 0;
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	UPROPERTY()
	FString CurrentMapName = FString("Default Map");

	UPROPERTY()
	ESaveSlotStatus SaveSlotStatus = ESaveSlotStatus::ESSS_Vacant;

	UPROPERTY()
	FName PlayerStartTag;

	/* Player variables */
	UPROPERTY()
	int32 PlayerLevel;
	UPROPERTY()
	int32 Exp;
	UPROPERTY()
	int32 SpellPoints;
	UPROPERTY()
	int32 AttributePoints;
	UPROPERTY()
	float Strength;
	UPROPERTY()
	float Inteligence;
	UPROPERTY()
	float Resilience;
	UPROPERTY()
	float Vigor;
};
