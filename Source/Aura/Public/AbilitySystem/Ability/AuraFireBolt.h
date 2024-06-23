// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const AActor* HomingTarget = nullptr, float PitchOverride = 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "30", UIMin = "30", ClampMax = "270", UIMax = "270"))
	float SpawnSpread = 90.f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMin = 1600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMax = 3200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bShouldBeHoming = true;
};
