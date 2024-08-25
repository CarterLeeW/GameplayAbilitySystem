// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraPassiveAbility.h"
#include "AuraManaSiphon.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraManaSiphon : public UAuraPassiveAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (ClampMin = "1", UIMin = "1", ClampMax = "30", UIMax = "30"))
	FScalableFloat PercentOfManaRestored = 1.0f;
};
