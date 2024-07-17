// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float FireDamage = Damage.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BLAST</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d balls of fire in a circle from the caster, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level, ManaCost, Cooldown, NumFireBalls, FMath::RoundToInt(FireDamage));
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level + 1));
	const float Cooldown = GetCooldown(Level + 1);
	const float FireDamage = Damage.GetValueAtLevel(Level + 1);
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d balls of firein a circle from the caster, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level + 1, ManaCost, Cooldown, NumFireBalls, FMath::RoundToInt(FireDamage));
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}
