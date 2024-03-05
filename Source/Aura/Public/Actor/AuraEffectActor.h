// Copyright Carter Wooton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;


UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	// Instant
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	const TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Duration
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	const TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Infinite
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	const TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandleMap;
private:

};
