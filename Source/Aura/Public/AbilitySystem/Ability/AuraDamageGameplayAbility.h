// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/* Damage effect parameters */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DebuffPeriod = 1.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;
};
