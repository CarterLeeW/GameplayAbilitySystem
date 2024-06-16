// Copyright Carter Wooton

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "AbilityDescriptionsData.generated.h"

USTRUCT(BlueprintType)
struct FAbilityDescriptionsData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityDescriptions")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityDescriptions")
	FString Description_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityDescriptions")
	FString Description_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityDescriptions")
	FString Description_3;
};
