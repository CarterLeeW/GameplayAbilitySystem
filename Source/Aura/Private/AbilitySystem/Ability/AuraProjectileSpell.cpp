// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

FString UAuraProjectileSpell::GetDescription(int32 Level) const
{
	const FString Bolts = NumProjectiles == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = DamageTypes.Find(FAuraGameplayTags::Get()->Damage_Fire)->GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d %s of fire, exploding on impact and dealing </><Damage>%d</> <Default>fire damage with a chance to burn</>\n\n<Default>Level: %d</>"),
			NumProjectiles, *Bolts, FMath::RoundToInt(FireDamage), Level);
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level) const
{
	const FString Bolts = NumProjectiles+1 == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = DamageTypes.Find(FAuraGameplayTags::Get()->Damage_Fire)->GetValueAtLevel(Level+1);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n<Default>Launches %d %s of fire, exploding on impact and dealing </><Damage>%d</> <Default>fire damage with a chance to burn</>\n\n<Default>Level: %d</>"),
		NumProjectiles+1, *Bolts, FMath::RoundToInt(FireDamage), Level+1);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectileSpell(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	// We are on the server
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
			GetAvatarActorFromActorInfo(),
			SocketTag);

		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		//Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		check(DamageEffectClass);

		// Configure EffectContextHandle
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		// Loop through damage types and set by caller magnitude
		for (const auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
