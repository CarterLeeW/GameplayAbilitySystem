// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(BluePrintReadOnly, EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(BluePrintReadOnly, EditDefaultsOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(BluePrintReadOnly, EditDefaultsOnly)
	int32 SpellPointReward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	UFUNCTION(BlueprintCallable)
	int32 FindLevelForExp(int32 Exp) const;
};
