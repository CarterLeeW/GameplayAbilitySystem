// Copyright Carter Wooton


#include "AbilitySystem/Ability/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float SpellDamage = Damage.GetValueAtLevel(Level);
	const float SpellRadius = RadialDamageOuterRadius / 100.f;
	return FString::Printf(TEXT(
		// Title
		"<Title>ARCANE SHARDS</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Spawns %d arcane shards from the ground, dealing damage up to </>"
		"<Damage>%d </>"
		"<Default>arcane damage to enemies within a radius of %.1f meters</>\n\n"),
		Level, ManaCost, Cooldown, NumShards.AsInteger(Level), FMath::RoundToInt(SpellDamage), SpellRadius);
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level+1));
	const float Cooldown = GetCooldown(Level+1);
	const float SpellDamage = Damage.GetValueAtLevel(Level+1);
	const float SpellRadius = RadialDamageOuterRadius / 100.f;
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"
		// Details
		"<Default>Level: %d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Spawns %d arcane shards from the ground, dealing up to </>"
		"<Damage>%d </>"
		"<Default>arcane damage per shard to enemies within a radius of %.1f meters</>\n\n"),
		Level+1, ManaCost, Cooldown, NumShards.AsInteger(Level+1), FMath::RoundToInt(SpellDamage), SpellRadius);
}
