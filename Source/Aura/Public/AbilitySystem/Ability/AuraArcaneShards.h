// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (ClampMin = "1", UIMin = "1", ClampMax = "5", UIMax = "5"))
	FScalableFloat NumShards = 1.0f;

};
