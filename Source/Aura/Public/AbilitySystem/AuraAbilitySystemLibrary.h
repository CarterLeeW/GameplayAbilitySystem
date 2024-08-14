// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
struct FWidgetControllerParams;
class AAuraHUD;
class UAbilityInfo;
class ULoadScreenSaveGame;
class ULootTiers;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Widgets */
	UFUNCTION(Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/* Ability System class defaults */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static ULootTiers* GetLootTiers(const UObject* WorldContextObject);

	/* Begin Effect Context Getters */
#pragma region EffectContextGetters
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->bIsCriticalHit;
		}
		return false;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->bIsBlockedHit;
		}
		return false;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->bIsSuccessfulDebuff;
		}
		return false;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->DebuffDamage;
		}
		return 0.f;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->DebuffDuration;
		}
		return 0.f;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffPeriod(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->DebuffPeriod;
		}
		return 0.f;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			if (AuraEffectContext->DamageType.IsValid())
			{
				return *AuraEffectContext->DamageType;
			}
		}
		return FGameplayTag();
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->DeathImpulse;
		}
		return FVector::ZeroVector;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->bIsRadialDamage;
		}
		return false;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->RadialDamageInnerRadius;
		}
		return 0.f;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->RadialDamageOuterRadius;
		}
		return 0.f;
	}
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
	{
		if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			return AuraEffectContext->RadialDamageOrigin;
		}
		return FVector::ZeroVector;
	}
	/* End Effect Context Getters */

	/* Begin Effect Context Setters */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->bIsCriticalHit = bInIsCriticalHit;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->bIsBlockedHit = bInIsBlockedHit;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->bIsSuccessfulDebuff = bInIsSuccessfulDebuff;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->DebuffDamage = InDebuffDamage;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->DebuffDuration = InDebuffDuration;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffPeriod(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffPeriod)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->DebuffPeriod = InDebuffPeriod;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->DamageType = MakeShared<FGameplayTag>(InDamageType);
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->DeathImpulse = InDeathImpulse;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->bIsRadialDamage = bInIsRadialDamage;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->RadialDamageInnerRadius = InRadialDamageInnerRadius;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->RadialDamageOuterRadius = InRadialDamageOuterRadius;
		}
	}
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin)
	{
		if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			AuraEffectContext->RadialDamageOrigin = InRadialDamageOrigin;
		}
	}
#pragma endregion
	/* End Effect Context Setters */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics", meta = (WorldContext = "WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin, TArray<AActor*>& OutOverlappingActors);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<AActor*> GetClosestTargets(int32 MaxTargets, TArray<AActor*> ActorsCopy, const FVector& Origin);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	static int32 GetExpRewardForCharacterClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AuraAbilitySystemLibrary|Math")
	static float GetValueAtLevel_Float(int32 Level, const FScalableFloat& Value) { return Value.GetValueAtLevel(Level); }
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AuraAbilitySystemLibrary|Math")
	static int32 GetValueAtLevel_Int32(int32 Level, const FScalableFloat& Value) { return Value.AsInteger(Level); }
};
