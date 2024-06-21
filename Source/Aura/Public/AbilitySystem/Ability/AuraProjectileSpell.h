// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectileSpell(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ClampMin = "1", UIMin = "1", ClampMax = "5", UIMax = "5"))
	FScalableFloat NumProjectiles = 1.f;
};
