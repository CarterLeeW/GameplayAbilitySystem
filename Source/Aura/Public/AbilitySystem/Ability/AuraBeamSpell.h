// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CacheMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget();
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(bool bIncludeFriendlyActors, TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam", meta = (ClampMin = "1", UIMin = "1"))
	FScalableFloat ChainRadius = 850.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam", meta = (ClampMin = "1", UIMin = "1", ClampMax = "10", UIMax = "10"))
	FScalableFloat MaxShockTargets = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = ".0001", UIMin = ".0001", ClampMax = "10", UIMax = "10"))
	float DamagePeriod = 0.1f;
};
