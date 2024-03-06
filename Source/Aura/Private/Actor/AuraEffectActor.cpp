// Copyright Carter Wooton


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		if (bIsInfinite
			&& (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
			&& ActiveEffectHandle.WasSuccessfullyApplied())
		{
			ActiveEffectHandleMap.Add(ActiveEffectHandle, TargetASC);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// Instant
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (auto& EffectClass : InstantGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	// Duration
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (auto& EffectClass : DurationGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	// Infinite
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (auto& EffectClass : InfiniteGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// Instant
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (auto& EffectClass : InstantGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	// Duration
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (auto& EffectClass : DurationGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	// Infinite
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (auto& EffectClass : InfiniteGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}

	// Infinite Removal
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(TargetASC))
		{
			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			// Find matching effects and remove from target ASC
			for (auto HandlePair : ActiveEffectHandleMap)
			{
				if (TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}
			// Remove matching effects from active effects map
			for (auto& Handle : HandlesToRemove)
			{
				ActiveEffectHandleMap.FindAndRemoveChecked(Handle);
			}
		}
	}
}
