// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraFireBolt.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const FString Bolts = NumProjectiles == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get()->Damage_Fire);
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BOLT</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d %s of fire, exploding on impact and dealing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
		Level, ManaCost, Cooldown, NumProjectiles, *Bolts, FMath::RoundToInt(FireDamage));
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level+1));
	const float Cooldown = GetCooldown(Level+1);
	const FString Bolts = NumProjectiles + 1 == 1 ? TEXT("bolt") : TEXT("bolts");
	const float FireDamage = GetDamageByDamageType(Level+1, FAuraGameplayTags::Get()->Damage_Fire);
	return FString::Printf(TEXT(
	// Title
	"<Title>NEXT LEVEL</>\n\n"
	// Details
	"<Default>Level: %d</>\n"
	"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
	"<Small>Cooldown: </><Cooldown>%.1f</>\n"
	// Description
	"<Default>Launches %d %s of fire, exploding on impact and dealing </>"
	"<Damage>%d </>"
	"<Default>fire damage with a chance to burn</>\n\n"),
	Level, ManaCost, Cooldown, NumProjectiles, * Bolts, FMath::RoundToInt(FireDamage));
}
